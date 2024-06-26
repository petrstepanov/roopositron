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
// #include <RooMinuit.h>
#include <RooFFTConvPdf.h>
#include <RooSimultaneous.h>
#include <RooPolynomial.h>
#include <RooNumIntConfig.h>
#include <RooCategory.h>
#include <RooChi2Var.h>
#include <RooDecay.h>
#include <RooPolynomial.h>
#include <RooCategory.h>
#include <RooChi2Var.h>
#include <RooDecay.h>
#include <RooPolynomial.h>
#include <RooCategory.h>
#include <RooChi2Var.h>
#include <RooDecay.h>
#include <RooPlot.h>
#include <RooHist.h>
#include <RooFFTConvPdf.h>
#include <TBrowser.h>
#include <TRandom.h>
#include <TROOT.h>
#include <TApplication.h>
#include <TFile.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TRootCanvas.h>
#include <TSystem.h>
#include <TStyle.h>
#include <TPaveStats.h>
#include <TLegend.h>
#include <TPaveText.h>
#include <TObjString.h>
#include <TList.h>
#include <TGaxis.h>
#include <TMatrixD.h>
#include <TCollection.h>
#include <TIterator.h>
#include <RooWorkspace.h>

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
#include <utility>

// Declare Map Container to perform simultaneous fit
typedef std::map<std::string, RooDataHist*> dhistMap;
typedef std::pair<std::string, RooDataHist*> dhistPair;

struct Spectrum {
	TString filename;
	TH1F* histogram;              // ROOT histogram
//	TH1F* histogramMaterial;      // Spectrum minus background and source contribution
	RooDataHist* dataHistogram;   // RooFit histogram
	Double_t integral;            // total counts
	Int_t numberOfBins;
	Int_t binWithMinimumCount;    // bin number with minimum value in the range
	Int_t binWithMaximumCount;    // bin number with maximum value in the range
	Double_t minimumCount;        // minimum count across all bins
	Double_t maximumCount;        // maximum count across all bins
	Double_t averageBackground;
	RooAbsPdf* model;
};

// ASCII font generator is here: http://patorjk.com/software/taag/#p=display&f=Graffiti&t=Resolution%0AFunction
int run(int argc, char* argv[], Bool_t isRoot = kFALSE) {

	// Change integrator steps to 30 (default 20)
	RooAbsReal::defaultIntegratorConfig()->getConfigSection("RooIntegrator1D").setRealValue("maxSteps", 30);

	// Scan current directory for '.Spe' files; exit if nothing found
	const std::vector<std::string> filenames = FileUtils::getFilenamesInCurrentDrectory(".Spe");
	if (filenames.size() < 1) {
		std::cout << "No Maestro '.Spe' files found in current directory. Press CTRL+C." << std::endl;
		return 0;
	}
	#ifdef USEDEBUG
		std::cout << "main" << std::endl << "Found " << filenames.size() << " spectra." << std::endl;
	#endif
	// Create constants object that stores values from "constants.txt" file
	Constants* constants = Constants::getInstance();

	// Define some global app constants
	const Int_t BINS = constants->getMaxChannel() - constants->getMinChannel() + 1;
	const Bool_t DO_RANGE =
			constants->getExcludeMinChannel() > 1 && constants->getExcludeMinChannel() < constants->getExcludeMaxChannel()
					&& constants->getExcludeMaxChannel() < BINS ? kTRUE : kFALSE;

	// Define Channels Observable
	RooRealVar* channels = new RooRealVar("channels", "Channels axis", 0, BINS, "ch");

	// Set convolution bins same as real number of axis bins (notices it works nice)
	channels->setBins(BINS);
//	channels->setBins(BINS, "default");
	channels->setBins(constants->getConvolutionBins() != 0 ? constants->getConvolutionBins() : BINS, "cache");

	// Construct a list of Spectrum struct     	 s to store individual spectra information
	std::vector<Spectrum> spectra;
	for (unsigned i = 0; i < filenames.size(); i++) {
		Spectrum s;
		s.filename = TString(filenames[i].c_str());
		s.histogram = FileUtils::importTH1F(filenames[i], i);
		s.dataHistogram = new RooDataHist(TString::Format("dataHistogram_%d", i + 1), s.filename.Data(), RooArgList(*channels), s.histogram);
		s.numberOfBins = s.histogram->GetXaxis()->GetNbins();
		s.integral = s.histogram->Integral();
		s.binWithMinimumCount = s.histogram->GetMinimumBin();
		s.binWithMaximumCount = s.histogram->GetMaximumBin();
		s.minimumCount = s.histogram->GetBinContent(s.binWithMinimumCount);
		s.maximumCount = s.histogram->GetBinContent(s.binWithMaximumCount);
		s.averageBackground = HistProcessor::getAverageBackground(s.histogram, Constants::getInstance()->getBackgroundBins());
		spectra.push_back(s);
		#ifdef USEDEBUG
			std::cout << "Spectrum " << i+1 << " file is \"" << s.filename << "\"" << std::endl << "  bins: " << s.numberOfBins << std::endl << "  integral: " << s.integral << std::endl << "  bin with minimum count: " << s.binWithMinimumCount << std::endl << "  bin with maximum count: " << s.binWithMaximumCount << std::endl << "  minimum count: " << s.minimumCount << std::endl << "  maximum count: " << s.maximumCount << std::endl << "  average background: " << s.averageBackground;
		#endif
	}

	// Initialize output path and storage for model parameters
	std::string outputPath = "./" + StringUtils::joinStrings(constants->getDecayModels()) + "-" + constants->getResolutionFunctionModel(); // + "-" + minimizerType.Data();
	FileUtils::createDirectory(outputPath);
	ParametersPool* pool = new ParametersPool(outputPath);


	// Construct additive decay models
	AdditiveConvolutionPdf* acp = new AdditiveConvolutionPdf(constants->getDecayModels(), constants->getResolutionFunctionModel(), constants->getSourceComponentsNumber(), channels);
	// Update fitting parameters starting values from pool (if exist in pool)
	pool->updateModelParameters(acp->getPdf()->getParameters(RooArgSet(*channels)));

	for (unsigned i = 0; i < spectra.size(); i++) {
		if (i == 0) {
			// First spectrum gets model from provider
			spectra[i].model = acp->getPdf();
		}
		else {
			// Prefix all parameters in not-first model
			// RooWorkspace suffixes the pdf by copying its instance and all child objects.
			spectra[i].model = RootHelper::suffixPdfAndNodes(acp->getPdf(), channels, TString::Format("%d", i+1));
		}

		// Set mean gauss values
		if (RooRealVar* gaussMean = RootHelper::getParameterByNameCommonOrLocal(spectra[i].model, AdditiveConvolutionPdf::VAR_MEAN_GAUSS_NAME)) {
			gaussMean->setConstant(kFALSE);
			gaussMean->Print();
			RootHelper::setRooRealVarValueLimits(gaussMean, spectra[i].binWithMaximumCount, spectra[i].binWithMaximumCount - 50, spectra[i].binWithMaximumCount + 50);
		}
		// Set average background counts
		if (RooRealVar* avgBgCount = RootHelper::getParameterByNameCommonOrLocal(spectra[i].model, AdditiveConvolutionPdf::VAR_BACKGROUND_COUNT_NAME)) {
			RootHelper::setRooRealVarValueLimits(avgBgCount, spectra[i].averageBackground, spectra[i].averageBackground/2, spectra[i].averageBackground*2);
		}
		// Set bins
		if (RooRealVar* bins = RootHelper::getParameterByNameCommonOrLocal(spectra[i].model, AdditiveConvolutionPdf::VAR_BINS_NAME)) {
			RootHelper::setRooRealVarValueLimits(bins, spectra[i].numberOfBins, spectra[i].numberOfBins, spectra[i].numberOfBins);
			bins->setConstant(kTRUE);
		}
		// Set full integral
		if (RooRealVar* fullIntegral = RootHelper::getParameterByNameCommonOrLocal(spectra[i].model, AdditiveConvolutionPdf::VAR_INTEGRAL_NAME)) {
			RootHelper::setRooRealVarValueLimits(fullIntegral, spectra[i].integral, spectra[i].integral, spectra[i].integral);
			fullIntegral->setConstant(kTRUE);
		}
	}

	// When fitting multiple spectra perform single first spectrum fit (1st in the list)
	// Erase all spectra from the list but first one
	Bool_t firstSpectrumWasFitted = pool->containsAllParameters(spectra[0].model->getParameters(*channels));
	if (spectra.size() > 1 && !firstSpectrumWasFitted){
		while(spectra.size() > 1){
			spectra.erase(spectra.end());
		}
	}

	// Introduce common parameters if more than one spectrum (simultaneous fit).
	if (spectra.size() > 1) {
		// Initialize commonizer, read parameters from the first spectrum
		ModelCommonizer* commonizer = new ModelCommonizer(spectra[0].model, channels, constants->getCommonParameters());

		// All other spectra
		// ParametersPool* pool = new ParametersPool(outputPath);
		for (unsigned i = 1; i < spectra.size(); i++) {
			RooAbsPdf* comminozedPdf = commonizer->replaceParametersWithCommon(spectra[i].model);
			spectra[i].model = comminozedPdf;
		}
	}

	// Output
	#ifdef USEDEBUG
		std::cout << "main" << std::endl << "Constructed following models:" << std::endl;
		for (unsigned i = 0; i < spectra.size(); i++) {
			spectra[i].model->Print();
		}
	#endif

	// Create output folder out of model components' names and resolution function name, e.g. "exp-exp-2gauss"
	// TString minimizerType(constants->getMinimizerType());
	// minimizerType.ToLower();

//  http://patorjk.com/software/taag/
//	___________.__  __    __  .__
//	\_   _____/|__|/  |__/  |_|__| ____    ____
//	 |    __)  |  \   __\   __\  |/    \  / ___\
//	 |     \   |  ||  |  |  | |  |   |  \/ /_/  >
//	 \___  /   |__||__|  |__| |__|___|  /\___  /
//	     \/                           \//_____/

	// Make array of category names
	TString* types = new TString[spectra.size()];
	for (unsigned i = 0; i < spectra.size(); i++)
		types[i] = TString::Format("spectrum_%d", i);

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

	if (spectra.size() > 1){
		// Read simultaneous model parameters from the pool file.
		// Important for non-common parameters that have prefix _#
		pool->updateModelParameters(simPdf->getParameters(*channels));
	}

	// User input parameter values from keyboard if not found in the pool
	pool->addInputModelParameters(simPdf->getParameters(*channels));

	if (spectra.size() > 1){
		// Proceed with fit or save "parameters.txt" file and quit
		std::cout << std::endl << "Simultaneous model is ready. Proceed with fit?" << std::endl;
		std::cout << "[y] start fitting." << std::endl << "[n] revise simultaneous parameters" << std::endl;
		std::string input;
		std::getline(std::cin, input);
		if (!input.empty()) {
			char character = input.at(0);
			if (character == 'n' || character == 'N') {
				pool->saveToFile();
				return 0;
			}
		}
	}

	// Save pool before fitting to create file with parameters
	// pool->saveToFile();

	// Use RooMinuit interface to minimize chi^2
	RooChi2Var* simChi2;
	if (DO_RANGE) {
		channels->setRange("LEFT", 1, constants->getExcludeMinChannel());
		channels->setRange("RIGHT", constants->getExcludeMaxChannel(), BINS);

		// Read models' parameters from the pool file. numCpu = RootHelper::getNumCpu();
		// RooChi2Var don't support ranges https://sft.its.cern.ch/jira/browse/ROOT-10038
		#if ROOT_VERSION_CODE >= ROOT_VERSION(6,31,0)
			RooAbsTestStatistic::Configuration chiConfig;
			chiConfig.rangeName="LEFT,RIGHT";
			chiConfig.nCPU=RootHelper::getNumCpu();
			simChi2 = new RooChi2Var("simChi2", "chi2", *simPdf, *combinedData, kTRUE, RooAbsData::ErrorType::Poisson, chiConfig);
		#else
			simChi2 = new RooChi2Var("simChi2", "chi2", *simPdf, *combinedData, RooFit::NumCPU(RootHelper::getNumCpu()));
		#endif
	} else {
		#if ROOT_VERSION_CODE >= ROOT_VERSION(6,31,0)
			RooAbsTestStatistic::Configuration chiConfig;
			chiConfig.nCPU=RootHelper::getNumCpu();
			simChi2 = new RooChi2Var("simChi2", "chi2", *simPdf, *combinedData, kTRUE, RooAbsData::ErrorType::Poisson, chiConfig);
		#else
			simChi2 = new RooChi2Var("simChi2", "chi2", *simPdf, *combinedData, RooFit::NumCPU(RootHelper::getNumCpu()));
		#endif
	}

	// Start timer to track performance
	TStopwatch* stopWatch = new TStopwatch();
	stopWatch->Start();

	// Minimize chi2
	RooMinimizer* m = new RooMinimizer(*simChi2);
	m->setMinimizerType(constants->getMinimizerType());
	// m->setEps(1000);
	// m->setStrategy(RooMinimizer::Speed); // RooMinimizer::Speed, RooMinimizer::Balance, RooMinimizer::Robustness

	Int_t resultMigrad = m->migrad();
	Int_t resultHesse = m->hesse();
	#ifdef USEDEBUG
		std::cout << "main" << std::endl << "Fitting completed: migrad=" << resultMigrad << ", hesse=" << resultHesse << std::endl;
	#endif
	stopWatch->Stop();

//	  ________                    .__    .__
//	 /  _____/___________  ______ |  |__ |__| ____   ______
//	/   \  __\_  __ \__  \ \____ \|  |  \|  |/ ___\ /  ___/
//	\    \_\  \  | \// __ \|  |_> >   Y  \  \  \___ \___ \
//	 \______  /__|  (____  /   __/|___|  /__|\___  >____  >
//	        \/           \/|__|        \/        \/     \/


	// Create canvases and pads first
	// Otherwise program segfaults when drawing stuff on a RooPlot (when calling AddLine() in TPaveText instance)
	TCanvas** canvas = new TCanvas*[spectra.size()];
	for (unsigned i = 0; i < spectra.size(); i++) {
		canvas[i] = new TCanvas(TString::Format("canvas-%d", i + 1), TString::Format("Spectrum N%d \"%s\"", i + 1, spectra[i].filename.Data()), constants->getImageWidth(),
				constants->getImageHeight());
		canvas[i]->SetFillColor(0);
		canvas[i]->Divide(1, 2);

		canvas[i]->cd(1)->SetPad(0, GraphicsHelper::RESIDUALS_PAD_RELATIVE_HEIGHT, 1, 1);
		gPad->SetMargin(0.07, 0.01, 0.08, 0.1);
		gPad->SetLogy();
		// gPad->SetGrid();

		canvas[i]->cd(2)->SetPad(0, 0, 1, GraphicsHelper::RESIDUALS_PAD_RELATIVE_HEIGHT);
		gPad->SetMargin(0.07, 0.01, 0.3, 0.05);
		// gPad->SetGrid();

		// Set Grid color (same as axis color)
		// gStyle->SetAxisColor(kRed, "xy");
	}

	// Draw spectrum plot (top)
	RooPlot** spectraPlot = new RooPlot*[spectra.size()];
	for (unsigned i = 0; i < spectra.size(); i++) {
		spectraPlot[i] = channels->frame();
		spectraPlot[i]->SetName(TString::Format("spectrum_%d", i));
		spectraPlot[i]->SetTitle(TString::Format("Spectrum %d \"%s\"", i + 1, spectra[i].filename.Data()));
		spectraPlot[i]->GetXaxis()->SetRangeUser(0, BINS);

		// Instantinate legend with empty first line
		TLegend* legend = new TLegend(/*x1*/ GraphicsHelper::LEGEND_XMIN-0.34, /*y1*/ 1-0.1-0.3, /*x2*/ GraphicsHelper::LEGEND_XMIN, /*y2*/ 1-0.1);
		legend->SetTextSize(GraphicsHelper::FONT_SIZE_SMALL);
		legend->SetTextFont(GraphicsHelper::FONT_TEXT);
		legend->SetBorderSize(0);
		legend->SetFillStyle(kFEmpty);
		legend->AddEntry("","","");

		// Plot data points
		spectra[i].dataHistogram->plotOn(spectraPlot[i], RooFit::LineStyle(kSolid), RooFit::LineColor(kBlack), RooFit::LineWidth(0), RooFit::MarkerSize(GraphicsHelper::MARKER_SIZE), RooFit::MarkerColor(kBlack), RooFit::Name("data"));
		legend->AddEntry(spectraPlot[i]->getHist("data"), "Experimental spectrum", "lep");

		// Draw Resolution Function
		RooAbsPdf* resolutionFunction = RootHelper::getComponentNameContains(spectra[i].model, AdditiveConvolutionPdf::PDF_RESOLUTION_FUNCTION_NAME);
		resolutionFunction->plotOn(spectraPlot[i], RooFit::LineStyle(kSolid), RooFit::LineColor(kGray + 1), RooFit::LineWidth(1), RooFit::Name("resolution"));
		legend->AddEntry(spectraPlot[i]->getCurve("resolution"), "Resolution function", "l");

		// Draw components. Problem: RooFFTConvPdf does not print components
		// Only print components that have "drawOnRooPlot" attribute set
		RooAbsPdf* modelNonConvoluted = RootHelper::getComponentNameContains(spectra[i].model, AdditiveConvolutionPdf::PDF_NON_CONVOLUTED_NAME);
		TIterator* it = modelNonConvoluted->getComponents()->createIterator();
		Int_t colorIndex = 0;
		while (TObject* temp = it->Next()) {
			if (RooAbsPdf* component = dynamic_cast<RooAbsPdf*>(temp)) {
				if (component->getAttribute("drawOnRooPlot") == kTRUE){
					Style_t color = GraphicsHelper::COLORS[colorIndex++];
					modelNonConvoluted->plotOn(spectraPlot[i], RooFit::LineStyle(kDashed), RooFit::LineColor(color), RooFit::LineWidth(2), RooFit::Name(component->GetName()), RooFit::Components(component->GetName()));
					std::string legendTitle = StringUtils::removeBrackets(component->GetTitle()); // RooWorkspace adds brackets (#) to object titles
					legend->AddEntry(spectraPlot[i]->getCurve(component->GetName()), legendTitle.c_str(), "l");
				}
			}
		}

		{
			// Obtain: background + source contribution * resolution function
			RooAbsPdf* bgPdf = RootHelper::getComponentNameContains(spectra[i].model, AdditiveConvolutionPdf::PDF_BACKGROUND_NAME);
			RooRealVar* bgCount = RootHelper::getParameterNameContains(spectra[i].model, AdditiveConvolutionPdf::VAR_BACKGROUND_COUNT_NAME);
			RooRealVar* bins = RootHelper::getParameterNameContains(spectra[i].model, AdditiveConvolutionPdf::VAR_BINS_NAME);
			RooRealVar* integral = RootHelper::getParameterNameContains(spectra[i].model, AdditiveConvolutionPdf::VAR_INTEGRAL_NAME);
			Double_t intBg = bgCount->getVal() * bins->getVal() / integral->getVal();
			RooAbsPdf* sourcePdf = RootHelper::getComponentNameContains(spectra[i].model, AdditiveConvolutionPdf::PDF_SOURCE_NAME);
			Double_t intSource = RootHelper::getParameterNameContains(spectra[i].model, AdditiveConvolutionPdf::VAR_INT_SOURCE_NAME)->getVal()/100;

			// Add background
			Double_t bgFraction = intBg/(intBg + (1-intBg)*intSource); // Background fraction with respect to source contribution
			RooRealVar* bgFractionVar = new RooRealVar(TString::Format("bgFractionVar%d", i).Data(), "Background fraction with respect to source contribution", bgFraction);
			RooAddPdf* sourceAndBgPdf = new RooAddPdf(TString::Format("sBgPdf%d", i).Data(), "Source contribution with background", *bgPdf, *sourcePdf, *bgFractionVar);
			sourceAndBgPdf->fixAddCoefNormalization(RooArgSet(*channels));

			// Convolute source contribution with resolution function
			RooFFTConvPdf* sourceAndBgConvolutedPdf = new RooFFTConvPdf(TString::Format("scp%d", i).Data(), "Source Convoluted PDF", *channels, *sourceAndBgPdf, *resolutionFunction);

			Double_t normalization = intBg + (1-intBg)*intSource;
			// sourceAndBgConvolutedPdf->plotOn(spectraPlot[i], RooFit::LineColor(kGray+1), RooFit::LineStyle(kDashed), RooFit::LineWidth(2), RooFit::Name("sourceAndBgConv"), RooFit::Normalization(normalization, RooAbsReal::Relative));
			sourceAndBgConvolutedPdf->plotOn(spectraPlot[i], RooFit::Invisible(), RooFit::LineStyle(kDashed), RooFit::LineWidth(1), RooFit::Name("sourceAndBgConv"), RooFit::Normalization(normalization, RooAbsReal::Relative));
			// legend->AddEntry(spectraPlot[i]->getCurve("sourceAndBgConv"), "Source contribution and Background (convoluted)", "l");

			// Next subtract that curve from the hist?
			const char* histMaterialName = TString::Format("%sMaterial", spectra[i].histogram->GetName()).Data();
			TH1F* materialHist = HistProcessor::subtractCurve(histMaterialName, spectra[i].histogram, spectraPlot[i]->getCurve("sourceAndBgConv"));
			RooDataHist* materialDataHist = new RooDataHist(TString::Format("materialDataHist_%d", i + 1), "Material data histogram", RooArgList(*channels), materialHist);
			materialDataHist->plotOn(spectraPlot[i], RooFit::LineWidth(0), RooFit::MarkerSize(GraphicsHelper::MARKER_SIZE), RooFit::MarkerColor(kGray), RooFit::Name("material"));
			legend->AddEntry(spectraPlot[i]->getHist("material"), "Spectrum without source contribution and bg", "lep");
		}

		// Draw fitting model
		if (DO_RANGE) {
			// Draw grayed out region excluded from plot
			GraphicsHelper::drawRegion(spectraPlot[i], constants->getExcludeMinChannel(), constants->getExcludeMaxChannel());
			spectra[i].model->plotOn(spectraPlot[i], RooFit::LineStyle(kSolid), RooFit::LineColor(GraphicsHelper::GRAPH_COLOR), RooFit::LineWidth(2), RooFit::LineStyle(kDashed));
			spectra[i].model->plotOn(spectraPlot[i], RooFit::LineStyle(kSolid), RooFit::LineColor(GraphicsHelper::GRAPH_COLOR), RooFit::LineWidth(2), RooFit::Name("fit"), RooFit::Range("LEFT,RIGHT"), RooFit::NormRange("LEFT,RIGHT"));
		} else {
			spectra[i].model->plotOn(spectraPlot[i], RooFit::LineStyle(kSolid), RooFit::LineColor(GraphicsHelper::GRAPH_COLOR), RooFit::LineWidth(2), RooFit::Name("fit"), RooFit::Normalization(spectra[i].integral, RooAbsReal::NumEvent));
		}
		legend->AddEntry(spectraPlot[i]->getCurve("fit"), "Fitting model", "l");

		// Add empty last line to legend
		legend->AddEntry("","","");
		legend->SetY1NDC(legend->GetY2NDC() - GraphicsHelper::LEGEND_LINE_HEIGHT*legend->GetNRows());
		std::cout << legend->GetTextSize() << " " << legend->GetTextFont() << std::endl;
		spectraPlot[i]->addObject(legend);

		// Add legend with list of model parameters
		RooArgSet* parameters = spectra[i].model->getParameters(spectraPlot[i]->getNormVars());

		RooArgSet* indirectParameters = new RooArgSet();  // Obtain indirect parameters from model components and resolution function PDFs
		std::vector<std::string> decayModelsList = constants->getDecayModels();  // For every decay model in the comma-separated list of models
		std::string resolutionId = constants->getResolutionFunctionModel();
		decayModelsList.push_back(resolutionId);  // Add resolution id to tokens to display its indirect parameters
		for (std::vector<std::string>::const_iterator it = decayModelsList.begin(); it != decayModelsList.end(); ++it) {
			const char* modelId = ((std::string) *it).c_str();
			RooArgSet* tempIndirectParameters = PdfFactory::getIndirectParameters(modelId, spectra[i].model);
			indirectParameters->add(*tempIndirectParameters);
		}

		RooArgSet* allParameters = new RooArgSet();
		allParameters->add(*parameters);
		allParameters->add(*indirectParameters);
		TPaveText* pt = GraphicsHelper::makePaveText(*allParameters, GraphicsHelper::LEGEND_XMIN, 1-0.01, 1-0.1);
		spectraPlot[i]->addObject(pt);

		// Set custom Y axis limits
		Double_t yMin = spectra[i].averageBackground / 5;
//		 Double_t yMin = pow(10, MathUtil::orderOfMagnitude(spectra[i].averageBackground)); // Minimum is order of magnitude of the average background value
		Double_t yMax = spectraPlot[i]->GetMaximum(); // Maximum is default (whatever ROOT plots)
		spectraPlot[i]->GetYaxis()->SetRangeUser(yMin, yMax);

		// Draw nanosecond axis
		Double_t scaleFactor = GraphicsHelper::getSpectrumPadFontFactor();
		if (RooRealVar* rooRealVar = RootHelper::getParameterNameContains(spectra[i].model, AdditiveConvolutionPdf::VAR_MEAN_GAUSS_NAME)) {
			Double_t zeroChannel = rooRealVar->getVal();

			// Get channel width for current spectrum
			Double_t channelWidth = 0;
//			RooArgSet* parameters = spectra[i].model->getParameters(*channels);
			if (RooRealVar* channelWidthVar = RootHelper::getParameterNameContains(spectra[i].model, AdditiveConvolutionPdf::VAR_CHANNEL_WIDTH_NAME)){
				channelWidth = channelWidthVar->getVal();
			}

			Double_t timeMin = -channelWidth * zeroChannel;
			Double_t timeMax = (channels->getMax() - zeroChannel) * channelWidth;
			TGaxis *timeAxis = new TGaxis(0, yMin, channels->getMax(), yMin, timeMin, timeMax, 510, "+L");
			timeAxis->SetName("timeAxis");
			timeAxis->SetTitleSize(scaleFactor * GraphicsHelper::FONT_SIZE_NORMAL);
			timeAxis->SetTitleOffset(0.64);
			timeAxis->SetTitleFont(42);
//			timeAxis->SetTitle("(ns)");
			timeAxis->SetLabelSize(scaleFactor * GraphicsHelper::FONT_SIZE_NORMAL);
			timeAxis->SetLabelOffset(0.02);
			timeAxis->SetLabelFont(42);
			spectraPlot[i]->addObject(timeAxis);

			// Add "(ns)" axis pave that overlays
			TPaveText *nsBox = new TPaveText(0.9, 0.006, 1/*-0.01*/, 0.08, "NDC"); // "NDC" is relative coordinates
			nsBox->SetTextAlign(ETextAlign::kHAlignRight + ETextAlign::kVAlignCenter);
			nsBox->SetTextSize(scaleFactor * GraphicsHelper::FONT_SIZE_NORMAL);
			nsBox->SetTextFont(42);
			nsBox->SetBorderSize(0);
			nsBox->SetMargin(0.1);
			nsBox->SetFillColor(EColor::kWhite);
			nsBox->AddText("(ns)");
			spectraPlot[i]->addObject(nsBox);
		}

		// Update regular axis
		spectraPlot[i]->GetXaxis()->SetLabelColor(0);
		spectraPlot[i]->GetXaxis()->SetTitleSize(0);
		spectraPlot[i]->GetXaxis()->SetTickLength(0);
		spectraPlot[i]->GetXaxis()->SetTickLength(scaleFactor * spectraPlot[i]->GetXaxis()->GetTickLength());

		spectraPlot[i]->GetYaxis()->SetLabelSize(scaleFactor * GraphicsHelper::FONT_SIZE_NORMAL);
		spectraPlot[i]->GetYaxis()->SetTitleSize(scaleFactor * GraphicsHelper::FONT_SIZE_NORMAL);
		spectraPlot[i]->GetYaxis()->SetTitleOffset(0.9);
		#ifdef USEDEBUG
			spectraPlot[i]->Print("V");
		#endif
	}
	#ifdef USEDEBUG
		std::cout << "main" << std::endl << "Spectra plots successfully created." << std::endl;
	#endif

	// Draw residuals plot (bottom)
	RooPlot** residualsPlot = new RooPlot*[spectra.size()];
	RooHist** hresid = new RooHist*[spectra.size()];
	for (unsigned i = 0; i < spectra.size(); i++) {
		residualsPlot[i] = channels->frame(RooFit::Title(" "));
		#if ROOT_VERSION_CODE >= ROOT_VERSION(6,31,0)
			RooChi2Var* chi2 = new RooChi2Var(TString::Format("#chi^{2}_{%d}", i + 1), "chi-square", *spectra[i].model, *spectra[i].dataHistogram, kTRUE, RooDataHist::ErrorType::Poisson);
		#else
			RooChi2Var* chi2 = new RooChi2Var(TString::Format("#chi^{2}_{%d}", i + 1), "chi-square", *spectra[i].model, *spectra[i].dataHistogram);
		#endif
		RooAbsCollection* freeParameters = (spectra[i].model->getParameters(*spectra[i].dataHistogram))->selectByAttrib("Constant", kFALSE);
		Int_t degreesFreedom = spectra[i].numberOfBins - freeParameters->getSize();
		Double_t chi2Value = chi2->getVal() / degreesFreedom;
		TObject* o = new TObject();
		// If doing ranges we have to manually construct chi frames from the curves
		// https://root-forum.cern.ch/t/pull-histogram-with-multiple-ranges/20935
		if (DO_RANGE) {
			RooHist* dataHist = (RooHist*) spectraPlot[i]->getHist("data");
			auto curve1 = (RooCurve*) spectraPlot[i]->getObject(4); // 2 is index in the list of RooPlot items (see printout from graphFrame[i]->Print("V")
			auto curve2 = (RooCurve*) spectraPlot[i]->getObject(5);
			auto hresid1 = dataHist->makePullHist(*curve1, true);
			auto hresid2 = dataHist->makePullHist(*curve2, true);
			hresid1->SetMarkerSize(GraphicsHelper::MARKER_SIZE);
			hresid2->SetMarkerSize(GraphicsHelper::MARKER_SIZE);
			// Draw options: "P" - with marker https://root.cern.ch/doc/v608/classTHistPainter.html
			residualsPlot[i]->addPlotable(hresid1, "P");
			residualsPlot[i]->addPlotable(hresid2, "P");
			#ifdef USEDEBUG
				std::cout << "main" << std::endl << "hresid2 draw option: " << hresid2->GetDrawOption() << std::endl;
			#endif
			// Since "HIST" draw option (no error bars) makes no effect? We manually ser errors to zero
			HistProcessor::setZeroErrors(hresid1);
			HistProcessor::setZeroErrors(hresid2);
		} else {
			hresid[i] = spectraPlot[i]->pullHist("data", "fit");
			hresid[i]->SetMarkerSize(GraphicsHelper::MARKER_SIZE);
			residualsPlot[i]->addPlotable(hresid[i], "P");
			#ifdef USEDEBUG
				std::cout << "main" << std::endl << "hresid[" << i << "] draw option: " << hresid[i]->GetDrawOption() << std::endl;
			#endif
			HistProcessor::setZeroErrors(hresid[i]);
		}

		// Draw region excluded from plot
		if (DO_RANGE) {
			GraphicsHelper::drawRegion(residualsPlot[i], constants->getExcludeMinChannel(), constants->getExcludeMaxChannel());
		}

		// Add legend with chi^2 value
		Double_t scaleFactor = GraphicsHelper::getResidualsPadFontFactor();
		TPaveText* leg = new TPaveText(GraphicsHelper::LEGEND_XMIN, 0.85, 0.99, 0.95, "BRNDC");  // x1 y1 x2 y2  0.75, 0.99, 0.9
		leg->AddText(TString::Format("#chi^{2} = %.1f / %d = %.3f", chi2->getVal(), degreesFreedom, chi2Value));
		leg->SetTextSize(scaleFactor * GraphicsHelper::FONT_SIZE_SMALL * 1.3);
		leg->SetTextFont(GraphicsHelper::FONT_TEXT);
		leg->SetBorderSize(1);
		leg->SetFillColor(0);
		leg->SetTextAlign(12);
		residualsPlot[i]->addObject(leg);

		// Draw horizontal line
		TLine* hr = new TLine(1, 0, channels->getBins(), 0);
		hr->SetLineStyle(1); // https://root.cern.ch/doc/master/classTAttLine.html#L3
		hr->SetLineWidth(2);
		hr->SetLineColor(GraphicsHelper::GRAPH_COLOR);
		residualsPlot[i]->addObject(hr);

		// Update axis
		residualsPlot[i]->GetYaxis()->SetTitle("Fit residuals");
		residualsPlot[i]->GetYaxis()->SetTitleOffset(0.48);
		residualsPlot[i]->GetYaxis()->SetTitleSize(scaleFactor * (GraphicsHelper::FONT_SIZE_NORMAL));
		residualsPlot[i]->GetYaxis()->SetLabelSize(scaleFactor * (GraphicsHelper::FONT_SIZE_NORMAL));
		residualsPlot[i]->GetYaxis()->SetLabelOffset(0.01);

		residualsPlot[i]->GetXaxis()->SetTitleSize(scaleFactor * (GraphicsHelper::FONT_SIZE_NORMAL));
		residualsPlot[i]->GetXaxis()->SetTitleOffset(2);
		residualsPlot[i]->GetXaxis()->SetLabelSize(scaleFactor * (GraphicsHelper::FONT_SIZE_NORMAL));
		residualsPlot[i]->GetXaxis()->SetLabelOffset(0.04);
		residualsPlot[i]->GetXaxis()->SetTickLength(scaleFactor * residualsPlot[i]->GetXaxis()->GetTickLength());

		// gStyle->SetAxisColor(kGreen);
		// gPad->RedrawAxis();

		#ifdef USEDEBUG
			residualsPlot[i]->Print("V");
		#endif
	}
	#ifdef USEDEBUG
		std::cout << "main" << std::endl << "Residual plots successfully created." << std::endl;
	#endif

	// Draw plots on the pads
	for (unsigned i = 0; i < spectra.size(); i++) {
		// Draw plots on correspondent pads
		canvas[i]->cd(1);
		spectraPlot[i]->Draw();
//		gStyle->SetAxisColor(kGreen);
//		gPad->RedrawAxis();

		canvas[i]->cd(2);
		residualsPlot[i]->Draw();

		canvas[i]->SetEditable(kFALSE);
		canvas[i]->Modified();
		canvas[i]->Update();

		// Save raster and vector images
		// https://root.cern.ch/doc/master/classTPad.html#a649899aa030f537517022a5d51ec152f
		TString imageFilePathName = TString::Format("%s/fit-%s-%s-%d", outputPath.c_str(), (StringUtils::joinStrings(constants->getDecayModels())).c_str(),
				constants->getResolutionFunctionModel(), i + 1);
		TString pngURI = imageFilePathName + ".png";
		// TString epsURI = imageFilePathName + ".eps";
		TString rootURI = imageFilePathName + ".root";

		canvas[i]->SaveAs(pngURI);
		canvas[i]->SaveAs(rootURI);
	}
	#ifdef USEDEBUG
		std::cout << "main" << std::endl << "Canvas images successfully exported." << std::endl;
	#endif

	gSystem->ProcessEvents();

	// Ask user how the fit is. Interrupt if bad.
//	std::string input;
//	if (spectra.size() > 1 && !firstSpectrumWasFitted){
//		std::cout << "First spectrum fit complete. Save fit parameters? (y/n)" << std::endl;
//	}
//	else {
//		std::cout << "Spectra fit complete. Save fit parameters? (y/n)" << std::endl;
//	}
//	std::getline(std::cin, input);
//	if (!input.empty()) {
//		char character = input.at(0);
//		if (character == 'n' || character == 'N') {
//			// If problem then exit.
//			std::cout << "Please try again with different starting parameters" << std::endl;
//			return 1;
//		}
//	}

	// Save parameters to file if everything looks good
	pool->saveToFile();

//	________          __                 __
//	\_____  \  __ ___/  |_______  __ ___/  |_
//	 /   |   \|  |  \   __\____ \|  |  \   __\
//	/    |    \  |  /|  | |  |_> >  |  /|  |
//	\_______  /____/ |__| |   __/|____/ |__|
//	        \/            |__|


	// TODO: move to FileUtils
	for (unsigned i = 0; i < spectra.size(); i++) {
		TString fileName = TString::Format("./%s-%s/data-%s-%s-%d.txt", (StringUtils::joinStrings(constants->getDecayModels())).c_str(),
				constants->getResolutionFunctionModel(), (StringUtils::joinStrings(constants->getDecayModels())).c_str(),
				constants->getResolutionFunctionModel(), i + 1);

		RooRealVar* channelWidthVar = RootHelper::getParameterNameContains(spectra[i].model, AdditiveConvolutionPdf::VAR_CHANNEL_WIDTH_NAME);
		Double_t channelWidth = (channelWidthVar) ? channelWidthVar->getVal() : 0;

		RooRealVar* meanGaussVar = RootHelper::getParameterNameContains(spectra[i].model, AdditiveConvolutionPdf::VAR_MEAN_GAUSS_NAME);
		Double_t meanGauss = (meanGaussVar) ? meanGaussVar->getVal() : 0;

		FileUtils::savePlotsToFile(spectraPlot[i], residualsPlot[i], fileName.Data(), channels, channelWidth, meanGauss, "time_ns");
	}
	stopWatch->Print();

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
