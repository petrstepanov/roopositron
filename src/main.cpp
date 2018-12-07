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
#include <TUnixSystem.h>

#include "util/FileUtils.h"
#include "util/RootHelper.h"

#include "model/Constants.h"
#include "model/RootConstants.h"
#include "model/ParamStorage.h"
#include "roofit/pdfs/TwoGaussian.h"
#include "roofit/pdfs/ExpPdf.h"
#include "roofit/pdfs/TrapPdf.h"
#include "roofit/pdfs/GrainPdf.h"
#include "roofit/pdfs/ThreeGaussian.h"

#include "util/GraphicsHelper.h"
#include "util/StringUtils.h"
#include "roofit/AdditiveConvolutionPdf.h"

//#include "temp/MyPdfCache.h"
//#include "temp/MyPdf.h"
//#include "temp/ConvPdf.h"
//#include "temp/ChannelConvolutionPdf.h"
//#include <unistd.h>

#include <iostream>
#include <sstream>

using namespace RooFit;
using namespace TMath;

// Declare Map Container to perform simultaneous fit
typedef std::map<std::string, RooDataHist*> dhistMap;
typedef std::pair<std::string, RooDataHist*> dhistPair;

//char* getCurrentTime(){
//    time_t now = time(0);
//    return ctime(&now);
//}

//Double_t getConstBackgroundFraction(TH1F* hist){
//    Int_t nBins = hist->GetXaxis()->GetNbins();
//    const Int_t wingBins = 20;
//    Double_t fullInt = hist->Integral(1, nBins);
//    Double_t leftWingAverage = (hist->Integral(1, wingBins)) / (Double_t)(wingBins);
////    Double_t rightWingAverage = (hist->Integral(nBins - wingBins, nBins)) / (Double_t)(wingBins);
////    Double_t bgInt = nBins * rightWingAverage;
//    Double_t bgInt = nBins * leftWingAverage;
//    return bgInt / fullInt;
//}

// ASCII font generator is here: http://patorjk.com/software/taag/#p=display&f=Graffiti&t=Resolution%0AFunction
int run(int argc, char* argv[], Bool_t isRoot = kFALSE){
	RootHelper::startTimer();

	// Print command line arguments
	std::cout << "Command line arguments (" << argc << "):" << std::endl;
	for (unsigned i = 0; i < argc; i++){
            std::cout << "argument " << i << ": " << argv[i] << std::endl;
        }

        // Input and output paths are either current directory or program arguments
//        std::string sInputPath = (isRoot || argc < 2) ? "" : argv[1];
//        std::string sOutputPath = (isRoot || argc < 2) ? "" : argv[2];

        std::string sInputPath = "";
        std::string sOutputPath = "";

        // Dirty fix for the Ghost peak
        std::string sGhostPeakCenter = (isRoot || argc == 1) ? "" : argv[1];
        int iGhostPeakCenter = 0;
        if(sGhostPeakCenter.length()>0){
            std::stringstream geek(sGhostPeakCenter);
            geek >> iGhostPeakCenter;
            std::cout << "Ghost peak center: " << iGhostPeakCenter << std:: endl;
        }
	// If ROOT session or directory not specified then set default input folder (./ - current directory)
	// otherwise use command line parameter (if provided)
        std::cout << "Input path is: \"" << sInputPath << "\"" << std::endl;

        // Get spectra filenames
        FileUtils* fileUtils = new FileUtils();
        std::list<std::string> fileNamesMap = fileUtils->getFilenamesInDrectory(sInputPath, ".Spe");

	// Define number of spectrums
	const int iNumberOfFiles = fileNamesMap.size();
	std::cout << "Found " << iNumberOfFiles << " files.\n" << std::endl;

        // Exit if no spectra found
        if (iNumberOfFiles < 1) {
            std::cout << "No Maestro `.Spe` files found in current directory. Press CTRL+C." << std::endl;
            return 0;
        }

	// Convert list to array of strings
	const char** array = new const char*[fileNamesMap.size()];
	unsigned index = 0;
	std::string* sFileNames = new std::string[fileNamesMap.size()];
	for (std::list<std::string>::const_iterator it = fileNamesMap.begin(); it != fileNamesMap.end(); ++it) {
		sFileNames[index] = it->c_str();
		index++;
	}

	TH1F** fullTH1F = new TH1F*[iNumberOfFiles];

	// Channels (bins) with maximum and minimum count
	Int_t* iTopChannel = new Int_t[iNumberOfFiles];
	Int_t* iLowChannel = new Int_t[iNumberOfFiles];

	// Y axis plotting range
	Int_t* iUpperLimit = new Int_t[iNumberOfFiles];
	Int_t* iLowerLimit = new Int_t[iNumberOfFiles];

        // Constants object reads values from "constants.txt" file
        Constants* constants = new Constants();
        if (!constants->isReadSuccess()){
            return 0;
        }
	
	// Set channel width in RootConstants (to be used in components instantiation)
	(RootConstants::getInstance())->channelWidth = new RooConstVar("channelWidth", "Channel width, ns", constants->getChannelWidth());
	
        const Int_t MIN_CHANNEL = constants->getMinChannel();
        const Int_t MAX_CHANNEL = constants->getMaxChannel();
        const Int_t CHANNELS = constants->getNumberOfChannels();
        Double_t CHANNEL_WIDTH = constants->getChannelWidth();
        const Int_t SKIP_LINES = constants->getSkipLines();

	for (unsigned i = 0; i < iNumberOfFiles; i++){
		// Import Data
		fullTH1F[i] = new TH1F(TString::Format("fullTH1F_%d", i + 1), TString::Format("fullTH1F_%d", i + 1), MAX_CHANNEL - MIN_CHANNEL + 1, 0, MAX_CHANNEL-MIN_CHANNEL+1);
                // Open File
                std::cout << "Spectrum " << i + 1 << " filename is \"" << sFileNames[i] << "\"" << std::endl;
                std::string path = sInputPath + sFileNames[i];
		fileUtils->importTH1F(fullTH1F[i], path, CHANNELS, SKIP_LINES, MIN_CHANNEL, MAX_CHANNEL);

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

	// Define Channels Axis

	RooRealVar* rChannels = new RooRealVar("rChannels", "Channels axis", 0, MAX_CHANNEL-MIN_CHANNEL+1, "ch");
	rChannels->setBins(MAX_CHANNEL - MIN_CHANNEL + 1);
	// Set convolution bins same as 
	rChannels->setBins(MAX_CHANNEL - MIN_CHANNEL + 1,"cache");
	// Conversion variables
	RooConstVar* channelWidth = new RooConstVar("channelWidth", "Bin channel width, ns", CHANNEL_WIDTH);

	// Convert TH1F spectra to RooDataHist
	RooDataHist** histSpectrum = new RooDataHist*[iNumberOfFiles];
	for (unsigned i = 0; i < iNumberOfFiles; i++){
            histSpectrum[i] = new RooDataHist(TString::Format("histSpectrum_%d", i + 1), sFileNames[i].c_str(), RooArgSet(*rChannels), RooFit::Import(*fullTH1F[i]));
	}

	// Read fit parameters from file
        // parameters filename is "parameters_XXX.txt", where XXX is model name (1exp, 2exp etc)
	std::string subDirectoryName = StringUtils::joinStrings(constants->getDecayModels(), "-"); // e.g. "exp-trapping"
	subDirectoryName += subDirectoryName += "-";
	ParamStorage* storage = new ParamStorage(subDirectoryName);

	// Construct additive decay models
        RooAbsPdf** decay_model = new RooAbsPdf*[iNumberOfFiles];
	for (unsigned i = 0; i < iNumberOfFiles; i++){
	    AdditiveConvolutionPdf* acp = new AdditiveConvolutionPdf(constants->getComponentIds(), const char* resolutionId, rChannels) {
	    if (i == 1){
		// Construct common parameters list
	    }
	}
	// TODO: Go through all model parameters and set gauss mean to whatever we have on the spectra.
	
	// TODO: Go through all model parameters and make common things common
	// Two-Gauss PDF
	RooAbsPdf** res_funct = new RooAbsPdf*[iNumberOfFiles];

	// Output
	std::cout << "Resolution Functions Created OK!" << std::endl;


	/*
          _________
         /   _____/ ____  __ _________   ____  ____
         \_____  \ /  _ \|  |  \_  __ \_/ ___\/ __ \
         /        (  <_> )  |  /|  | \/\  \__\  ___/
        /_______  /\____/|____/ |__|    \___  >___  >
                \/                          \/    \/
        _________                __         ._____.           __  .__
        \_   ___ \  ____   _____/  |________|__\_ |__  __ ___/  |_|__| ____   ____
        /    \  \/ /  _ \ /    \   __\_  __ \  || __ \|  |  \   __\  |/  _ \ /    \
        \     \___(  <_> )   |  \  |  |  | \/  || \_\ \  |  /|  | |  (  <_> )   |  \
         \______  /\____/|___|  /__|  |__|  |__||___  /____/ |__| |__|\____/|___|  /
                \/            \/                    \/                           \/

        */

	// Lifetime in source
	RooRealVar* t_source = storage->getOrMakeNew("t_source", "source_e+_lifetime", 0.385, 0.385, 0.385, "ns"); // Kapton
	RooFormulaVar* t_source_ch = new RooFormulaVar("t_source_ch", "@0/@1", RooArgList(*t_source, *channelWidth));

	RooRealVar* I_source = storage->getOrMakeNew("I_source", "source_contribution", 11, 8, 16, "%");
	RooFormulaVar* I_source_ = new RooFormulaVar("I_source_", "@0/100", *I_source);

        RooAbsPdf* decay_source;
        if(constants->getSourceContributionModel()=="1exp"){
            decay_source = new ExpPdf("decay_source", "decay_source", *rChannels, *t_source_ch);
        }
        else if(constants->getSourceContributionModel()=="2exp"){
            RooRealVar* t_source_2 = storage->getOrMakeNew("t_source_2", "source_2exp_lifetime", 3, 1, 7, "ns");
            RooFormulaVar* t_source_2_ch = new RooFormulaVar("t_source_2_ch", "@0/@1", RooArgList(*t_source_2, *channelWidth));
            RooRealVar* I_source_2 = storage->getOrMakeNew("I_source_2", "source_2exp_fraction", 5, 0, 15, "%");
            RooFormulaVar* I_source_2_ = new RooFormulaVar("I_source_2_", "@0/100.", *I_source_2);
            decay_source = new TwoExpPdf("decay_source", "decay_source", *rChannels, *t_source_ch, *t_source_2_ch, *I_source_2_);
        }


        // Old convolution begin
//      RooAddPdf* decay_model_sum = new RooAddPdf("decay_model_sum", "decay_model_sum", RooArgList(*decay_source, *decay_model), *I_source_);
//	RooFFTConvPdf** decay_model_with_source = new RooFFTConvPdf*[iNumberOfFiles];
//	rChannels->setBins(constants->getConvolutionBins(), "cache");
//        rChannels->setBins(MAX_CHANNEL - MIN_CHANNEL + 1, "cache");
//	for (unsigned i = 0; i<iNumberOfFiles; i++){
//            decay_model_with_source[i] = new RooFFTConvPdf(TString::Format("decay_model_with_source_%d", i + 1), TString::Format("Grain Boundary Model N%d", i + 1), *rChannels, *decay_model_sum, *res_funct[i]);
//            decay_model_with_source[i] -> setBufferStrategy(RooFFTConvPdf::Flat); // Extend (default), Flat, Mirror (dont work really well)
//            decay_model_with_source[i] -> setCacheObservables(*rChannels);
//            decay_model_with_source[i] -> setBufferFraction(1.0);
//	}
        // Old convolution end

        RooFFTConvPdf** model_conv = new RooFFTConvPdf*[iNumberOfFiles];
        RooFFTConvPdf** source_conv = new RooFFTConvPdf*[iNumberOfFiles];
        RooAddPdf** decay_model_with_source = new RooAddPdf*[iNumberOfFiles];
        rChannels->setBins(MAX_CHANNEL - MIN_CHANNEL + 1,"cache") ;
//        rChannels->setBins(2048,"cache") ;
        // New convolution begin (fix)
	for (unsigned i = 0; i<iNumberOfFiles; i++){
            model_conv[i] = new RooFFTConvPdf(TString::Format("model_conv_%d", i + 1), TString::Format("Model Conv N%d", i + 1), *rChannels, *decay_model, *res_funct[i]);
            source_conv[i] = new RooFFTConvPdf(TString::Format("source_conv_%d", i + 1), TString::Format("Source Conv N%d", i + 1), *rChannels, *decay_source, *res_funct[i]);
            decay_model_with_source[i] = new RooAddPdf(TString::Format("decay_model_with_source_%d", i + 1), "decay_model_with_source", RooArgList(*source_conv[i], *model_conv[i]), *I_source_);
	}
        // New convolution end (fix)

//      RooNumConvPdf** decay_model_with_source = new RooNumConvPdf*[iNumberOfFiles];
//      for (unsigned i = 0; i<iNumberOfFiles; i++){
//          decay_model_with_source[i] = new RooNumConvPdf(TString::Format("decay_model_with_source_%d", i + 1), TString::Format("Convoluted model N%d", i + 1), *rChannels, *decay_model_sum, *res_funct[i]);
////        ((RooNumConvPdf*) decay_model_with_source[i])->setConvolutionWindow(*zero_ch[i],*g1_fwhm,2);
//      }

//        rChannels->setBins(MAX_CHANNEL - MIN_CHANNEL + 1, "cache");
//        rChannels->setBins(500, "cache");
//        ChannelConvolutionPdf** decay_model_with_source = new ChannelConvolutionPdf*[iNumberOfFiles];
//        for (unsigned i = 0; i<iNumberOfFiles; i++){
//            decay_model_with_source[i] = new ChannelConvolutionPdf(TString::Format("decay_model_with_source_%d", i + 1), TString::Format("Convoluted model N%d", i + 1), *rChannels, *decay_model_sum, *res_funct[i]);
//            ((ChannelConvolutionPdf*) decay_model_with_source[i])->setConvolutionWindow(*zero_ch[i],*g1_fwhm,2);
//        }


	/*
        __________                __                                          .___
        \______   \_____    ____ |  | __  ___________  ____  __ __  ____    __| _/
         |    |  _/\__  \ _/ ___\|  |/ / / ___\_  __ \/  _ \|  |  \/    \  / __ |
         |    |   \ / __ \\  \___|    < / /_/  >  | \(  <_> )  |  /   |  \/ /_/ |
         |______  /(____  /\___  >__|_ \\___  /|__|   \____/|____/|___|  /\____ |
                \/      \/     \/     \/_____/                         \/      \/

	*/
	// Background Model is RooPolynomial: f(x) = sum_i a_i * x^i
	// as by default a_0=1 and a_i=0, so we get horisontal line

	// new RooPolynomial("constBg", "y=1", *x, RooArgSet(RooConst(1)));
	RooPolynomial** bg = new RooPolynomial*[iNumberOfFiles];
	for (unsigned i = 0; i < iNumberOfFiles; i++){
		bg[i] = new RooPolynomial(TString::Format("bg_func_%d", i + 1), "y=1", *rChannels, RooArgSet());
//		 bg[i] = new RooPolynomial(TString::Format("bg_func_%d", i + 1), "Background Polynom", *rChannels);
	}

	// Background fractions array
//	RooConstVar** I_bg = new RooConstVar*[iNumberOfFiles];
//	RooRealVar** bg = new RooRealVar*[iNumberOfFiles];
	RooFormulaVar** I_bg = new RooFormulaVar*[iNumberOfFiles];

//        Double_t* bgFraction = new Double_t[iNumberOfFiles];
	for (unsigned i = 0; i < iNumberOfFiles; i++){
//		bgFraction[i] = getConstBackgroundFraction(fullTH1F[i]);
//		I_bg[i] = new RooConstVar(TString::Format("I_bg_%d", i + 1), "background_fraction", bgFraction[i]); //, bgFraction[i]/2, bgFraction[i]*2);
//              Double_t rightWingAverage = (hist->Integral(nBins - wingBins, nBins)) / (Double_t)(wingBins);
//              Double_t bgInt = nBins * rightWingAverage;
//		I_bg[i] = new RooRealVar(TString::Format("I_bg_%d", i + 1), "background_fraction", bgFraction[i],bgFraction[i]/2, bgFraction[i]*2); //, bgFraction[i]/2, bgFraction[i]*2);

            // Get average background
            Int_t wingBins = 20;
            Int_t bins = fullTH1F[i]->GetXaxis()->GetNbins();
            Double_t wingIntegral = fullTH1F[i]->Integral(bins-20, bins);
            std::cout << "wingIntegral: " << wingIntegral << std::endl;
            Double_t averageBg = wingIntegral / wingBins;
            std::cout << "averageBg: " << averageBg << std::endl;
            RooRealVar* bg = new RooRealVar(TString::Format("bg_%d", i + 1), TString::Format("Background %d", i + 1), averageBg, averageBg/2, averageBg*2);
            bg->Print();
            RooConstVar* nBins = new RooConstVar(TString::Format("nBins_%d", i + 1), TString::Format("Number of bins %d", i + 1), bins);
            nBins->Print();
            RooConstVar* fullIntegral = new RooConstVar(TString::Format("fullInt_%d", i + 1), TString::Format("Full integral %d", i + 1), fullTH1F[i]->Integral(1, bins));
            fullIntegral->Print();
            I_bg[i] = new RooFormulaVar(TString::Format("I_bg_%d", i + 1), "@0*@1/@2", RooArgList(*bg, *nBins, *fullIntegral));
            I_bg[i]->Print();
	}

	// ADD BACKGROUND PDF
	RooAddPdf** decay_model_with_source_bg = new RooAddPdf*[iNumberOfFiles];
        for (unsigned i = 0; i < iNumberOfFiles; i++){
            decay_model_with_source_bg[i] = new RooAddPdf(TString::Format("Fit_%d", i + 1), "Decay Model with Source Contribution and Background", RooArgList(*bg[i], *decay_model_with_source[i]), *I_bg[i]);
        }
        // BEGIN ghost peak fix
//        if (iGhostPeakCenter == 0){
//            for (unsigned i = 0; i < iNumberOfFiles; i++){
//                    decay_model_with_source_bg[i] = new RooAddPdf(TString::Format("Fit_%d", i + 1), "Decay Model with Source Contribution and Background", RooArgList(*bg[i], *decay_model_with_source[i]), *I_bg[i]);
//            }
//        }
//        else {
//            RooAddPdf** decay_model_with_source_bg_temp = new RooAddPdf*[iNumberOfFiles];
//            std::cout << "Ghost detected!" << std::endl;
//            RooRealVar* mean = new RooRealVar("mean","mean of the ghost gaussian", iGhostPeakCenter, iGhostPeakCenter-30, iGhostPeakCenter+30, "ch") ;
//            RooRealVar* sigma = new RooRealVar("sigma","width of the ghost gaussian",75, 20, 150, "ch");
//
//            // Build gaussian p.d.f in terms of x,mean and sigma
//            RooGaussian* ghost = new RooGaussian("ghost","ghost gaussian PDF", *rChannels, *mean, *sigma);
//            RooRealVar* I_ghost = new RooRealVar("I_ghost", "ghost gaussian intensity", 0.0025, 0.0025 / 10, 0.0025 * 10);
//
//            for (unsigned i = 0; i < iNumberOfFiles; i++){
//                decay_model_with_source_bg_temp[i] = new RooAddPdf(TString::Format("Fit_%d", i + 1), "Decay Model with Source Contribution and Background", RooArgList(*bg[i], *decay_model_with_source[i]), *I_bg[i]);
//                decay_model_with_source_bg[i] = new RooAddPdf(TString::Format("Fit_ghost_%d", i + 1), "Decay Model with Source Contribution and Background", RooArgList(*ghost, *decay_model_with_source_bg_temp[i]), *I_ghost);
//            }
//        }
        // END ghost peak fix

	// Output
	std::cout << "Background OK!" << std::endl;

	// GET AND STORE NUMBER OF FREE PARAMETERS FROM THE MODEL CREATED
	RooArgSet** floatPars = new RooArgSet*[iNumberOfFiles];
	RooAbsCollection** floatPars1 = new RooAbsCollection*[iNumberOfFiles];
	Int_t* np = new Int_t[iNumberOfFiles];

	for (unsigned i = 0; i<iNumberOfFiles; i++){
		floatPars[i] = decay_model_with_source_bg[i]->getParameters(histSpectrum[i]);
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
            simPdf->addPdf(*decay_model_with_source_bg[i], types[i].Data());
	}

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

        RooMinimizer:
        RooMinimizer* m = new RooMinimizer(*simChi2);
//        m->setStrategy(); // RooMinimizer::Speed (default), RooMinimizer::Balance, RooMinimizer::Robustness
        m->setMinimizerType("Minuit");
        Int_t resultMigrad = m->migrad();
        Int_t resultHesse = m->hesse();
        std::cout << "RooMinimizer: migrad=" << resultMigrad << ", hesse=" << resultHesse << std::endl;

	RooArgSet* simFloatPars = simPdf->getParameters(*combinedData);
	RooAbsCollection* simFloatPars1 = simFloatPars->selectByAttrib("Constant", kFALSE);
	Int_t simNp = simFloatPars1->getSize();

	std::cout << "Fit Performed OK!" << std::endl;

	// Save parameters to file
	// if (!storage->fileExists(filename)) storage->save();
	storage->save();

      	// Variable to store chi2 values for every spectrum
	RooChi2Var** chi2 = new RooChi2Var*[iNumberOfFiles];
	//  for (i=0; i<iNumberOfFiles; i++) chi2[i] = new RooChi2Var (TString::Format("chi2_%d",i+1),"chi powered",*decay_model_with_source_bg[i],*histSpectrum[i],NumCPU(2),DataError(RooAbsData::Poisson));
	for (unsigned i = 0; i<iNumberOfFiles; i++){
            chi2[i] = new RooChi2Var(TString::Format("chi2_%d", i + 1), "chi powered", *decay_model_with_source_bg[i], *histSpectrum[i]); // NumCPU(8) , , DataError(RooAbsData::None)
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
//                histSpectrum[i]->plotOn(graphFrame[i], LineStyle(kSolid), LineColor(kBlack), LineWidth(0), MarkerSize(0.2), MarkerColor(kBlack), Range("LEFT,RIGHT"));
//            } else {
//                histSpectrum[i]->plotOn(graphFrame[i], LineStyle(kSolid), LineColor(kBlack), LineWidth(0), MarkerSize(0.2), MarkerColor(kBlack));
//            }
            histSpectrum[i]->plotOn(graphFrame[i], LineStyle(kSolid), LineColor(kBlack), LineWidth(0), MarkerSize(0.2), MarkerColor(kBlack), Name("data"));


            // Draw Resolution Function summed with Background
//            RooRealVar* bgFractionReal = new RooRealVar(TString::Format("bg_fraction_real_%d", i+1), "", bgFraction[i]);
//            RooAddPdf* resFuncPlusBg = new RooAddPdf(TString::Format("res_funct_with_bg_%d", i+1), TString::Format("res_funct_with_bg_%d", i+1), RooArgList(*bg[i], *res_funct[i]), RooArgList(*bgFractionReal));// , kTRUE);
//            resFuncPlusBg->plotOn(graphFrame[i], LineStyle(3), LineColor(kGray + 3), LineWidth(1), Name("resolution"));

            // Draw Resolution Function
            res_funct[i]->plotOn(graphFrame[i], LineStyle(3), LineColor(kGray + 3), LineWidth(1), Name("resolution"));

            // Draw complete fit, dont't forget the ranges option
            // https://root-forum.cern.ch/t/excluding-regions-in-a-fit/9109
            if (doRange){
                decay_model_with_source_bg[i]->plotOn(graphFrame[i], LineStyle(kSolid), LineColor(kPink - 4), LineWidth(2), Name("fit"), Range("LEFT,RIGHT"));
            } else {
                decay_model_with_source_bg[i]->plotOn(graphFrame[i], LineStyle(kSolid), LineColor(kPink - 4), LineWidth(2), Name("fit"));
            }

            //                std::string legendLabel = constants->getDecayModel() + " model parameters";
            decay_model_with_source_bg[i]->paramOn(graphFrame[i], Layout(0.78, 0.97, 0.9), Format("NEU", AutoPrecision(3)), ShowConstants(kTRUE));// , Label(legendLabel.c_str()) Parameters(decay_model_with_source_bg[i] -> getParameters(histSpectrum[i]);

            graphFrame[i]->Print("V");
	}

	std::cout << "Plot Frames Created OK!" << std::endl;


	// Evaluate Chi2 Values
	Double_t* chi2Value = new Double_t[iNumberOfFiles];
	RooHist** hresid = new RooHist*[iNumberOfFiles];
	RooPlot** chiFrame = new RooPlot*[iNumberOfFiles];
	Int_t* n_Degree = new Int_t[iNumberOfFiles];
	for (unsigned i = 0; i<iNumberOfFiles; i++){
                RooAbsCollection* freeParameters = (decay_model_with_source_bg[i]->getParameters(*histSpectrum[i]))->selectByAttrib("Constant",kFALSE);
		n_Degree[i] = MAX_CHANNEL - MIN_CHANNEL + 1 - freeParameters->getSize();
		// std::cout << "simNp: " << simNp << std::endl;
		// chi2Value[i] = (simChi2->getVal()) / n_Degree[i];  // multiply on number of files
		chi2Value[i] = chi2[i]->getVal() / n_Degree[i];  // multiply on number of files

                chiFrame[i] = rChannels->frame(Title(" ")); //Title(TString::Format("Goodness of fit: chi^2 = %.1f / %d = %.3f", chi2[i]->getVal(), n_Degree[i], chi2Value[i])));
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

            TLegend* leg = new TLegend(0.78,0.8,0.97-0.01,0.95-0.01);  // x1 y1 x2 y2
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
                    (constants->getDecayModel()).c_str(),
                    (constants->getResolutionFunctionModel()).c_str(),
                    (constants->getDecayModel()).c_str(),
                    (constants->getResolutionFunctionModel()).c_str(), i+1);
            fileUtils->saveImage(canvas[i], imageFilename.Data());
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
    for (unsigned i = 0; i<iNumberOfFiles; i++){
        std::ofstream outputFile;
        TString dataFilename = TString::Format("./%s-%s/data-%s-%s-%d.txt",
                (constants->getDecayModel()).c_str(),
                (constants->getResolutionFunctionModel()).c_str(),
                (constants->getDecayModel()).c_str(),
                (constants->getResolutionFunctionModel()).c_str(), i+1);
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

    // Write
    //std::cout << "Current date and time is " << getCurrentTime() << std::endl;

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
