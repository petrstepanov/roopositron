#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif

#ifndef ROOT_TBrowser 
#include "TBrowser.h" 
#endif

#ifndef ROOT_TRandom
#include "TRandom.h"
#endif

#ifndef ROOT_TApplication
#include "TApplication.h"
#endif

#ifndef ROO_CONST_VAR
#include "RooConstVar.h"
#endif

#ifndef ROO_REAL_VAR
#include "RooRealVar.h"
#endif

#ifndef ROO_FORMULA_VAR 
#include "RooFormulaVar.h"
#endif

#ifndef ROO_DATA_HIST
#include "RooDataHist.h"
#endif

#ifndef ROO_GAUSSIAN
#include "RooGaussian.h"
#endif

#ifndef ROO_GAUSS_MODEL
#include "RooGaussModel.h"
#endif

#ifndef ROO_ADD_MODEL
#include "RooAddModel.h"
#endif

#ifndef ROO_GENERIC_PDF
#include "RooGenericPdf.h"
#endif

#ifndef ROO_ADD_PDF
#include "RooAddPdf.h"
#endif

#ifndef ROO_MINUIT
#include "RooMinuit.h"
#endif

#ifndef ROOFFTCONVPDF
#include "RooFFTConvPdf.h"
#endif

#ifndef ROO_SIMULTANEOUS
#include "RooSimultaneous.h"
#endif

#ifndef ROO_POLYNOMIAL
#include "RooPolynomial.h"
#endif

#ifndef ROO_CATEGORY
#include "RooCategory.h"
#endif

#ifndef ROO_CHI2_VAR
#include "RooChi2Var.h"
#endif

#ifndef ROO_DECAY
#include "RooDecay.h"
#endif

#ifndef ROO_PLOT
#include "RooPlot.h"
#endif

#ifndef ROO_HIST
#include "RooHist.h"
#endif

#ifndef ROOT_TFile
#include "TFile.h"
#endif

#ifndef ROOT_TH1F
#include "TH1F.h"
#endif

#ifndef ROOT_TCanvas
#include "TCanvas.h"
#endif

#ifndef ROOT_TStyle
#include "TStyle.h"
#endif

#ifndef MY_MYPDFCACHE
#include "MyPdfCache.h"
#endif

#ifndef MY_PARAMSTORAGE
#include "ParamStorage.h"
#endif

#ifndef MY_TWOGAUSSIAN
#include "TwoGaussian.h"
#endif

#ifndef MY_EXPPDF
#include "ExpPdf.h"
#endif

#ifndef MY_TWOEXPPDF
#include "TwoExpPdf.h"
#endif

#ifndef MY_MYPDF
#include "MyPdf.h"
#endif

#ifndef MY_TRAPPDF
#include "TrapPdf.h"
#endif

#ifndef MY_FILEUTILS
#include "FileUtils.h"
#endif

#ifndef MY_CONSTANTS
#include "Constants.h"
#endif

//#ifndef _UNISTD_H_
//#include <unistd.h>
//#endif

using namespace RooFit;
using namespace TMath;

// Declare Map Container to perform simultaneous fit
typedef std::map<std::string, RooDataHist*> dhistMap;
typedef std::pair<std::string, RooDataHist*> dhistPair;

char* getCurrentTime(){
    time_t now = time(0);
    return ctime(&now);
}

Double_t getConstBackgroundFraction(TH1F* hist){
    Int_t nBins = hist->GetXaxis()->GetNbins();
    const Int_t wingBins = 40;
    Double_t fullInt = hist->Integral(1, nBins);
    Double_t leftWingAverage = (hist->Integral(1, wingBins)) / (Double_t)(wingBins);
//    Double_t rightWingAverage = (hist->Integral(nBins - wingBins, nBins)) / (Double_t)(wingBins);
//    Double_t bgInt = nBins * rightWingAverage;
    Double_t bgInt = nBins * leftWingAverage;
    return bgInt / fullInt;
}

int run(TApplication* theApp, Bool_t isRoot = kFALSE){
        // Output current time
	std::cout << "Current date and time is " << getCurrentTime() << std::endl;

	// Print command line arguments
	int argc = theApp->Argc();
	char **argv = theApp->Argv();
	std::cout << "Command line arguments (" << argc << "):" << std::endl;
	for (unsigned i = 0; i < argc; i++){
            std::cout << "argument " << i << ": " << argv[i] << std::endl;
        }

        // Input and output paths are either current directory or program arguments
        std::string sInputPath = (isRoot || argc < 2) ? "" : argv[1];
        std::string sOutputPath = (isRoot || argc < 2) ? "" : argv[2];

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
        if (iNumberOfFiles < 1) return 0;
        
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
        Int_t MIN_CHANNEL = constants->getMinChannel();
        Int_t MAX_CHANNEL = constants->getMaxChannel();
        Int_t CHANNELS = constants->getNumberOfChannels();
        Double_t CHANNEL_WIDTH = constants->getChannelWidth();
        Int_t SKIP_LINES = constants->getSkipLines();
        
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

		// Evaluate counts axis limits (for graphical output)
		iUpperLimit[i] = 2 * iMaxCount;
		iLowerLimit[i] = iMinCount / 2;
	}

	/*
	_____         .__           _________       __
	/  _  \ ___  __|__| ______  /   _____/ _____/  |_ __ ________
	/  /_\  \\  \/  /  |/  ___/  \_____  \_/ __ \   __\  |  \____ \
	/    |    \>    <|  |\___ \   /        \  ___/|  | |  |  /  |_> >
	\____|__  /__/\_ \__/____  > /_______  /\___  >__| |____/|   __/
	\/      \/       \/          \/     \/           |__|
	*/

	// Define Channels Axis

	RooRealVar* rChannels = new RooRealVar("rChannels", "Channels axis", 0, MAX_CHANNEL-MIN_CHANNEL+1, "ch");
	rChannels->setBins(MAX_CHANNEL - MIN_CHANNEL + 1);

	// Conversion variables
	RooConstVar* channelWidth = new RooConstVar("channelWidth", "Channel width, ns", CHANNEL_WIDTH);

	// Convert TH1F spectra to RooDataHist
	RooDataHist** histSpectrum = new RooDataHist*[iNumberOfFiles];
	TCanvas** histCanvas = new TCanvas*[iNumberOfFiles];
	for (unsigned i = 0; i < iNumberOfFiles; i++){
		histSpectrum[i] = new RooDataHist(TString::Format("histSpectrum_%d", i + 1), sFileNames[i].c_str(), RooArgSet(*rChannels), fullTH1F[i]);
	}

	/*
	__________                    .__          __  .__
	\______   \ ____   __________ |  |  __ ___/  |_|__| ____   ____
	|       _// __ \ /  ___/  _ \|  | |  |  \   __\  |/  _ \ /    \
	|    |   \  ___/ \___ (  <_> )  |_|  |  /|  | |  (  <_> )   |  \
	|____|_  /\___  >____  >____/|____/____/ |__| |__|\____/|___|  /
	\/     \/     \/                                      \/
	___________                   __  .__
	\_   _____/_ __  ____   _____/  |_|__| ____   ____
	|    __)|  |  \/    \_/ ___\   __\  |/  _ \ /    \
	|     \ |  |  /   |  \  \___|  | |  (  <_> )   |  \
	\___  / |____/|___|  /\___  >__| |__|\____/|___|  /
	\/             \/     \/                    \/
	*/


	// CREATE RESOLUTION FUNCTIONS
	// by  the way, FWHM = 2 sqrt(2 ln 2) * dispersion
	//              FWHM = 2.3548 * dispersion
	//     0.4247 * FWHM = dispersion

	// Read resolution function parameters from file
        // parameters filename is "parameters_XXX.txt", where XXX is model name (1exp, 2exp etc)
        std::string modelName = constants->getDecayModel();
	ParamStorage* storage = new ParamStorage(modelName);
        
	RooConstVar* fwhm2disp = new RooConstVar("fwhm2disp", "Coefficient to convert fwhm to dispersion", 0.4247);  //1.0/(2.0*Sqrt(2.0*Log(2.0)))

	// Zero channel value is relative to the MIN_CHANNEL value
	RooRealVar** zero_ch = new RooRealVar*[iNumberOfFiles];
	RooConstVar* minChannel = new RooConstVar("minChannel", "Channel of Left Fitment Interval", MIN_CHANNEL);
	for (unsigned i = 0; i < iNumberOfFiles; i++){
		Double_t zeroChannelVal = iTopChannel[i] - 25;
		Double_t zeroChannelDelta = 35;
		TString name = TString::Format("zero_ch_%d", i + 1);
		TString description = TString::Format("zero_channel_%d", i + 1);
		zero_ch[i] = storage->getOrMakeNew(name, description, zeroChannelVal, zeroChannelVal - zeroChannelDelta, zeroChannelVal + zeroChannelDelta, "ch");
	}

	// 1st Gauss FWHM
	RooRealVar* g1_fwhm = storage->getOrMakeNew("g1_fwhm", "1st_gauss_fwhm", 0.300, 0.100, 0.500, "ns");
	RooFormulaVar* gauss_1_dispersion = new RooFormulaVar("gauss_1_dispersion", "@0*@1/@2", RooArgList(*g1_fwhm, *fwhm2disp, *channelWidth));

	// Fraction of the 1st gauss
	RooRealVar* gauss_1_fraction_pct = storage->getOrMakeNew("g1_frac", "1st_gauss_fraction", 98, 95, 100, "%");
	RooFormulaVar* gauss_1_fraction = new RooFormulaVar("g1_fraction", "@0/100", *gauss_1_fraction_pct);

	// 2nd gauss dispertion
	RooRealVar* g2_fwhm = storage->getOrMakeNew("g2_fwhm", "2nd_gauss_fwhm", 1.0, 0.5, 1.5, "ns");
	RooFormulaVar* gauss_2_dispersion = new RooFormulaVar("gauss_2_dispersion", "@0*@1/@2", RooArgList(*g2_fwhm, *fwhm2disp, *channelWidth));

	// 2nd Gauss shift
	RooRealVar** g2_shift = new RooRealVar*[iNumberOfFiles];
	RooFormulaVar** zero_ch_relative_2 = new RooFormulaVar*[iNumberOfFiles];
	for (unsigned i = 0; i < iNumberOfFiles; i++) {
		g2_shift[i] = storage->getOrMakeNew(TString::Format("g2_shift_%d", i + 1), TString::Format("2nd_gauss_shift_%d", i + 1), 0, -50, 50, "ch");
		zero_ch_relative_2[i] = new RooFormulaVar(TString::Format("zero_ch_relative_2_%d", i + 1), "@0+@1", RooArgList(*zero_ch[i], *g2_shift[i]));
	}

	// Two-Gauss PDF
	TwoGaussian** res_funct = new TwoGaussian*[iNumberOfFiles];
	for (unsigned i = 0; i<iNumberOfFiles; i++){
		res_funct[i] = new TwoGaussian(TString::Format("res_funct_%d", i + 1), "Resolution Function for Convolution (2 Gauss)", *rChannels, *zero_ch[i], *gauss_1_dispersion, *zero_ch_relative_2[i], *gauss_2_dispersion, *gauss_1_fraction);
	}

	// Output
	std::cout << "Resolution Functions Created OK!" << std::endl;


	/*
	________                                  _____             .___     .__
	\______ \   ____   ____ _____  ___.__.   /     \   ____   __| _/____ |  |
	|    |  \_/ __ \_/ ___\\__  \<   |  |  /  \ /  \ /  _ \ / __ |/ __ \|  |
	|    `   \  ___/\  \___ / __ \\___  | /    Y    (  <_> ) /_/ \  ___/|  |__
	/_______  /\___  >\___  >____  / ____| \____|__  /\____/\____ |\___  >____/
	\/     \/     \/     \/\/              \/            \/    \/

	*/

        RooAbsPdf* decay_model; // Polymorphism lol
        // Monoexponential model
        if (constants->getDecayModel() == "1exp"){
            RooRealVar* tau = storage->getOrMakeNew("tau", "positron_lifetime", 0.15, 0.1, 0.2, "ns");
            RooFormulaVar* tau_ch = new RooFormulaVar("tau_ch", "@0/@1", RooArgList(*tau, *channelWidth));
            decay_model = new ExpPdf("decay_model", "decay_model", *rChannels, *tau_ch);
        }
        // Two-exponential model
        else if(constants->getDecayModel() == "2exp"){
            RooRealVar* tau1 = storage->getOrMakeNew("tau1", "1st_exponent_lifetime", 0.15, 0.1, 0.7, "ns");
            RooFormulaVar* tau1_ch = new RooFormulaVar("tau1_ch", "@0/@1", RooArgList(*tau1, *channelWidth));
            RooRealVar* tau2 = storage->getOrMakeNew("tau2", "2nd_exponent_lifetime", 0.5, 0.3, 3.0, "ns");
            RooFormulaVar* tau2_ch = new RooFormulaVar("tau2_ch", "@0/@1", RooArgList(*tau2, *channelWidth));
            RooRealVar* I_tau2 = storage->getOrMakeNew("I_tau2", "2nd_exponent_fraction", 10, 0, 100, "%");
            RooFormulaVar* I_tau2_ = new RooFormulaVar("I_tau2_", "@0/100.", *I_tau2);
            decay_model = new TwoExpPdf("decay_model", "decay_model", *rChannels, *tau1_ch, *tau2_ch, *I_tau2_);        
        }
        // Trapping Model
        else if (constants->getDecayModel() == "trapping"){
            RooRealVar* tauBulk = storage->getOrMakeNew("tauBulk", "e+_lifetime_in_source", 0.15, 0.1, 0.3, "ns");
            RooFormulaVar* tauBulk_ch = new RooFormulaVar("tauBulk_ch", "@0/@1", RooArgList(*tauBulk, *channelWidth));            
            RooRealVar* tauVac = storage->getOrMakeNew("tauVac", "e+_lifetime_in_vacancy", 0.2, 0.1, 0.3, "ns");
            RooFormulaVar* tauVac_ch = new RooFormulaVar("tauVac_ch", "@0/@1", RooArgList(*tauVac, *channelWidth));            
            RooRealVar* kappaVac = storage->getOrMakeNew("kappaVac", "vacancy_trapping_rate", 1, 1E-2, 1E2, "1/ns");
            RooFormulaVar* kappaVac_ch = new RooFormulaVar("kappaVac_ch", "@0*@1", RooArgList(*kappaVac, *channelWidth));
            decay_model = new TrapPdf("decay_model", "decay_model", *rChannels, *tauBulk_ch, *tauVac_ch, *kappaVac_ch);
        }        
        // Grain Boundary Model
        else if (constants->getDecayModel() == "grain"){
            RooRealVar* tauBulk = storage->getOrMakeNew("tauBulk", "e+_lifetime_in_source", 0.120, 0.120, 0.120, "ns");
            RooFormulaVar* lambdaBulk = new RooFormulaVar("lambdaBulk", "1/@0", *tauBulk);
            RooFormulaVar* lambdaBulk_ch = new RooFormulaVar("lambdaBulk_ch", "@0*@1", RooArgList(*lambdaBulk, *channelWidth));

            RooRealVar* tauGrain = storage->getOrMakeNew("tauGrain", "e+_lifetime_in_grain", 0.140, 0.140, 0.140, "ns");
            RooFormulaVar* lambdaGrain = new RooFormulaVar("lambdaGrain", "1/@0", *tauGrain);
            RooFormulaVar* lambdaGrain_ch = new RooFormulaVar("lambdaGrain_ch", "@0*@1", RooArgList(*lambdaGrain, *channelWidth));

            RooRealVar* tauVac = storage->getOrMakeNew("tauVac", "e+_lifetime_in_vacancy", 0.180, 0.180, 0.180, "ns");
            RooFormulaVar* lambdaVac = new RooFormulaVar("lambdaVac", "1/@0", *tauVac);
            RooFormulaVar* lambdaVac_ch = new RooFormulaVar("lambdaVac_ch", "@0*@1", RooArgList(*lambdaVac, *channelWidth));

            RooRealVar* kappaVac = storage->getOrMakeNew("kappaVac", "vacancy_trapping_rate", 1, 1E-2, 1E2, "1/ns");
            RooFormulaVar* kappaVac_ch = new RooFormulaVar("kappaVac_ch", "@0*@1", RooArgList(*kappaVac, *channelWidth));

            RooRealVar* lambdaJ = storage->getOrMakeNew("lambdaJ", "grain_trapping_rate", 1, 1E-2, 1E2, "1/ns");
            RooFormulaVar* lambdaJ_ch = new RooFormulaVar("lambdaJ_ch", "@0*@1", RooArgList(*lambdaJ, *channelWidth));

            // Initialize cache
            MyPdfCache* myPdfCache = new MyPdfCache(100, MIN_CHANNEL, MAX_CHANNEL);
            MyPdf::myPdfCache = myPdfCache;

            decay_model = new MyPdf("decay_model", "decay_model", *rChannels, *lambdaJ_ch, *lambdaBulk_ch, *lambdaGrain_ch, *lambdaVac_ch, *kappaVac_ch);
            // add cache to model - counts 10x faster            
        }

        
	/*
	_________
	/   _____/ ____  __ _________   ____  ____
	\_____  \ /  _ \|  |  \_  __ \_/ ___\/ __ \
	/        (  <_> )  |  /|  | \/\  \__\  ___/
	/________/\____/|____/ |__|    \___  >___  >
	\/                          \/    \/
	_________                __         ._____.           __  .__
	\_   ___ \  ____   _____/  |________|__\_ |__  __ ___/  |_|__| ____   ____
	/    \  \/ /  _ \ /    \   __\_  __ \  || __ \|  |  \   __\  |/  _ \ /    \
	\     \___(  <_> )   |  \  |  |  | \/  || \_\ \  |  /|  | |  (  <_> )   |  \
	\______  /\____/|___|  /__|  |__|  |__||___  /____/ |__| |__|\____/|___|  /
	\/            \/                    \/                           \/

	*/

	// Lifetime in source
	RooRealVar* t_source = storage->getOrMakeNew("t_source", "source_e+_lifetime", 0.38, 0.38, 0.38, "ns"); // Kapton
	RooFormulaVar* t_source_ch = new RooFormulaVar("t_source_ch", "@0/@1", RooArgList(*t_source, *channelWidth));
        
	RooRealVar* I_source = storage->getOrMakeNew("I_source", "source_contribution", 12, 8, 16, "%");
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
        
	RooAddPdf* decay_model_sum = new RooAddPdf("decay_model_sum", "decay_model_sum", RooArgList(*decay_source, *decay_model), *I_source_);
	RooFFTConvPdf** decay_model_with_source = new RooFFTConvPdf*[iNumberOfFiles];
	rChannels->setBins(100, "cache");
	for (unsigned i = 0; i<iNumberOfFiles; i++){
		decay_model_with_source[i] = new RooFFTConvPdf(TString::Format("decay_model_with_source_%d", i + 1), TString::Format("Grain Boundary Model N%d", i + 1), *rChannels, *decay_model_sum, *res_funct[i]);
		decay_model_with_source[i]->setBufferFraction(0);
	}


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
		bg[i] = new RooPolynomial(TString::Format("bg_%d", i + 1), "y=1", *rChannels, RooArgSet());
		// bg[i] = new RooPolynomial(TString::Format("bg_%d", i + 1), "Background Polynom", *rChannels);
	}

	// Background fractions array
	RooRealVar** I_bg = new RooRealVar*[iNumberOfFiles];
        Double_t* bgFraction = new Double_t[iNumberOfFiles];
	for (unsigned i = 0; i < iNumberOfFiles; i++){
		bgFraction[i] = getConstBackgroundFraction(fullTH1F[i]);
		I_bg[i] = new RooRealVar(TString::Format("I_bg_%d", i + 1), "background_fraction", bgFraction[i]); //, bgFraction / 100, bgFraction*10);
	}

	// ADD BACKGROUND PDF
	RooAddPdf** decay_model_with_source_bg = new RooAddPdf*[iNumberOfFiles];
	for (unsigned i = 0; i < iNumberOfFiles; i++){
		decay_model_with_source_bg[i] = new RooAddPdf(TString::Format("Fit_%d", i + 1), "Decay Model with Source Contribution and Background", RooArgList(*bg[i], *decay_model_with_source[i]), *I_bg[i]);
	}

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
	___________.__  __                         __
	\_   _____/|__|/  |_  _____   ____   _____/  |_
	|    __)  |  \   __\/     \_/ __ \ /    \   __\
	|     \   |  ||  | |  Y Y  \  ___/|   |  \  |
	\___  /   |__||__| |__|_|  /\___  >___|  /__|
	\/                   \/     \/     \/
	__________
	\______   \_______  ____   ____  ____   ____   ______ ______
	|     ___/\_  __ \/  _ \_/ ___\/ __ \_/ __ \ /  ___//  ___/
	|    |     |  | \(  <_> )  \__\  ___/\  ___/ \___ \ \___ \
	|____|     |__|   \____/ \___  >___  >\___  >____  >____  >
	\/    \/     \/     \/     \/

	*/
        
	// Save storage before fitting to create file with parameters 
        // in case   user doesnt want to wait till fitting ends
        storage->save();        
        
	// CASE: FIT SPECTRUMS SEPARATELY WITH ROOMINUIT

	// Create Category Types
	TString* sTypes = new TString[iNumberOfFiles];
	for (unsigned i = 0; i < iNumberOfFiles; i++){
		sTypes[i] = TString::Format("spec%d", i);
	}

	// Define category with spectra names
	RooCategory* sample = new RooCategory("sample", "sample");
	sample->clearTypes();
	for (unsigned i = 0; i < iNumberOfFiles; i++){
		sample->defineType(sTypes[i].Data());
	}

	// Use stl::map to keep spectra pointers
	dhistMap myHistMap;
	for (unsigned i = 0; i<iNumberOfFiles; i++){
		std::string type = sTypes[i].Data();
		myHistMap.insert(dhistPair(type, histSpectrum[i]));
	}

	// Construct combined dataset in (x,sample)
	std::cout << "Constructing combined dataset" << std::endl;

	// Commented out because of the exception in libRoofitCore.dll in MSVC
	// RooDataHist* combData = new RooDataHist("combData", "combined data", *rChannels, *sample, myHistMap);

	// Following code is a workaround
	RooDataHist* combData = new RooDataHist();

	if (iNumberOfFiles == 1)      combData = new RooDataHist("combData", "combined data", *rChannels, Index(*sample), Import("spec0", *histSpectrum[0]));
	else if (iNumberOfFiles == 2) combData = new RooDataHist("combData", "combined data", *rChannels, Index(*sample), Import("spec0", *histSpectrum[0]), Import("spec1", *histSpectrum[1]));
	else if (iNumberOfFiles == 3) combData = new RooDataHist("combData", "combined data", *rChannels, Index(*sample), Import("spec0", *histSpectrum[0]), Import("spec1", *histSpectrum[1]), Import("spec2", *histSpectrum[2]));
	else if (iNumberOfFiles == 4) combData = new RooDataHist("combData", "combined data", *rChannels, Index(*sample), Import("spec0", *histSpectrum[0]), Import("spec1", *histSpectrum[1]), Import("spec2", *histSpectrum[2]), Import("spec3", *histSpectrum[3]));
	else if (iNumberOfFiles == 5) combData = new RooDataHist("combData", "combined data", *rChannels, Index(*sample), Import("spec0", *histSpectrum[0]), Import("spec1", *histSpectrum[1]), Import("spec2", *histSpectrum[2]), Import("spec3", *histSpectrum[3]), Import("spec4", *histSpectrum[4]));
	else if (iNumberOfFiles == 6) combData = new RooDataHist("combData", "combined data", *rChannels, Index(*sample), Import("spec0", *histSpectrum[0]), Import("spec1", *histSpectrum[1]), Import("spec2", *histSpectrum[2]), Import("spec3", *histSpectrum[3]), Import("spec4", *histSpectrum[4]), Import("spec5", *histSpectrum[5]));
	else if (iNumberOfFiles == 7) combData = new RooDataHist("combData", "combined data", *rChannels, Index(*sample), Import("spec0", *histSpectrum[0]), Import("spec1", *histSpectrum[1]), Import("spec2", *histSpectrum[2]), Import("spec3", *histSpectrum[3]), Import("spec4", *histSpectrum[4]), Import("spec5", *histSpectrum[5]), Import("spec6", *histSpectrum[6]));
	else exit(1);

	combData->Print();
	RooSimultaneous* simPdf = new RooSimultaneous("simPdf", "Simultaneous PDF", *sample);

	// Associate models and states
	for (unsigned i = 0; i < iNumberOfFiles; i++){
		simPdf->addPdf(*decay_model_with_source_bg[i], sTypes[i].Data());
	}

	// simPdf->fitTo(*combData) ;
	// simPdf->fitTo(*combData, NumCPU(NUM_CPU));

	// Use RooMinuit interface to minimize chi^2
	RooChi2Var* simChi2 = new RooChi2Var("simChi2", "chi2", *simPdf, *combData); // , NumCPU(NUM_CPU) , DataError(RooAbsData::None));
	RooMinuit m(*simChi2);
	m.migrad();
//	m.improve();
//	m.hesse();

	// m.save();
	// m.fit("r");

	RooArgSet* simFloatPars = simPdf->getParameters(*combData);
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

		histSpectrum[i]->plotOn(graphFrame[i], LineStyle(kSolid), LineColor(kBlack), LineWidth(1), MarkerSize(0.5), MarkerColor(kBlack));

                // Draw Resolution Function sumed with Background
                RooRealVar* bgFractionReal = new RooRealVar(TString::Format("bg_fraction_real_%d", i+1), "", bgFraction[i]);
                RooAddPdf* resFuncPlusBg = new RooAddPdf(TString::Format("res_funct_with_bg_%d", i+1), TString::Format("res_funct_with_bg_%d", i+1), RooArgList(*bg[i], *res_funct[i]), RooArgList(*bgFractionReal), kTRUE);
		resFuncPlusBg->plotOn(graphFrame[i], LineStyle(3), LineColor(kGray + 3), LineWidth(1));
//    		res_funct[i]->plotOn(graphFrame[i], LineStyle(3), LineColor(kGray + 3), LineWidth(1));
                
                // Draw complete fit
		decay_model_with_source_bg[i]->plotOn(graphFrame[i], LineStyle(kSolid), LineColor(kPink - 4), LineWidth(2));
                std::string legendLabel = constants->getDecayModel() + " model parameters";
		decay_model_with_source_bg[i]->paramOn(graphFrame[i], Layout(0.6), Format("NEU", AutoPrecision(1)), ShowConstants(kTRUE), Label(legendLabel.c_str()));// Parameters(decay_model_with_source_bg[i] -> getParameters(histSpectrum[i]);
		// test_model[i]->plotOn(graphFrame[i], LineStyle(kSolid), LineColor(kPink-4), LineWidth(2));
		// test_model[i]->paramOn(graphFrame[i], Layout(0.6), Format("NEU", AutoPrecision(1)));// Parameters(decay_model_with_source_bg[i] -> getParameters(histSpectrum[i]);
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
		hresid[i] = graphFrame[i]->pullHist();
		chiFrame[i] = rChannels->frame(Title(TString::Format("Goodness of fit: chi^2 = %.1f / %d = %.3f", chi2[i]->getVal(), n_Degree[i], chi2Value[i])));
		chiFrame[i]->addPlotable(hresid[i], "BX");
		chiFrame[i]->GetXaxis()->SetRangeUser(0, MAX_CHANNEL-MIN_CHANNEL+1);
		// histSpectrum[i]->statOn(chiFrame[i]);
	}
	std::cout << "Chi2 Frames Created OK!" << std::endl;


	// POPUP CANVAS WITH BOTH FRAMES
	TCanvas** canvas = new TCanvas*[iNumberOfFiles];
	for (unsigned i = 0; i<iNumberOfFiles; i++){
            canvas[i] = new TCanvas(TString::Format("canvas-%d", i + 1), TString::Format("Spectrum N%d \"%s\"", i + 1, sFileNames[i].c_str()), constants->getImageWidth(), constants->getImageHeight());
            canvas[i]->SetFillColor(0);
            canvas[i]->Divide(1, 2);
            canvas[i]->cd(1);
            canvas[i]->cd(1)->SetLogy();
            graphFrame[i]->GetYaxis()->SetRangeUser(1, iUpperLimit[i]);
            graphFrame[i]->Draw();
            canvas[i]->cd(2);
            chiFrame[i]->Draw();
            if (!isRoot){
                canvas[i]->Modified();
                canvas[i]->Update();                
//              gSystem->ProcessEvents();
            }
            TString imageFilename = TString::Format("%sfit-%s-%d.png", sOutputPath.c_str(), (constants->getDecayModel()).c_str(), i+1);
            fileUtils->saveImage(canvas[i], imageFilename.Data());
	}
       
	std::cout << "Current date and time is " << getCurrentTime() << std::endl;

	return 1;
}


int main(int argc, char* argv[]){
//    If need to use gdb - use this timeout and -O2 compiler flag
//    int seconds = 3;
//    usleep(seconds*1E6);
    // If we run compiled executable we need to instantinate TApplication manually
    TApplication* theApp = new TApplication("Positron Fit", &argc, argv);
    run(theApp);
    // Otherwise it crashes
    theApp->Run();            
    return 1;
}

void positronfit(){
    // If running app like `root -l positronfit.c` the instance of gApplication is created
    run(gApplication, kTRUE);
}