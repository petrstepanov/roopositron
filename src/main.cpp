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

#include "util/FileUtils.h"
#include "util/RootHelper.h"

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
    std::string filename;
    Double_t counts;                 // total counts
    Int_t channels;
    Int_t minimumBin;             // bin number with minimum event count
    Int_t maximumBin;             // bin number with maximum event count
    Double_t minimumCount;           // minimum count across all bins
    Double_t maximumCount;           // maximum count across all bins
};

// ASCII font generator is here: http://patorjk.com/software/taag/#p=display&f=Graffiti&t=Resolution%0AFunction
int run(int argc, char* argv[], Bool_t isRoot = kFALSE){
	// Start timer to track performance
	RootHelper::startTimer();

	// Construct a list of .Spe files from current directory
        std::vector<std::string> filenames = FileUtils::getFilenamesInCurrentDrectory(".Spe");
	
	// Construct a list of Spectrum structs to store spectra information
	std::vector<Spectrum> spectra;
	for (unsigned i=0; i<filenames.size(); i++){
	    Spectrum s;
	    TH1F* histogram = FileUtils::importTH1F(filenames[i], i);
	    s.histogram = histogram;
	    s.filename = filenames[i];
	    s.channels = histogram->GetXaxis()->GetNbins();
	    s.counts = histogram->Integral();
	    s.minimumBin = histogram->GetMinimumBin();
	    s.maximumBin = histogram->GetMaximumBin();
	    s.minimumCount = histogram->GetBinContent(histogram->GetMinimumBin());
	    s.maximumCount = histogram->GetBinContent(histogram->GetMaximumBin());
	}
	
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
		sFileNames[index] = ((std::string)*it).c_str();
		index++;
	}

	TH1F** fullTH1F = new TH1F*[iNumberOfFiles];

	// Channels (bins) with maximum and minimum count
	Int_t* iTopChannel = new Int_t[iNumberOfFiles];
	Int_t* iLowChannel = new Int_t[iNumberOfFiles];

//	// Y axis plotting range
//	Int_t* iUpperLimit = new Int_t[iNumberOfFiles];
//	Int_t* iLowerLimit = new Int_t[iNumberOfFiles];

        // Constants object reads values from "constants.txt" file
        Constants* constants = Constants::getInstance();
	
	// TODO: drop these variables in favor of histogram struct

        const Int_t MIN_CHANNEL = constants->getMinChannel();
        const Int_t MAX_CHANNEL = constants->getMaxChannel();
        const Int_t CHANNELS = constants->getNumberOfChannels();
        Double_t CHANNEL_WIDTH = constants->getChannelWidth();
        const Int_t SKIP_LINES = constants->getSkipLines();

	for (unsigned i = 0; i < iNumberOfFiles; i++){
		// Import Data
		fullTH1F[i] = new TH1F(TString::Format("fullTH1F_%d", i + 1), TString::Format("fullTH1F_%d", i + 1), MAX_CHANNEL - MIN_CHANNEL + 1, 0, MAX_CHANNEL-MIN_CHANNEL+1);
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
//		iUpperLimit[i] = 2 * iMaxCount;
//		iLowerLimit[i] = iMinCount / 2;
	}

	// Define Channels Axis

	RooRealVar* rChannels = new RooRealVar("rChannels", "Channels axis", 0, MAX_CHANNEL-MIN_CHANNEL+1, "ch");
	rChannels->setBins(MAX_CHANNEL - MIN_CHANNEL + 1);
	// Set convolution bins same as 
	rChannels->setBins(MAX_CHANNEL - MIN_CHANNEL + 1,"cache");

	// Convert TH1F spectra to RooDataHist
	RooDataHist** histSpectrum = new RooDataHist*[iNumberOfFiles];
	for (unsigned i = 0; i < iNumberOfFiles; i++){
            histSpectrum[i] = new RooDataHist(TString::Format("histSpectrum_%d", i + 1), sFileNames[i].c_str(), RooArgSet(*rChannels), RooFit::Import(*fullTH1F[i]));
	}

	// Construct additive decay models
	ObjectNamer* simultaneousNamer = new ObjectNamer("-");
        RooAbsPdf** decay_model = new RooAbsPdf*[iNumberOfFiles];
	RooAbsPdf** res_funct = new RooAbsPdf*[iNumberOfFiles];
	for (unsigned i = 0; i < iNumberOfFiles; i++){
	    AdditiveConvolutionPdf* acp = new AdditiveConvolutionPdf(constants->getDecayModels(), constants->getResolutionFunctionModel(), rChannels);
	    RooAbsPdf* pdf = acp->getPdf();
	    
	    // Hack - set mean gauss values
	    RooRealVar* gaussMean = (RooRealVar*) (pdf->getParameters(*rChannels))->find("gaussMean");
	    if (gaussMean){ 
		gaussMean->setVal(iTopChannel[i]);
	    }
	    
	    // Initialize background here
	    RooPolynomial* bg = new RooPolynomial("bg", "y=1", *rChannels, RooArgSet());
	    Double_t averageBackground = HistProcessor::getAverageBackground(fullTH1F[i]);
	    Int_t b = fullTH1F[i]->GetXaxis()->GetNbins();
	    // Parameterize background as counts, not as fraction
	    RooRealVar* bgCount = new RooRealVar("bgCount", "Background level counts", averageBackground, averageBackground/2, averageBackground*2, "counts");
	    RooConstVar* bins = new RooConstVar("bins", "Histogram bins", b);
	    RooConstVar* fullIntegral = new RooConstVar("fullIntegral", "Full histogram integral", fullTH1F[i]->Integral(1, b));
	    RooFormulaVar* IBg = new RooFormulaVar("IBg", "@0*@1/@2", RooArgList(*bgCount, *bins, *fullIntegral));	    
	    
	    decay_model[i] = new RooAddPdf("componentsSourceBackgroundModel", "Components model with source contribution and background", RooArgList(*bg, *pdf), *IBg);
	    res_funct[i] = acp->getResolutionFunction();
	    
	    // Run namer, add "_#" to every parameter and model name
//	    simultaneousNamer->fixUniquePdfAndParameterNames(decay_model[i], rChannels);
	}

//	// Introduce common parameters if more than one spectrum (simultaneous fit).
//	if (iNumberOfFiles > 1){
//	    // Initialize commonizer, read parameters from the first spectrum
//	    ModelCommonizer* commonizer = new ModelCommonizer(decay_model[0], rChannels, constants->getCommonParameters());	
//	    // All other spectra 
//	    for (unsigned i = 1; i < iNumberOfFiles; i++){
//		RooAbsPdf* newPdf = commonizer->replaceParametersWithCommon(decay_model[i]);
//		decay_model[i] = newPdf;
//	    }
//	}

	// Output
	std::cout << std::endl << "Constructed following models" << std::endl;
	for (unsigned i = 0; i < iNumberOfFiles; i++){	
	    std::cout << std::endl << "Model " << i+1 << ": " << decay_model[i]->GetName() << std::endl;
	    //RootHelper::printPdfCoefficientNames(decay_model[i], rChannels);
	    decay_model[i]->Print();
	}	
	
	// Create output folder out of model components' names and resolution function name, e.g. "exp-exp-2gauss"
	std::string outputPath = StringUtils::joinStrings(constants->getDecayModels())
			       + "-" + constants->getResolutionFunctionModel();
	FileUtils::createDirectory(outputPath);
	
	// Read models' parameters from the pool.
	// User input parameter values from keyboard if not found in the pool
	ParametersPool* storage = new ParametersPool(outputPath);
	for (unsigned i = 0; i<iNumberOfFiles; i++){
	    storage->updateModelParametersValuesFromPool(decay_model[i]->getParameters(*rChannels));
	}	
	

	// Obtain and store number of free parameters for every model (why?)
	RooArgSet** floatPars = new RooArgSet*[iNumberOfFiles];
	RooAbsCollection** floatPars1 = new RooAbsCollection*[iNumberOfFiles];
	Int_t* np = new Int_t[iNumberOfFiles];

	for (unsigned i = 0; i<iNumberOfFiles; i++){
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
        __________
        \______   \_______  ____   ____  ____   ______ ______
         |     ___/\_  __ \/  _ \_/ ___\/ __ \ /  ___//  ___/
         |    |     |  | \(  <_> )  \__\  ___/ \___ \ \___ \
         |____|     |__|   \____/ \___  >___  >____  >____  >
                                      \/    \/     \/     \/

	*/

	// Save storage before fitting to create file with parameters
        // in case   user doesnt want to wait till fitting ends
        storage->save();

        // Make array of category names
        TString* types = new TString[iNumberOfFiles];
        for (unsigned i = 0; i<iNumberOfFiles; i++) types[i] = TString::Format("spectrum_%d", i);

	// Define category and map with spectra
	RooCategory* category = new RooCategory("category", "sample");
	std::map<std::string, RooDataHist*> histogramsMap;
	for (unsigned i = 0; i<iNumberOfFiles; i++){
            std::string type = types[i].Data();
            category->defineType(type.c_str());
            histogramsMap.insert(dhistPair(type, histSpectrum[i]));
	}

	// Construct combined dataset
        RooDataHist* combinedData = new RooDataHist("combData", "combined data", *rChannels, *category, histogramsMap);

	// Following code is a workaround
//	RooDataHist* combData;
//	if (iNumberOfFiles == 1)      combData = new RooDataHist("combData", "combined data", *rChannels, Index(*sample), Import("spec0", *histSpectrum[0]));
//	else if (iNumberOfFiles == 2) combData = new RooDataHist("combData", "combined data", *rChannels, Index(*sample), Import("spec0", *histSpectrum[0]), Import("spec1", *histSpectrum[1]));
//	else if (iNumberOfFiles == 3) combData = new RooDataHist("combData", "combined data", *rChannels, Index(*sample), Import("spec0", *histSpectrum[0]), Import("spec1", *histSpectrum[1]), Import("spec2", *histSpectrum[2]));
//	else if (iNumberOfFiles == 4) combData = new RooDataHist("combData", "combined data", *rChannels, Index(*sample), Import("spec0", *histSpectrum[0]), Import("spec1", *histSpectrum[1]), Import("spec2", *histSpectrum[2]), Import("spec3", *histSpectrum[3]));
//	else if (iNumberOfFiles == 5) combData = new RooDataHist("combData", "combined data", *rChannels, Index(*sample), Import("spec0", *histSpectrum[0]), Import("spec1", *histSpectrum[1]), Import("spec2", *histSpectrum[2]), Import("spec3", *histSpectrum[3]), Import("spec4", *histSpectrum[4]));
//	else if (iNumberOfFiles == 6) combData = new RooDataHist("combData", "combined data", *rChannels, Index(*sample), Import("spec0", *histSpectrum[0]), Import("spec1", *histSpectrum[1]), Import("spec2", *histSpectrum[2]), Import("spec3", *histSpectrum[3]), Import("spec4", *histSpectrum[4]), Import("spec5", *histSpectrum[5]));
//	else if (iNumberOfFiles == 7) combData = new RooDataHist("combData", "combined data", *rChannels, Index(*sample), Import("spec0", *histSpectrum[0]), Import("spec1", *histSpectrum[1]), Import("spec2", *histSpectrum[2]), Import("spec3", *histSpectrum[3]), Import("spec4", *histSpectrum[4]), Import("spec5", *histSpectrum[5]), Import("spec6", *histSpectrum[6]));
//	else exit(1);
//	combinedData->Print();

        // Construct combined model and add
	RooSimultaneous* simPdf = new RooSimultaneous("simPdf", "Simultaneous PDF", *category);
	for (unsigned i = 0; i < iNumberOfFiles; i++){
            simPdf->addPdf(*decay_model[i], types[i].Data());
	}
	RootHelper::printPdfCoefficientNames(simPdf, rChannels);
        // simPdf->fitTo(*combData) ;
	// simPdf->fitTo(*combData, NumCPU(NUM_CPU));

	// Use RooMinuit interface to minimize chi^2

        Int_t EXCLUDE_MIN_CHANNEL = constants->getExcludeMinChannel();
        Int_t EXCLUDE_MAX_CHANNEL = constants->getExcludeMaxChannel();
        Bool_t doRange = kFALSE;
        if (EXCLUDE_MIN_CHANNEL > 1 && EXCLUDE_MIN_CHANNEL < EXCLUDE_MAX_CHANNEL && EXCLUDE_MAX_CHANNEL < MAX_CHANNEL - MIN_CHANNEL){
            doRange = kTRUE;
        }

        RooChi2Var* simChi2;
        if (doRange){
            std::cout << "Doing Ranges!" << std::endl;
            rChannels->setRange("LEFT",1, EXCLUDE_MIN_CHANNEL);
            rChannels->setRange("RIGHT",EXCLUDE_MAX_CHANNEL, rChannels->getBins());
	    Int_t numCpu = RootHelper::getNumCpu();
            simChi2 = new RooChi2Var("simChi2", "chi2", *simPdf, *combinedData, RooFit::Range("LEFT,RIGHT"), RooFit::NumCPU(numCpu));


            // Define ranges for every spectrum, then use SplitRange... (not worked)
            // https://root-forum.cern.ch/t/trying-a-simultaneous-fit-in-roofit/3168
//            for (unsigned i = 0; i < iNumberOfFiles; i++){
//                TString rangeName = "LEFT";
//                rangeName += '_';
//                rangeName += types[i];
//                rChannels->setRange(rangeName.Data(),1, EXCLUDE_MIN_CHANNEL) ;
//
//                rangeName = "RIGHT";
//                rangeName += '_';
//                rangeName += types[i];
//                rChannels->setRange(rangeName.Data(),EXCLUDE_MAX_CHANNEL, rChannels->getBins()) ;
//            }
//             simChi2 = new RooChi2Var("simChi2", "chi2", *simPdf, *combinedData, RooFit::Range("LEFT,RIGHT"), RooFit::SplitRange(kTRUE));

            // Ranges only work with fitTo (not combined ranges)!
            // TODO: Try SplitRange() https://root-forum.cern.ch/t/trying-a-simultaneous-fit-in-roofit/3168
//            simPdf->fitTo(*combinedData, RooFit::Range("LEFT,RIGHT"), RooFit::SplitRange(kTRUE)) ;
//            simPdf->fitTo(*combinedData, RooFit::Range("LEFT,RIGHT")) ;
//            RooFitResult* r_sb12 = simPdf->fitTo(*combinedData,Range("LEFT,RIGHT",kFALSE),Save()) ;

        }
        else {
	    Int_t numCpu = RootHelper::getNumCpu();	    
            simChi2 = new RooChi2Var("simChi2", "chi2", *simPdf, *combinedData, RooFit::NumCPU(numCpu));
        }

        // RooMinimizer:
        RooMinimizer* m = new RooMinimizer(*simChi2);
        // m->setStrategy(); // RooMinimizer::Speed (default), RooMinimizer::Balance, RooMinimizer::Robustness
        m->setMinimizerType("Minuit");
        Int_t resultMigrad = 1; //m->migrad();
        Int_t resultHesse = 2; //m->hesse();
        std::cout << "RooMinimizer: migrad=" << resultMigrad << ", hesse=" << resultHesse << std::endl;

	RooArgSet* simFloatPars = simPdf->getParameters(*combinedData);
	RooAbsCollection* simFloatPars1 = simFloatPars->selectByAttrib("Constant", kFALSE);
	Int_t simNp = simFloatPars1->getSize();

	std::cout << "Fit Performed OK!" << std::endl;

	// Save parameters to file
	storage->save();

      	// Variable to store chi2 values for every spectrum
	RooChi2Var** chi2 = new RooChi2Var*[iNumberOfFiles];
	//  for (i=0; i<iNumberOfFiles; i++) chi2[i] = new RooChi2Var (TString::Format("chi2_%d",i+1),"chi powered",*decay_model_with_source_bg[i],*histSpectrum[i],NumCPU(2),DataError(RooAbsData::Poisson));
	for (unsigned i = 0; i<iNumberOfFiles; i++){
            chi2[i] = new RooChi2Var(TString::Format("chi2_%d", i + 1), "chi powered", *decay_model[i], *histSpectrum[i]); // NumCPU(8) , , DataError(RooAbsData::None)
	}

	/*
          ________                    .__    .__
         /  _____/___________  ______ |  |__ |__| ____   ______
        /   \  __\_  __ \__  \ \____ \|  |  \|  |/ ___\ /  ___/
        \    \_\  \  | \// __ \|  |_> >   Y  \  \  \___ \___ \
         \______  /__|  (____  /   __/|___|  /__|\___  >____  >
                \/           \/|__|        \/        \/     \/

	*/
	// Define frame for data points and fit
	RooPlot** graphFrame = new RooPlot*[iNumberOfFiles];
	for (unsigned i = 0; i < iNumberOfFiles; i++){
            // graphFrame[i] = rChannels -> frame(CHANNELS+1);
            graphFrame[i] = rChannels->frame(MAX_CHANNEL - MIN_CHANNEL + 1);
            graphFrame[i]->SetTitle(TString::Format("Spectrum N%d \"%s\"", i + 1, sFileNames[i].c_str()));
            graphFrame[i]->SetName(TString::Format("spectrum_%d", i));

            graphFrame[i]->GetXaxis()->SetRangeUser(0, MAX_CHANNEL-MIN_CHANNEL + 1);

//            if (doRange){
//                histSpectrum[i]->plotOn(graphFrame[i], RooFit::LineStyle(kSolid), RooFit::LineColor(kBlack), RooFit::LineWidth(0), RooFit::MarkerSize(0.2), RooFit::MarkerColor(kBlack), Range("LEFT,RIGHT"));
//            } else {
//                histSpectrum[i]->plotOn(graphFrame[i], RooFit::LineStyle(kSolid), RooFit::LineColor(kBlack), RooFit::LineWidth(0), RooFit::MarkerSize(0.2), RooFit::MarkerColor(kBlack));
//            }
            histSpectrum[i]->plotOn(graphFrame[i], RooFit::LineStyle(kSolid), RooFit::LineColor(kBlack), RooFit::LineWidth(0), RooFit::MarkerSize(0.2), RooFit::MarkerColor(kBlack), RooFit::Name("data"));


            // Draw Resolution Function summed with Background
//            RooRealVar* bgFractionReal = new RooRealVar(TString::Format("bg_fraction_real_%d", i+1), "", bgFraction[i]);
//            RooAddPdf* resFuncPlusBg = new RooAddPdf(TString::Format("res_funct_with_bg_%d", i+1), TString::Format("res_funct_with_bg_%d", i+1), RooArgList(*bg[i], *res_funct[i]), RooArgList(*bgFractionReal));// , kTRUE);
//            resFuncPlusBg->plotOn(graphFrame[i], RooFit::LineStyle(3), RooFit::LineColor(kGray + 3), RooFit::LineWidth(1), RooFit::Name("resolution"));

            // Draw Resolution Function
            res_funct[i]->plotOn(graphFrame[i], RooFit::LineStyle(3), RooFit::LineColor(kGray + 3), RooFit::LineWidth(1), RooFit::Name("resolution"));

            // Draw complete fit, dont't forget the ranges option
            // https://root-forum.cern.ch/t/excluding-regions-in-a-fit/9109
            if (doRange){
                decay_model[i]->plotOn(graphFrame[i], RooFit::LineStyle(kSolid), RooFit::LineColor(kPink - 4), RooFit::LineWidth(2), RooFit::Name("fit"), RooFit::Range("LEFT,RIGHT"));
            } else {
                decay_model[i]->plotOn(graphFrame[i], RooFit::LineStyle(kSolid), RooFit::LineColor(kPink - 4), RooFit::LineWidth(2), RooFit::Name("fit"));
            }

            //                std::string legendLabel = constants->getDecayModel() + " model parameters";
            decay_model[i]->paramOn(graphFrame[i], RooFit::Layout(0.78, 1, 0.9), RooFit::Format("NEU", RooFit::AutoPrecision(3)), RooFit::ShowConstants(kTRUE));// , Label(legendLabel.c_str()) Parameters(decay_model_with_source_bg[i] -> getParameters(histSpectrum[i]);

            graphFrame[i]->Print("V");
	}

	std::cout << "Plot Frames Created OK!" << std::endl;


	// Evaluate Chi2 Values
	Double_t* chi2Value = new Double_t[iNumberOfFiles];
	RooHist** hresid = new RooHist*[iNumberOfFiles];
	RooPlot** chiFrame = new RooPlot*[iNumberOfFiles];
	Int_t* n_Degree = new Int_t[iNumberOfFiles];
	for (unsigned i = 0; i<iNumberOfFiles; i++){
                RooAbsCollection* freeParameters = (decay_model[i]->getParameters(*histSpectrum[i]))->selectByAttrib("Constant",kFALSE);
		n_Degree[i] = MAX_CHANNEL - MIN_CHANNEL + 1 - freeParameters->getSize();
		// std::cout << "simNp: " << simNp << std::endl;
		// chi2Value[i] = (simChi2->getVal()) / n_Degree[i];  // multiply on number of files
		chi2Value[i] = chi2[i]->getVal() / n_Degree[i];  // multiply on number of files

                chiFrame[i] = rChannels->frame(RooFit::Title(" ")); //Title(TString::Format("Goodness of fit: chi^2 = %.1f / %d = %.3f", chi2[i]->getVal(), n_Degree[i], chi2Value[i])));
                // If doing ranges we have to manually construct chi frames from the curves
                // https://root-forum.cern.ch/t/pull-histogram-with-multiple-ranges/20935
                if (doRange){
                    RooHist* dataHist  = (RooHist*) graphFrame[i]->getHist("data");
                    auto curve1 = (RooCurve*) graphFrame[i]->getObject(2);  // 2 is index in the list of RooPlot items (see printout from graphFrame[i]->Print("V")
                    auto curve2 = (RooCurve*) graphFrame[i]->getObject(3);
                    auto hresid1 = dataHist->makePullHist(*curve1,true);
                    auto hresid2 = dataHist->makePullHist(*curve2,true);
                    hresid1->SetLineWidth(0);
                    hresid1->SetMarkerSize(0.2);
                    hresid2->SetLineWidth(0);
                    hresid2->SetMarkerSize(0.2);
                    chiFrame[i]->addPlotable(hresid1,"P");
                    chiFrame[i]->addPlotable(hresid2);
                }
                else {
                    hresid[i] = graphFrame[i]->pullHist();
                    hresid[i]->SetLineWidth(0);
                    hresid[i]->SetMarkerSize(0.2);
                    chiFrame[i]->addPlotable(hresid[i]);
                }
//                ((RooDataHist*)hresid[i])->plotOn(chiFrame[i],DataError(RooAbsData::None));
//		chiFrame[i]->GetXaxis()->SetRangeUser(0, MAX_CHANNEL-MIN_CHANNEL+1);

                // Write RooDataHist statistics on chi frame (to create TPaveStats object)
		// histSpectrum[i]->statOn(chiFrame[i]);
	}
	std::cout << "Chi2 Frames Created OK!" << std::endl;


	// DRAW CANVAS WITH BOTH FRAMES
	TCanvas** canvas = new TCanvas*[iNumberOfFiles];
	for (unsigned i = 0; i<iNumberOfFiles; i++){
            canvas[i] = new TCanvas(TString::Format("canvas-%d", i + 1), TString::Format("Spectrum N%d \"%s\"", i + 1, sFileNames[i].c_str()), constants->getImageWidth(), constants->getImageHeight());
            canvas[i]->SetFillColor(0);
            canvas[i]->Divide(1, 2);
//            canvas[i]->cd(1)->SetPad(0, 0.4, 1, 1); // xlow ylow xup yup
            canvas[i]->cd(1)->SetMargin(0.055, 0.01, 0.03, 0.1); // left right bottom top
            canvas[i]->cd(1)->SetLogy();
            Double_t fontSize = 0.06;
            graphFrame[i]->GetXaxis()->SetLabelColor(0);
            graphFrame[i]->GetXaxis()->SetTitleSize(0);
            graphFrame[i]->GetYaxis()->SetLabelSize(fontSize - 0.01);
            graphFrame[i]->GetYaxis()->SetRangeUser(1, iUpperLimit[i]);
            graphFrame[i]->GetYaxis()->SetTitleSize(fontSize);
            graphFrame[i]->GetYaxis()->SetTitleOffset(0.5);

            if (doRange){
                GraphicsHelper::drawRegion(graphFrame[i], EXCLUDE_MIN_CHANNEL, EXCLUDE_MAX_CHANNEL);
            }

            graphFrame[i]->Draw(); // Draw frame on current canvas (pad)

//            canvas[i]->cd(2)->SetPad(0, 0, 1, 0.4);
            canvas[i]->cd(2)->SetMargin(0.055, 0.01, 0.25, 0.05); // left right bottom top - margin for bottom title space

            chiFrame[i]->GetXaxis()->SetLabelSize(fontSize - 0.01);
            chiFrame[i]->GetYaxis()->SetLabelSize(fontSize - 0.01);
            chiFrame[i]->GetYaxis()->SetLabelOffset(0.01);
            chiFrame[i]->GetXaxis()->SetLabelOffset(0.04);
            chiFrame[i]->GetXaxis()->SetTitleSize(fontSize);
            chiFrame[i]->GetXaxis()->SetTitleOffset(2);

            if (doRange){
                GraphicsHelper::drawRegion(chiFrame[i], EXCLUDE_MIN_CHANNEL, EXCLUDE_MAX_CHANNEL);
            }

            // Draw horizontal line along residuals
            TLine* hr = new TLine(1, 0, rChannels->getBins(), 0);
            hr->SetLineStyle(7);
            hr->SetLineWidth(2);
            hr->SetLineColor(kPink - 4);
//            sBox->SetFillColorAlpha(2, 0.2);
            chiFrame[i]->addObject(hr);

            chiFrame[i]->Draw();

            TLegend* leg = new TLegend(0.78,0.8,0.97,0.95-0.01);  // x1 y1 x2 y2
            leg->SetHeader(TString::Format("chi^2 = %.1f / %d = %.3f", chi2[i]->getVal(), n_Degree[i], chi2Value[i]));
            leg->SetTextSize(0.05);
            leg->SetLineWidth(0);
            leg->Draw();

            //            ps->SetName("mystats");

//            if (!isRoot){
                canvas[i]->Modified();
                canvas[i]->Update();
//              gSystem->ProcessEvents();
//            }


            TString imageFilename = TString::Format("./%s-%s/fit-%s-%s-%d.png",
                    (StringUtils::joinStrings(constants->getDecayModels())).c_str(),
                    constants->getResolutionFunctionModel(),
                    (StringUtils::joinStrings(constants->getDecayModels())).c_str(),
                    constants->getResolutionFunctionModel(), i+1);
            FileUtils::saveImage(canvas[i], imageFilename.Data());
	}


        /*
        ________          __                 __
        \_____  \  __ ___/  |_______  __ ___/  |_
         /   |   \|  |  \   __\____ \|  |  \   __\
        /    |    \  |  /|  | |  |_> >  |  /|  |
        \_______  /____/ |__| |   __/|____/ |__|
                \/            |__|
        __________                                     __
        \______   \_____ ____________    _____ _____ _/  |_  ___________  ______
         |     ___/\__  \\_  __ \__  \  /     \\__  \\   __\/ __ \_  __ \/  ___/
         |    |     / __ \|  | \// __ \|  Y Y  \/ __ \|  | \  ___/|  | \/\___ \
         |____|    (____  /__|  (____  /__|_|  (____  /__|  \___  >__|  /____  >
                        \/           \/      \/     \/          \/           \/

        */
	
	// TODO: move to FileUtils
    for (unsigned i = 0; i<iNumberOfFiles; i++){
        std::ofstream outputFile;
        TString dataFilename = TString::Format("./%s-%s/data-%s-%s-%d.txt",
                (StringUtils::joinStrings(constants->getDecayModels())).c_str(),
                constants->getResolutionFunctionModel(),
                (StringUtils::joinStrings(constants->getDecayModels())).c_str(),
                constants->getResolutionFunctionModel(), i+1);
        outputFile.open(dataFilename.Data());

        const unsigned FILE_COLUMN_WIDTH = 20;
        // Write file header
        outputFile << std::left << std::setw(FILE_COLUMN_WIDTH) << "\"Channel\""
                   << std::left << std::setw(FILE_COLUMN_WIDTH) << "\"Time, ns\""
                   << std::left << std::setw(FILE_COLUMN_WIDTH) << "\"Count\""
                   << std::left << std::setw(FILE_COLUMN_WIDTH) << "\"Error\""
                   << std::left << std::setw(FILE_COLUMN_WIDTH) << "\"Resolution\""
                   << std::left << std::setw(FILE_COLUMN_WIDTH) << "\"Fit\""
                   << std::left << std::setw(FILE_COLUMN_WIDTH) << "\"Chi^2\"" << std::endl;

        for (unsigned j=MIN_CHANNEL; j<=MAX_CHANNEL; j++){
            Double_t time = (double)(j-MIN_CHANNEL)*(constants->getChannelWidth());
            Double_t count = fullTH1F[i]->GetBinContent(j-MIN_CHANNEL+1);
            Double_t error = fullTH1F[i]->GetBinError(j-MIN_CHANNEL+1);
            Double_t resolution = graphFrame[i]->getCurve("resolution")->Eval(j-MIN_CHANNEL+0.5);
            Double_t fit = graphFrame[i]->getCurve("fit")->Eval(j-MIN_CHANNEL+0.5);
            Double_t channel, chi;
            if (!doRange){
                hresid[i]->GetPoint(j-MIN_CHANNEL+1, channel, chi);
            }
            else {
//                TODO: add code for ranges (two curves)
            }
//
            outputFile << std::left << std::setw(FILE_COLUMN_WIDTH) << j
                       << std::left << std::setw(FILE_COLUMN_WIDTH) << time
                       << std::left << std::setw(FILE_COLUMN_WIDTH) << count
                       << std::left << std::setw(FILE_COLUMN_WIDTH) << error
                       << std::left << std::setw(FILE_COLUMN_WIDTH) << resolution
                       << std::left << std::setw(FILE_COLUMN_WIDTH) << fit
                       << std::left << std::setw(FILE_COLUMN_WIDTH) << chi;
            outputFile << std::endl;
        }

        outputFile.close();
    }

    RootHelper::stopAndPrintTimer();
    return 1;
}


int main(int argc, char* argv[]){
//    If need to use gdb - use this timeout and -O2 compiler flag
//    int seconds = 3;
//    usleep(seconds*1E6);

    // Create ROOT application
    // https://github.com/root-project/root/blob/master/tutorials/gui/mditest.C#L409
    TApplication* app = new TApplication("Positron Fit", &argc, argv);

    run(argc, argv);

    app->Run();
    return 0;
}

// void positronfit(){
    // If running app like `root -l positronfit.c` the instance of gApplication is created
    // run(gApplication->Argc(), gApplication->Argv(), kTRUE);
// }
