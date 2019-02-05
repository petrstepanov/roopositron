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

//#include "temp/MyPdfCache.h"
//#include "temp/MyPdf.h"
//#include "temp/ConvPdf.h"
//#include "temp/ChannelConvolutionPdf.h"
//#include <unistd.h>

#include <iostream>
#include <sstream>

//using namespace RooFit;
//using namespace TMath;

// Declare Map Container to perform simultaneous fit
typedef std::map<std::string, RooDataHist*> dhistMap;
typedef std::pair<std::string, RooDataHist*> dhistPair;

// TODO: make FileUtils output spectrums
struct Spectrum {
	TH1F* histogram;              // ROOT histogram
	TString filename;
	Double_t integral;            // total counts
	Int_t numberOfBins;
	Int_t binWithMinimumCount;    // bin number with minimum value in the range
	Int_t binWithMaximumCount;    // bin number with maximum value in the range
	Double_t minimumCount;        // minimum count across all bins
	Double_t maximumCount;        // maximum count across all bins
	Double_t averageBackground;
};

// ASCII font generator is here: http://patorjk.com/software/taag/#p=display&f=Graffiti&t=Resolution%0AFunction
int run(int argc, char* argv[], Bool_t isRoot = kFALSE) {
	// Start timer to track performance
	RootHelper::startTimer();

	// Construct a list of .Spe files from current directory
	std::vector<std::string> filenames = FileUtils::getFilenamesInCurrentDrectory(".Spe");

	// Construct a list of Spectrum structs to store individual spectra information
	std::vector<Spectrum> spectra;
	for (unsigned i = 0; i < filenames.size(); i++) {
		Spectrum s;
		s.histogram = FileUtils::importTH1F(filenames[i], i);
		s.filename = filenames[i].c_str();
		s.numberOfBins = s.histogram->GetXaxis()->GetNbins();
		s.integral = s.histogram->Integral();
		s.binWithMinimumCount = s.histogram->GetMinimumBin();
		s.binWithMinimumCount = s.histogram->GetMaximumBin();
		s.minimumCount = s.histogram->GetBinContent(s.binWithMinimumCount);
		s.maximumCount = s.histogram->GetBinContent(s.binWithMaximumCount);
		s.averageBackground = HistProcessor::getAverageBackground(s.histogram);
		spectra.push_back(s);
	}

	// Create constants object that stores values from "constants.txt" file
	Constants* constants = Constants::getInstance();

	// Define some global app constants
	const Int_t BINS = constants->getMaxChannel() - constants->getMinChannel() + 1;
	const Bool_t doRange = constants->getExcludeMinChannel() > 1 && constants->getExcludeMinChannel() < constants->getExcludeMaxChannel() && constants->getExcludeMaxChannel() < BINS ? kTRUE : kFALSE;

	// Define number of spectrums
	const int iNumberOfFiles = filenames.size();
	std::cout << "Found " << iNumberOfFiles << " files.\n" << std::endl;

	// Exit if no spectra found
	if (iNumberOfFiles < 1) {
		std::cout << "No Maestro `.Spe` files found in current directory. Press CTRL+C." << std::endl;
		return 0;
	}

	// Convert list to array of strings
	const char** array = new const char*[filenames.size()];
	unsigned index = 0;
	std::string* sFileNames = new std::string[filenames.size()];
	for (std::vector<std::string>::iterator it = filenames.begin(); it != filenames.end(); ++it) {
		sFileNames[index] = ((std::string) *it).c_str();
		index++;
	}

	TH1F** fullTH1F = new TH1F*[iNumberOfFiles];

	// Channels (bins) with maximum and minimum count
	Int_t* iTopChannel = new Int_t[iNumberOfFiles];
	Int_t* iLowChannel = new Int_t[iNumberOfFiles];

//	// Y axis plotting range
	Int_t* iUpperLimit = new Int_t[iNumberOfFiles];
	Int_t* iLowerLimit = new Int_t[iNumberOfFiles];

	// TODO: drop these variables in favor of histogram struct

//        const Int_t CHANNELS = constants->getNumberOfChannels();
	const Double_t CHANNEL_WIDTH = constants->getChannelWidth();
	const Int_t SKIP_LINES = constants->getSkipLines();

	for (unsigned i = 0; i < iNumberOfFiles; i++) {
		// Import Data
		fullTH1F[i] = new TH1F(TString::Format("fullTH1F_%d", i + 1), TString::Format("fullTH1F_%d", i + 1), BINS, 0, BINS);
		// Open File
		std::cout << std::endl << "Spectrum " << i + 1 << " filename is \"" << sFileNames[i] << "\"" << std::endl;
		std::string path = sFileNames[i];
		fullTH1F[i] = FileUtils::importTH1F(path, i);

		iTopChannel[i] = fullTH1F[i]->GetMaximumBin();
		iLowChannel[i] = fullTH1F[i]->GetMinimumBin();
		std::cout << "  Top Channel is " << iTopChannel[i] << std::endl;
		std::cout << "  Low Channel is " << iLowChannel[i] << std::endl;

		Int_t iMaxCount = fullTH1F[i]->GetBinContent(iTopChannel[i]);
		Int_t iMinCount = fullTH1F[i]->GetBinContent(iLowChannel[i]);
		std::cout << "  Maximum count is: " << iMaxCount << std::endl;
		std::cout << "  Minimum count is: " << iMinCount << std::endl;

		std::cout << "  Total number of events:" << fullTH1F[i]->Integral() << std::endl;

		// Evaluate counts axis limits (for graphical output)
		iUpperLimit[i] = 2 * iMaxCount;
		iLowerLimit[i] = iMinCount / 2;
	}

	// Define Channels Observable
	RooRealVar* rChannels = new RooRealVar("rChannels", "Channels axis", 0, BINS, "ch");

	// Set convolution bins same as real number of axis bins (notices it works nice)
	rChannels->setBins(BINS);
	rChannels->setBins(BINS, "cache");

	// Convert TH1F spectra to RooDataHist
	RooDataHist** histSpectrum = new RooDataHist*[iNumberOfFiles];
	for (unsigned i = 0; i < iNumberOfFiles; i++) {
		histSpectrum[i] = new RooDataHist(TString::Format("histSpectrum_%d", i + 1), sFileNames[i].c_str(), RooArgSet(*rChannels), RooFit::Import(*fullTH1F[i]));
	}

	// Construct additive decay models
	RooAbsPdf** decay_model = new RooAbsPdf*[iNumberOfFiles];
	RooAbsPdf** res_funct = new RooAbsPdf*[iNumberOfFiles];

	RooWorkspace* w = new RooWorkspace("w", "w");
	for (unsigned i = 0; i < iNumberOfFiles; i++) {
		AdditiveConvolutionPdf* acp = new AdditiveConvolutionPdf(constants->getDecayModels(), constants->getResolutionFunctionModel(), constants->getSourceComponentsNumber(), rChannels);
		RooAbsPdf* pdf = acp->getPdf();

		// Set mean gauss values
		RooRealVar* gaussMean = (RooRealVar*) (pdf->getParameters(*rChannels))->find("mean_gauss");
		if (gaussMean) {
			gaussMean->setMin(iTopChannel[i] - 100);
			gaussMean->setMax(iTopChannel[i] + 100);
			gaussMean->setVal(iTopChannel[i]);
		}

		// Initialize background here
		RooPolynomial* bg = new RooPolynomial("bg", "y=1", *rChannels, RooArgSet());
		// Parameterize background as counts, not as fraction (intentionally RooRealVar, not RooConst)
		RooRealVar* bgCount = new RooRealVar("background", "Background level counts", spectra[i].averageBackground, spectra[i].averageBackground / 2, spectra[i].averageBackground * 2, "counts");
		bgCount->setConstant(kTRUE);
		Int_t b = fullTH1F[i]->GetXaxis()->GetNbins();
		RooRealVar* bins = new RooRealVar("bins", "Histogram bins", b, b - 1, b + 1);
		bins->setConstant(kTRUE);
		RooRealVar* fullIntegral = new RooRealVar("integral", "Full histogram integral", fullTH1F[i]->Integral(1, b), fullTH1F[i]->Integral(1, b) - 1, fullTH1F[i]->Integral(1, b) + 1);
		fullIntegral->setConstant(kTRUE);
		RooFormulaVar* IBg = new RooFormulaVar("IBg", "@0*@1/@2", RooArgList(*bgCount, *bins, *fullIntegral));

		// Final PDFs can have the same name, need to provide resolution protocol
		RooAddPdf* pdfWithBg = new RooAddPdf("pdf", "Final model", RooArgList(*bg, *pdf), *IBg);

		// Rename PDF parameters via RooWorkspace
		if (i == 0) {
			w->import(*pdfWithBg);
			decay_model[i] = w->pdf("pdf");
		} else {
			std::string suffix = std::to_string(i + 1);
			w->import(*pdfWithBg, RooFit::RenameAllVariablesExcept(suffix.c_str(), rChannels->GetName()), RooFit::RenameAllNodes(suffix.c_str()));
			decay_model[i] = w->pdf(Form("pdf_%d", i + 1));
		}
//	    components[i] = acp->getAllComponents();
//	    convolutedComponents[i] = acp->getConvolutedComponents();
//	    decay_model[i] = pdfWithBg;
		res_funct[i] = acp->getResolutionFunction();
//	    source_component[i] = acp->getSourceCompoment();
//	    source_component_convoluted[i] = acp->getConvolutedSourceComponent();
		// from workspace

		// Run namer, add "_#" to every parameter and model name
//	    simultaneousNamer->fixUniquePdfAndParameterNames(decay_model[i], rChannels);
//	    ObjectNamer::suffixAllModelParameters(decay_model[i], rChannels, i+1);
	}

	w->Print();

	// Introduce common parameters if more than one spectrum (simultaneous fit).
	if (iNumberOfFiles > 1) {
		// Initialize commonizer, read parameters from the first spectrum
		ModelCommonizer* commonizer = new ModelCommonizer(decay_model[0], rChannels, constants->getCommonParameters());
		// All other spectra

		for (unsigned i = 1; i < iNumberOfFiles; i++) {
			RooAbsPdf* newPdf = commonizer->replaceParametersWithCommon(decay_model[i]);
			decay_model[i] = newPdf;
		}
	}

	// Output
	std::cout << std::endl << "Constructed following models" << std::endl;
	for (unsigned i = 0; i < iNumberOfFiles; i++) {
//	    std::cout << std::endl << "Model " << i+1 << ": " << decay_model[i]->GetName() << std::endl;
		RootHelper::printPdfCoefficientNames(decay_model[i], rChannels);
//	    decay_model[i]->Print();
	}

	// Create output folder out of model components' names and resolution function name, e.g. "exp-exp-2gauss"
	std::string outputPath = StringUtils::joinStrings(constants->getDecayModels()) + "-" + constants->getResolutionFunctionModel();
	FileUtils::createDirectory(outputPath);

	// Read models' parameters from the pool.
	// User input parameter values from keyboard if not found in the pool
//	ParametersPool* storage = new ParametersPool(outputPath);
//	for (unsigned i = 0; i < iNumberOfFiles; i++) {
//		storage->updateModelParametersValuesFromPool(decay_model[i]->getParameters(*rChannels));
//	}

	// Obtain and store number of free parameters for every model (why?)
	RooArgSet** floatPars = new RooArgSet*[iNumberOfFiles];
	RooAbsCollection** floatPars1 = new RooAbsCollection*[iNumberOfFiles];
	Int_t* np = new Int_t[iNumberOfFiles];

	for (unsigned i = 0; i < iNumberOfFiles; i++) {
		floatPars[i] = decay_model[i]->getParameters(histSpectrum[i]);
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
	TString* types = new TString[iNumberOfFiles];
	for (unsigned i = 0; i < iNumberOfFiles; i++)
		types[i] = TString::Format("spectrum_%d", i);

	// Define category and map with spectra
	RooCategory* category = new RooCategory("category", "sample");
	std::map<std::string, RooDataHist*> histogramsMap;
	for (unsigned i = 0; i < iNumberOfFiles; i++) {
		std::string type = types[i].Data();
		category->defineType(type.c_str());
		histogramsMap.insert(dhistPair(type, histSpectrum[i]));
	}

	// Construct combined dataset
	RooDataHist* combinedData = new RooDataHist("combData", "combined data", *rChannels, *category, histogramsMap);

	// Construct combined model and add
	RooSimultaneous* simPdf = new RooSimultaneous("simPdf", "Simultaneous PDF", *category);
	for (unsigned i = 0; i < iNumberOfFiles; i++) {
		simPdf->addPdf(*decay_model[i], types[i].Data());
	}

	// Read models' parameters from the pool file.
	// User input parameter values from keyboard if not found in the pool
	ParametersPool* storage = new ParametersPool(outputPath);
	storage->updateModelParametersValuesFromPool(simPdf->getParameters(*rChannels));

	// Save storage before fitting to create file with parameters
	// in case user doesn't want to wait till fitting ends
	storage->save(simPdf->getParameters(*rChannels));

//	RootHelper::printPdfCoefficientNames(simPdf, rChannels);
	// simPdf->fitTo(*combData) ;
	// simPdf->fitTo(*combData, NumCPU(NUM_CPU));

	// Use RooMinuit interface to minimize chi^2

	RooChi2Var* simChi2;
	if (doRange) {
		std::cout << "Doing Ranges!" << std::endl;
		rChannels->setRange("LEFT", 1, constants->getExcludeMinChannel());
		rChannels->setRange("RIGHT", constants->getExcludeMaxChannel(), BINS);
		Int_t numCpu = RootHelper::getNumCpu();
		simChi2 = new RooChi2Var("simChi2", "chi2", *simPdf, *combinedData, RooFit::Range("LEFT,RIGHT"), RooFit::NumCPU(numCpu));

		// Define ranges for every spectrum, then use SplitRange... (not worked)
		// https://root-forum.cern.ch/t/trying-a-simultaneous-fit-in-roofit/3168
//            for (unsigned i = 0; i < iNumberOfFiles; i++){
//                TString rangeName = "LEFT";
//                rangeName += '_';
//                rangeName += types[i];
//                rChannels->setRange(rangeName.Data(),1, EXCLUDE_minChannel) ;
//
//                rangeName = "RIGHT";
//                rangeName += '_';
//                rangeName += types[i];
//                rChannels->setRange(rangeName.Data(),EXCLUDE_maxChannel, rChannels->getBins()) ;
//            }
//             simChi2 = new RooChi2Var("simChi2", "chi2", *simPdf, *combinedData, RooFit::Range("LEFT,RIGHT"), RooFit::SplitRange(kTRUE));

		// Ranges only work with fitTo (not combined ranges)!
		// TODO: Try SplitRange() https://root-forum.cern.ch/t/trying-a-simultaneous-fit-in-roofit/3168
//            simPdf->fitTo(*combinedData, RooFit::Range("LEFT,RIGHT"), RooFit::SplitRange(kTRUE)) ;
//            simPdf->fitTo(*combinedData, RooFit::Range("LEFT,RIGHT")) ;
//            RooFitResult* r_sb12 = simPdf->fitTo(*combinedData,Range("LEFT,RIGHT",kFALSE),Save()) ;

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
	Debug("[main] migrad=" << resultMigrad << ", hesse=" << resultHesse);

	RooArgSet* simFloatPars = simPdf->getParameters(*combinedData);
	RooAbsCollection* simFloatPars1 = simFloatPars->selectByAttrib("Constant", kFALSE);
	Int_t simNp = simFloatPars1->getSize();

	Debug("[main] Fitting completed.");

	// Save parameters to file
	storage->save(simPdf->getParameters(*rChannels));

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
	TCanvas** canvas = new TCanvas*[iNumberOfFiles];
	for (unsigned i = 0; i < iNumberOfFiles; i++) {
		canvas[i] = new TCanvas(Form("canvas-%d", i + 1), Form("Spectrum N%d \"%s\"", i + 1, sFileNames[i].c_str()), constants->getImageWidth(), constants->getImageHeight());
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
	RooPlot** graphFrame = new RooPlot*[iNumberOfFiles];
	for (unsigned i = 0; i < iNumberOfFiles; i++) {
		graphFrame[i] = rChannels->frame();
		graphFrame[i]->SetName(Form("spectrum_%d", i));
		graphFrame[i]->SetTitle(Form("Spectrum %d \"%s\"", i + 1, sFileNames[i].c_str()));
		graphFrame[i]->GetXaxis()->SetRangeUser(0, BINS);

		histSpectrum[i]->plotOn(graphFrame[i], RooFit::LineStyle(kSolid), RooFit::LineColor(kBlack), RooFit::LineWidth(0), RooFit::MarkerSize(0.2), RooFit::MarkerColor(kBlack), RooFit::Name("data"));

		// Draw Resolution Function
		res_funct[i]->plotOn(graphFrame[i], RooFit::LineStyle(kSolid), RooFit::LineColor(kGray + 1), RooFit::LineWidth(1), RooFit::Name("resolution"));

		// Draw complete fit, dont't forget the ranges if needed
		// https://root-forum.cern.ch/t/excluding-regions-in-a-fit/9109
		decay_model[i]->plotOn(graphFrame[i], RooFit::LineStyle(kSolid), RooFit::LineColor(kPink - 4), RooFit::LineWidth(2), RooFit::Name("fit"), doRange ? RooFit::Range("LEFT,RIGHT") : NULL);

		// Add legend with list of model parameters
		RooArgSet* parameters = decay_model[i]->getParameters(graphFrame[i]->getNormVars());
		TPaveText* pt = GraphicsHelper::makePaveText(*parameters, 3, 0.75, 0.99, 0.9);
		graphFrame[i]->addObject(pt);

		// Set custom Y axis limits
		Double_t yMin = pow(10, MathUtil::orderOfMagnitude(spectra[i].averageBackground));  // Minimum is order of magnitude of the average background value
		Double_t yMax = graphFrame[i]->GetMaximum();  // Maximum is default (whatever ROOT plots)
		Debug(yMin << " " << yMax);
		graphFrame[i]->GetYaxis()->SetRangeUser(yMin, yMax);

		// Draw grayed out region excluded from plot
		if (doRange) {
			GraphicsHelper::drawRegion(graphFrame[i], constants->getExcludeMinChannel(), constants->getExcludeMaxChannel());
		}

		// Draw nanosecond axis
		Double_t scaleFactor = GraphicsHelper::getSpectrumPadFontFactor();
		RooAbsArg* rooAbsArg = decay_model[i]->getParameters(graphFrame[i]->getNormVars())->find("mean_gauss");
		if (RooRealVar* rooRealVar = dynamic_cast<RooRealVar*>(rooAbsArg)) {
			Double_t zeroChannel = rooRealVar->getVal();
			Double_t channelWidth = Constants::getInstance()->getChannelWidth();
			Double_t timeMin = -channelWidth * zeroChannel;
			Double_t timeMax = (rChannels->getMax() - zeroChannel) * channelWidth;
			TGaxis *timeAxis = new TGaxis(0, yMin, rChannels->getMax(), yMin, timeMin, timeMax, 510, "+L");
			timeAxis->SetTitleSize(scaleFactor * GraphicsHelper::FONT_SIZE_NORMAL);
			timeAxis->SetTitleOffset(0.68);
			timeAxis->SetTitleFont(42);
			timeAxis->SetTitle("(ns)");
			timeAxis->SetTitleSize(scaleFactor * GraphicsHelper::FONT_SIZE_NORMAL);
			timeAxis->SetLabelSize(scaleFactor * GraphicsHelper::FONT_SIZE_NORMAL);
			timeAxis->SetLabelOffset(0.02);
			timeAxis->SetLabelFont(42);
			graphFrame[i]->addObject(timeAxis);
		}

		// Update regular axis
		graphFrame[i]->GetXaxis()->SetLabelColor(0);
		graphFrame[i]->GetXaxis()->SetTitleSize(0);
		graphFrame[i]->GetXaxis()->SetTickLength(0);

		graphFrame[i]->GetYaxis()->SetLabelSize(scaleFactor * (GraphicsHelper::FONT_SIZE_NORMAL));
		graphFrame[i]->GetYaxis()->SetTitleSize(scaleFactor * GraphicsHelper::FONT_SIZE_NORMAL);
		graphFrame[i]->GetYaxis()->SetTitleOffset(0.9);

#ifdef USEDEBUG
		graphFrame[i]->Print("V");
#endif
	}
	Debug("[main] Spectra plots successfully created.");

	// Draw residuals plot (bottom)
	RooPlot** chiFrame = new RooPlot*[iNumberOfFiles];
	RooHist** hresid = new RooHist*[iNumberOfFiles];
	for (unsigned i = 0; i < iNumberOfFiles; i++) {
		chiFrame[i] = rChannels->frame(RooFit::Title(" "));
		RooChi2Var* chi2 = new RooChi2Var(Form("#chi^{2}_{%d}", i + 1), "chi-square", *decay_model[i], *histSpectrum[i]);
		RooAbsCollection* freeParameters = (decay_model[i]->getParameters(*histSpectrum[i]))->selectByAttrib("Constant", kFALSE);
		Int_t degreesFreedom = spectra[i].numberOfBins - freeParameters->getSize();
		Double_t chi2Value = chi2->getVal() / degreesFreedom;

		// If doing ranges we have to manually construct chi frames from the curves
		// https://root-forum.cern.ch/t/pull-histogram-with-multiple-ranges/20935
		if (doRange) {
			RooHist* dataHist = (RooHist*) graphFrame[i]->getHist("data");
			auto curve1 = (RooCurve*) graphFrame[i]->getObject(2);  // 2 is index in the list of RooPlot items (see printout from graphFrame[i]->Print("V")
			auto curve2 = (RooCurve*) graphFrame[i]->getObject(3);
			auto hresid1 = dataHist->makePullHist(*curve1, true);
			auto hresid2 = dataHist->makePullHist(*curve2, true);
			hresid1->SetLineWidth(0);
			hresid1->SetMarkerSize(0.2);
			hresid2->SetLineWidth(0);
			hresid2->SetMarkerSize(0.2);
			chiFrame[i]->addPlotable(hresid1, "P");
			chiFrame[i]->addPlotable(hresid2);
		} else {
			hresid[i] = graphFrame[i]->pullHist();
			hresid[i]->SetLineWidth(0);
			hresid[i]->SetMarkerSize(0.2);
			chiFrame[i]->addPlotable(hresid[i]);
		}

		// Draw region excluded from plot
		if (doRange) {
			GraphicsHelper::drawRegion(chiFrame[i], constants->getExcludeMinChannel(), constants->getExcludeMaxChannel());
		}

		// Add legend with chi^2 value
		Double_t scaleFactor = GraphicsHelper::getResidualsPadFontFactor();
		TPaveText* leg = new TPaveText(0.75, 0.85, 0.99, 0.95, "BRNDC");  // x1 y1 x2 y2  0.75, 0.99, 0.9
		leg->AddText(Form("#chi^{2} = %.1f / %d = %.3f", chi2->getVal(), degreesFreedom, chi2Value));
		leg->SetTextSize(scaleFactor * GraphicsHelper::FONT_SIZE_SMALL * 1.3);
		leg->SetBorderSize(1);
		leg->SetFillColor(0);
		leg->SetTextAlign(12);
		chiFrame[i]->addObject(leg);

		// Draw horizontal line
		TLine* hr = new TLine(1, 0, rChannels->getBins(), 0);
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

		// Write RooDataHist statistics on chi frame (to create TPaveStats object)
		histSpectrum[i]->statOn(chiFrame[i]);

#ifdef USEDEBUG
		chiFrame[i]->Print("V");
#endif
	}
	Debug("[main] Residual plots successfully created.");

	// Draw plots on the pads
	for (unsigned i = 0; i < iNumberOfFiles; i++) {
		// Draw plots on correspondent pads
		canvas[i]->cd(1);
		graphFrame[i]->Draw();
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
	for (unsigned i = 0; i < iNumberOfFiles; i++) {
		std::ofstream outputFile;
		TString dataFilename = TString::Format("./%s-%s/data-%s-%s-%d.txt", (StringUtils::joinStrings(constants->getDecayModels())).c_str(), constants->getResolutionFunctionModel(),
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
			Double_t count = fullTH1F[i]->GetBinContent(j - minChannel + 1);
			Double_t error = fullTH1F[i]->GetBinError(j - minChannel + 1);
			Double_t resolution = graphFrame[i]->getCurve("resolution")->Eval(j - minChannel + 0.5);
			Double_t fit = graphFrame[i]->getCurve("fit")->Eval(j - minChannel + 0.5);
			Double_t channel, chi;
			if (!doRange) {
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
