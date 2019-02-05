#ifndef __CINT__
#include <RooGlobalFunc.h>
#endif

#include <RooMinimizer.h>
#include <RooConstVar.h>
#include <RooRealVar.h>
#include <RooFormulaVar.h>
#include <RooDataHist.h>
#include <RooGaussian.h>
#include <RooGaussModel.h>
#include <RooAddModel.h>
#include <RooGenericPdf.h>
#include <RooAddPdf.h>
#include <RooMinuit.h>
#include <RooFFTConvPdf.h>
#include <RooNumConvPdf.h>
#include <RooSimultaneous.h>
#include <RooPolynomial.h>
#include <RooCategory.h>
#include <RooChi2Var.h>
#include <RooDecay.h>
#include <RooPlot.h>
#include <RooHist.h>
#include <TBrowser.h>
#include <TRandom.h>
#include <TApplication.h>
#include <TFile.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TPaveStats.h>
#include <TLegend.h>
#include <TPaveText.h>
#include <TGaxis.h>

#include "RooWorkspace.h"

#include "model/Constants.h"
#include "model/ParametersPool.h"
#include "roofit/pdfs/TwoGaussian.h"
#include "roofit/pdfs/ExpPdf.h"
#include "roofit/pdfs/TrapPdf.h"
#include "roofit/pdfs/GrainPdf.h"
#include "roofit/pdfs/ThreeGaussian.h"

#include "util/GraphicsHelper.h"
#include "util/StringUtils.h"
#include "util/HistProcessor.h"
#include "roofit/AdditiveConvolutionPdf.h"
#include "roofit/ModelCommonizer.h"
#include "util/ObjectNamer.h"
#include "util/MathUtil.h"
#include "util/FileUtils.h"
#include "util/RootHelper.h"
#include "util/Debug.h"

#include <iostream>
#include <sstream>

// Declare Map Container to perform simultaneous fit
typedef std::map<std::string, RooDataHist*> dhistMap;
typedef std::pair<std::string, RooDataHist*> dhistPair;

struct Spectrum {
	TString filename;
	TH1F* histogram;              // ROOT histogram
	RooDataHist* dataHistogram;   // RooFit histogram
	Double_t integral;            // total counts
	Int_t numberOfBins;
	Int_t binWithMinimumCount;    // bin number with minimum value in the range
	Int_t binWithMaximumCount;    // bin number with maximum value in the range
	Double_t minimumCount;        // minimum count across all bins
	Double_t maximumCount;        // maximum count across all bins
	Double_t averageBackground;
	RooAbsPdf* model;
	RooAbsPdf* resolutionFunction;
};

// ASCII font generator is here: http://patorjk.com/software/taag/#p=display&f=Graffiti&t=Resolution%0AFunction
int run(int argc, char* argv[], Bool_t isRoot = kFALSE) {
	// Start timer to track performance
	RootHelper::startTimer();

	// Create constants object that stores values from "constants.txt" file
	Constants* constants = Constants::getInstance();

	// Define some global app constants
	const Int_t BINS = constants->getMaxChannel() - constants->getMinChannel() + 1;
	const Bool_t DO_RANGE = constants->getExcludeMinChannel() > 1 && constants->getExcludeMinChannel() < constants->getExcludeMaxChannel() && constants->getExcludeMaxChannel() < BINS ? kTRUE : kFALSE;

	// Define Channels Observable
	RooRealVar* channels = new RooRealVar("channels", "Channels axis", 0, BINS, "ch");

	// Set convolution bins same as real number of axis bins (notices it works nice)
	channels->setBins(BINS);
	channels->setBins(BINS, "cache");

	// Scan current directory for '.Spe' files; exit if nothing found
	const std::vector<std::string> filenames = FileUtils::getFilenamesInCurrentDrectory(".Spe");
	if (filenames.size() < 1) {
		std::cout << "No Maestro '.Spe' files found in current directory. Press CTRL+C." << std::endl;
		return 0;
	}
	Debug("[main] Found " << filenames.size() << " spectra.");

	// Construct a list of Spectrum structs to store individual spectra information
	std::vector<Spectrum> spectra;
	for (unsigned i = 0; i < filenames.size(); i++) {
		Spectrum s;
		s.filename = TString(filenames[i].c_str());
		s.histogram = FileUtils::importTH1F(filenames[i], i);
		s.dataHistogram = new RooDataHist(Form("dataHistogram_%d", i + 1), s.filename.Data(), RooArgSet(*channels), RooFit::Import(*(s.histogram)));
		s.numberOfBins = s.histogram->GetXaxis()->GetNbins();
		s.integral = s.histogram->Integral();
		s.binWithMinimumCount = s.histogram->GetMinimumBin();
		s.binWithMaximumCount = s.histogram->GetMaximumBin();
		s.minimumCount = s.histogram->GetBinContent(s.binWithMinimumCount);
		s.maximumCount = s.histogram->GetBinContent(s.binWithMaximumCount);
		s.averageBackground = HistProcessor::getAverageBackground(s.histogram);
		spectra.push_back(s);
		Debug("Spectrum " << i+1 << " file is \"" << s.filename << "\"");
		Debug("  bins: " << s.numberOfBins);
		Debug("  integral: " << s.integral);
		Debug("  bin with minimum count: " << s.binWithMinimumCount);
		Debug("  bin with maximum count: " << s.binWithMaximumCount);
		Debug("  minimum count: " << s.minimumCount);
		Debug("  maximum count: " << s.maximumCount);
		Debug("  average background: " << s.averageBackground);
	}

	// Construct additive decay models
	RooWorkspace* w = new RooWorkspace("w", "w");
	for (unsigned i = 0; i < spectra.size(); i++) {
		AdditiveConvolutionPdf* acp = new AdditiveConvolutionPdf(constants->getDecayModels(), constants->getResolutionFunctionModel(), constants->getSourceComponentsNumber(), channels);
		RooAbsPdf* pdf = acp->getPdf();

		// Set mean gauss values
		if (RooRealVar* gaussMean = (RooRealVar*) (pdf->getParameters(*channels))->find("mean_gauss")) {
			gaussMean->setConstant(kFALSE);
			gaussMean->setMin(spectra[i].binWithMaximumCount - 100);
			gaussMean->setMax(spectra[i].binWithMaximumCount + 100);
			gaussMean->setVal(spectra[i].binWithMaximumCount);
		}

		// Initialize background here
		RooPolynomial* bg = new RooPolynomial("bg", "y=1", *channels, RooArgSet());

		// Parameterize background as counts, not as fraction (intentionally RooRealVar, not RooConst)
		RooRealVar* bgCount = new RooRealVar("background", "Background level counts", spectra[i].averageBackground, spectra[i].averageBackground / 2, spectra[i].averageBackground * 2, "counts");
		bgCount->setConstant(kTRUE);
		Int_t b = spectra[i].numberOfBins;
		RooRealVar* bins = new RooRealVar("bins", "Histogram bins", b, b, b);
		bins->setConstant(kTRUE);
		RooRealVar* fullIntegral = new RooRealVar("integral", "Full histogram integral", spectra[i].integral, spectra[i].integral, spectra[i].integral);
		fullIntegral->setConstant(kTRUE);
		RooFormulaVar* IBg = new RooFormulaVar("IBg", "@0*@1/@2", RooArgList(*bgCount, *bins, *fullIntegral));

		// Final PDFs can have the same name, need to provide resolution protocol
		RooAddPdf* pdfWithBg = new RooAddPdf("pdf", "Final model", RooArgList(*bg, *pdf), *IBg);

		// Rename PDF parameters via RooWorkspace
		if (i == 0) {
			w->import(*pdfWithBg);
			spectra[i].model = w->pdf("pdf");
		} else {
			std::string suffix = std::to_string(i + 1);
			w->import(*pdfWithBg, RooFit::RenameAllVariablesExcept(suffix.c_str(), channels->GetName()), RooFit::RenameAllNodes(suffix.c_str()));
			spectra[i].model = w->pdf(Form("pdf_%d", i + 1));
		}

		// Save resolution function
		spectra[i].resolutionFunction = acp->getResolutionFunction();
	}

#ifdef USEDEBUG
	w->Print();
#endif

	// Introduce common parameters if more than one spectrum (simultaneous fit).
	if (spectra.size() > 1) {
		// Initialize commonizer, read parameters from the first spectrum
		ModelCommonizer* commonizer = new ModelCommonizer(spectra[0].model, channels, constants->getCommonParameters());
		// All other spectra

		for (unsigned i = 1; i < spectra.size(); i++) {
			RooAbsPdf* newPdf = commonizer->replaceParametersWithCommon(spectra[i].model);
			spectra[i].model = newPdf;
		}
	}

	// Output
	Debug("[main] Constructed following models:");
	for (unsigned i = 0; i < spectra.size(); i++) {
#ifdef USEDEBUG
		spectra[i].model->Print();
	}
#endif

	// Create output folder out of model components' names and resolution function name, e.g. "exp-exp-2gauss"
	std::string outputPath = StringUtils::joinStrings(constants->getDecayModels()) + "-" + constants->getResolutionFunctionModel();
	FileUtils::createDirectory(outputPath);

	// Obtain and store number of free parameters for every model (why?)
	RooArgSet** floatPars = new RooArgSet*[spectra.size()];
	RooAbsCollection** floatPars1 = new RooAbsCollection*[spectra.size()];
	Int_t* np = new Int_t[spectra.size()];

	for (unsigned i = 0; i < spectra.size(); i++) {
		floatPars[i] = spectra[i].model->getParameters(spectra[i].dataHistogram);
		floatPars1[i] = floatPars[i]->selectByAttrib("Constant", kFALSE);
		np[i] = floatPars1[i]->getSize();
	}

	/*
	 ___________.__  __    __  .__
	 \_   _____/|__|/  |__/  |_|__| ____    ____
	 |    __)  |  \   __\   __\  |/    \  / ___\
	 |     \   |  ||  |  |  | |  |   |  \/ /_/  >
	 \___  /   |__||__|  |__| |__|___|  /\___  /
	 \/                           \//_____/

	 */

	// Make array of category names
	TString* types = new TString[spectra.size()];
	for (unsigned i = 0; i < spectra.size(); i++)
		types[i] = Form("spectrum_%d", i);

	// Define category and map with spectra
	RooCategory* category = new RooCategory("category", "sample");
	std::map<std::string, RooDataHist*> histogramsMap;
	for (unsigned i = 0; i < spectra.size(); i++) {
		std::string type = types[i].Data();
		category->defineType(type.c_str());
		histogramsMap.insert(dhistPair(type, spectra[i].dataHistogram));
	}

	// Construct combined dataset
	RooDataHist* combinedData = new RooDataHist("combData", "combined data", *channels, *category, histogramsMap);

	// Construct combined model and add
	RooSimultaneous* simPdf = new RooSimultaneous("simPdf", "Simultaneous PDF", *category);
	for (unsigned i = 0; i < spectra.size(); i++) {
		simPdf->addPdf(*spectra[i].model, types[i].Data());
	}

	// Read models' parameters from the pool file.
	// User input parameter values from keyboard if not found in the pool
	ParametersPool* storage = new ParametersPool(outputPath);
	storage->updateModelParametersValuesFromPool(simPdf->getParameters(*channels));

	// Save storage before fitting to create file with parameters
	// in case user doesn't want to wait till fitting ends
	storage->save(simPdf->getParameters(*channels));

	// Use RooMinuit interface to minimize chi^2

	RooChi2Var* simChi2;
	if (DO_RANGE) {
		channels->setRange("LEFT", 1, constants->getExcludeMinChannel());
		channels->setRange("RIGHT", constants->getExcludeMaxChannel(), BINS);
		Int_t numCpu = RootHelper::getNumCpu();
		simChi2 = new RooChi2Var("simChi2", "chi2", *simPdf, *combinedData, RooFit::Range("LEFT,RIGHT"), RooFit::NumCPU(numCpu));
	} else {
		Int_t numCpu = RootHelper::getNumCpu();
		simChi2 = new RooChi2Var("simChi2", "chi2", *simPdf, *combinedData, RooFit::NumCPU(numCpu));
	}

	// RooMinimizer:
	RooMinimizer* m = new RooMinimizer(*simChi2);
	// m->setStrategy(); // RooMinimizer::Speed (default), RooMinimizer::Balance, RooMinimizer::Robustness
	m->setMinimizerType("Minuit");
	Int_t resultMigrad = m->migrad();
	Int_t resultHesse = m->hesse();
	Debug("[main] Fitting completed: migrad=" << resultMigrad << ", hesse=" << resultHesse);

	// Save parameters to file
	storage->save(simPdf->getParameters(*channels));

	/*
	 ________                    .__    .__
	 /  _____/___________  ______ |  |__ |__| ____   ______
	 /   \  __\_  __ \__  \ \____ \|  |  \|  |/ ___\ /  ___/
	 \    \_\  \  | \// __ \|  |_> >   Y  \  \  \___ \___ \
	 \______  /__|  (____  /   __/|___|  /__|\___  >____  >
	 \/           \/|__|        \/        \/     \/

	 */

	// Create canvases and pads first
	// Otherwise program segfaults when drawing stuff on a RooPlot (when calling AddLine() in TPaveText instance)
	TCanvas** canvas = new TCanvas*[spectra.size()];
	for (unsigned i = 0; i < spectra.size(); i++) {
		canvas[i] = new TCanvas(Form("canvas-%d", i + 1), Form("Spectrum N%d \"%s\"", i + 1, spectra[i].filename.Data()), constants->getImageWidth(), constants->getImageHeight());
		canvas[i]->SetFillColor(0);
		canvas[i]->Divide(1, 2);

		canvas[i]->cd(1)->SetPad(0, GraphicsHelper::RESIDUALS_PAD_RELATIVE_HEIGHT, 1, 1);
		gPad->SetMargin(0.07, 0.01, 0.08, 0.1);
		gPad->SetLogy();
		gPad->SetGrid();

		canvas[i]->cd(2)->SetPad(0, 0, 1, GraphicsHelper::RESIDUALS_PAD_RELATIVE_HEIGHT);
		gPad->SetMargin(0.07, 0.01, 0.3, 0.05);
		gPad->SetGrid();
	}

	// Draw spectrum plot (top)
	RooPlot** spectraPlot = new RooPlot*[spectra.size()];
	for (unsigned i = 0; i < spectra.size(); i++) {
		spectraPlot[i] = channels->frame();
		spectraPlot[i]->SetName(Form("spectrum_%d", i));
		spectraPlot[i]->SetTitle(Form("Spectrum %d \"%s\"", i + 1, spectra[i].filename.Data()));
		spectraPlot[i]->GetXaxis()->SetRangeUser(0, BINS);

		spectra[i].dataHistogram->plotOn(spectraPlot[i], RooFit::LineStyle(kSolid), RooFit::LineColor(kBlack), RooFit::LineWidth(0), RooFit::MarkerSize(0.2), RooFit::MarkerColor(kBlack),
				RooFit::Name("data"));

		// Draw Resolution Function
		spectra[i].resolutionFunction->plotOn(spectraPlot[i], RooFit::LineStyle(kSolid), RooFit::LineColor(kGray + 1), RooFit::LineWidth(1), RooFit::Name("resolution"));

		// Draw complete fit, dont't forget the ranges if needed
		// https://root-forum.cern.ch/t/excluding-regions-in-a-fit/9109
		spectra[i].model->plotOn(spectraPlot[i], RooFit::LineStyle(kSolid), RooFit::LineColor(kPink - 4), RooFit::LineWidth(2), RooFit::Name("fit"), DO_RANGE ? RooFit::Range("LEFT,RIGHT") : NULL);

		// Add legend with list of model parameters
		RooArgSet* parameters = spectra[i].model->getParameters(spectraPlot[i]->getNormVars());
		TPaveText* pt = GraphicsHelper::makePaveText(*parameters, GraphicsHelper::LEGEND_XMIN, 0.99, 0.9);
		spectraPlot[i]->addObject(pt);

		// Set custom Y axis limits
		Double_t yMin = pow(10, MathUtil::orderOfMagnitude(spectra[i].averageBackground));  // Minimum is order of magnitude of the average background value
		Double_t yMax = spectraPlot[i]->GetMaximum();  // Maximum is default (whatever ROOT plots)
		spectraPlot[i]->GetYaxis()->SetRangeUser(yMin, yMax);

		// Draw grayed out region excluded from plot
		if (DO_RANGE) {
			GraphicsHelper::drawRegion(spectraPlot[i], constants->getExcludeMinChannel(), constants->getExcludeMaxChannel());
		}

		// Draw nanosecond axis
		Double_t scaleFactor = GraphicsHelper::getSpectrumPadFontFactor();
		RooAbsArg* rooAbsArg = spectra[i].model->getParameters(spectraPlot[i]->getNormVars())->find("mean_gauss");
		if (RooRealVar* rooRealVar = dynamic_cast<RooRealVar*>(rooAbsArg)) {
			Double_t zeroChannel = rooRealVar->getVal();
			Double_t channelWidth = Constants::getInstance()->getChannelWidth();
			Double_t timeMin = -channelWidth * zeroChannel;
			Double_t timeMax = (channels->getMax() - zeroChannel) * channelWidth;
			TGaxis *timeAxis = new TGaxis(0, yMin, channels->getMax(), yMin, timeMin, timeMax, 510, "+L");
			timeAxis->SetTitleSize(scaleFactor * GraphicsHelper::FONT_SIZE_NORMAL);
			timeAxis->SetTitleOffset(0.68);
			timeAxis->SetTitleFont(42);
			timeAxis->SetTitle("(ns)");
			timeAxis->SetTitleSize(scaleFactor * GraphicsHelper::FONT_SIZE_NORMAL);
			timeAxis->SetLabelSize(scaleFactor * GraphicsHelper::FONT_SIZE_NORMAL);
			timeAxis->SetLabelOffset(0.02);
			timeAxis->SetLabelFont(42);
			spectraPlot[i]->addObject(timeAxis);
		}

		// Update regular axis
		spectraPlot[i]->GetXaxis()->SetLabelColor(0);
		spectraPlot[i]->GetXaxis()->SetTitleSize(0);
		spectraPlot[i]->GetXaxis()->SetTickLength(0);

		spectraPlot[i]->GetYaxis()->SetLabelSize(scaleFactor * (GraphicsHelper::FONT_SIZE_NORMAL));
		spectraPlot[i]->GetYaxis()->SetTitleSize(scaleFactor * GraphicsHelper::FONT_SIZE_NORMAL);
		spectraPlot[i]->GetYaxis()->SetTitleOffset(0.9);

#ifdef USEDEBUG
		spectraPlot[i]->Print("V");
#endif
	}
	Debug("[main] Spectra plots successfully created.");

	// Draw residuals plot (bottom)
	RooPlot** chiFrame = new RooPlot*[spectra.size()];
	RooHist** hresid = new RooHist*[spectra.size()];
	for (unsigned i = 0; i < spectra.size(); i++) {
		chiFrame[i] = channels->frame(RooFit::Title(" "));
		RooChi2Var* chi2 = new RooChi2Var(Form("#chi^{2}_{%d}", i + 1), "chi-square", *spectra[i].model, *spectra[i].dataHistogram);
		RooAbsCollection* freeParameters = (spectra[i].model->getParameters(*spectra[i].dataHistogram))->selectByAttrib("Constant", kFALSE);
		Int_t degreesFreedom = spectra[i].numberOfBins - freeParameters->getSize();
		Double_t chi2Value = chi2->getVal() / degreesFreedom;

		// If doing ranges we have to manually construct chi frames from the curves
		// https://root-forum.cern.ch/t/pull-histogram-with-multiple-ranges/20935
		if (DO_RANGE) {
			RooHist* dataHist = (RooHist*) spectraPlot[i]->getHist("data");
			auto curve1 = (RooCurve*) spectraPlot[i]->getObject(2);  // 2 is index in the list of RooPlot items (see printout from graphFrame[i]->Print("V")
			auto curve2 = (RooCurve*) spectraPlot[i]->getObject(3);
			auto hresid1 = dataHist->makePullHist(*curve1, true);
			auto hresid2 = dataHist->makePullHist(*curve2, true);
			hresid1->SetLineWidth(0);
			hresid1->SetMarkerSize(0.2);
			hresid2->SetLineWidth(0);
			hresid2->SetMarkerSize(0.2);
			chiFrame[i]->addPlotable(hresid1, "P");
			chiFrame[i]->addPlotable(hresid2);
		} else {
			hresid[i] = spectraPlot[i]->pullHist();
			hresid[i]->SetLineWidth(0);
			hresid[i]->SetMarkerSize(0.2);
			chiFrame[i]->addPlotable(hresid[i]);
		}

		// Draw region excluded from plot
		if (DO_RANGE) {
			GraphicsHelper::drawRegion(chiFrame[i], constants->getExcludeMinChannel(), constants->getExcludeMaxChannel());
		}

		// Add legend with chi^2 value
		Double_t scaleFactor = GraphicsHelper::getResidualsPadFontFactor();
		TPaveText* leg = new TPaveText(GraphicsHelper::LEGEND_XMIN, 0.85, 0.99, 0.95, "BRNDC");  // x1 y1 x2 y2  0.75, 0.99, 0.9
		leg->AddText(Form("#chi^{2} = %.1f / %d = %.3f", chi2->getVal(), degreesFreedom, chi2Value));
		leg->SetTextSize(scaleFactor * GraphicsHelper::FONT_SIZE_SMALL * 1.3);
		leg->SetBorderSize(1);
		leg->SetFillColor(0);
		leg->SetTextAlign(12);
		chiFrame[i]->addObject(leg);

		// Draw horizontal line
		TLine* hr = new TLine(1, 0, channels->getBins(), 0);
		hr->SetLineStyle(7);
		hr->SetLineWidth(2);
		hr->SetLineColor(kPink - 4);
		chiFrame[i]->addObject(hr);

		// Update axis
		chiFrame[i]->GetYaxis()->SetTitle("Fit residuals");
		chiFrame[i]->GetYaxis()->SetTitleOffset(0.48);
		chiFrame[i]->GetYaxis()->SetTitleSize(scaleFactor * (GraphicsHelper::FONT_SIZE_NORMAL));
		chiFrame[i]->GetYaxis()->SetLabelSize(scaleFactor * (GraphicsHelper::FONT_SIZE_NORMAL));
		chiFrame[i]->GetYaxis()->SetLabelOffset(0.01);

		chiFrame[i]->GetXaxis()->SetTitleSize(scaleFactor * (GraphicsHelper::FONT_SIZE_NORMAL));
		chiFrame[i]->GetXaxis()->SetTitleOffset(2);
		chiFrame[i]->GetXaxis()->SetLabelSize(scaleFactor * (GraphicsHelper::FONT_SIZE_NORMAL));
		chiFrame[i]->GetXaxis()->SetLabelOffset(0.04);

#ifdef USEDEBUG
		chiFrame[i]->Print("V");
#endif
	}
	Debug("[main] Residual plots successfully created.");

	// Draw plots on the pads
	for (unsigned i = 0; i < spectra.size(); i++) {
		// Draw plots on correspondent pads
		canvas[i]->cd(1);
		spectraPlot[i]->Draw();
		canvas[i]->cd(2);
		chiFrame[i]->Draw();
		canvas[i]->Modified();
		canvas[i]->Update();

		// Save raster and vector images
		// https://root.cern.ch/doc/master/classTPad.html#a649899aa030f537517022a5d51ec152f
		TString imageFilePathName = Form("./%s-%s/fit-%s-%s-%d", (StringUtils::joinStrings(constants->getDecayModels())).c_str(), constants->getResolutionFunctionModel(),
				(StringUtils::joinStrings(constants->getDecayModels())).c_str(), constants->getResolutionFunctionModel(), i + 1);
		TString pngURI = imageFilePathName + ".png";
		TString epsURI = imageFilePathName + ".eps";
		canvas[i]->Print(pngURI, "png");
		canvas[i]->Print(epsURI, "eps");
	}
	Debug("[main] Canvas images successfully exported.");

	/*
	 ________          __                 __
	 \_____  \  __ ___/  |_______  __ ___/  |_
	 /   |   \|  |  \   __\____ \|  |  \   __\
	/    |    \  |  /|  | |  |_> >  |  /|  |
	 \_______  /____/ |__| |   __/|____/ |__|
	 \/            |__|

	 */

	// TODO: move to FileUtils
	for (unsigned i = 0; i < spectra.size(); i++) {
		std::ofstream outputFile;
		TString dataFilename = Form("./%s-%s/data-%s-%s-%d.txt", (StringUtils::joinStrings(constants->getDecayModels())).c_str(), constants->getResolutionFunctionModel(),
				(StringUtils::joinStrings(constants->getDecayModels())).c_str(), constants->getResolutionFunctionModel(), i + 1);
		outputFile.open(dataFilename.Data());

		const unsigned FILE_COLUMN_WIDTH = 20;
		// Write file header
		outputFile << std::left << std::setw(FILE_COLUMN_WIDTH) << "\"Channel\"" << std::left << std::setw(FILE_COLUMN_WIDTH) << "\"Time, ns\"" << std::left << std::setw(FILE_COLUMN_WIDTH)
				<< "\"Count\"" << std::left << std::setw(FILE_COLUMN_WIDTH) << "\"Error\"" << std::left << std::setw(FILE_COLUMN_WIDTH) << "\"Resolution\"" << std::left << std::setw(FILE_COLUMN_WIDTH)
				<< "\"Fit\"" << std::left << std::setw(FILE_COLUMN_WIDTH) << "\"Chi^2\"" << std::endl;

		const Int_t minChannel = constants->getMinChannel();
		const Int_t maxChannel = constants->getMaxChannel();
		for (unsigned j = minChannel; j <= maxChannel; j++) {
			Double_t time = (double) (j - minChannel) * (constants->getChannelWidth());
			Double_t count = spectra[i].histogram->GetBinContent(j - minChannel + 1);
			Double_t error = spectra[i].histogram->GetBinError(j - minChannel + 1);
			Double_t resolution = spectraPlot[i]->getCurve("resolution")->Eval(j - minChannel + 0.5);
			Double_t fit = spectraPlot[i]->getCurve("fit")->Eval(j - minChannel + 0.5);
			Double_t channel, chi;
			if (!DO_RANGE) {
				hresid[i]->GetPoint(j - minChannel + 1, channel, chi);
			} else {
//                TODO: add code for ranges (two curves)
			}
			outputFile << std::left << std::setw(FILE_COLUMN_WIDTH) << j << std::left << std::setw(FILE_COLUMN_WIDTH) << time << std::left << std::setw(FILE_COLUMN_WIDTH) << count << std::left
					<< std::setw(FILE_COLUMN_WIDTH) << error << std::left << std::setw(FILE_COLUMN_WIDTH) << resolution << std::left << std::setw(FILE_COLUMN_WIDTH) << fit << std::left
					<< std::setw(FILE_COLUMN_WIDTH) << chi;
			outputFile << std::endl;
		}
		outputFile.close();
	}
	RootHelper::stopAndPrintTimer();
	return 1;
}

int main(int argc, char* argv[]) {
	// Create ROOT application
	// https://github.com/root-project/root/blob/master/tutorials/gui/mditest.C#L409
	TApplication* app = new TApplication("positronFit", &argc, argv);

	run(argc, argv);

	app->Run();
	return 0;
}
