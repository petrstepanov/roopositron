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
#include "ParamStorage.h"
#include "TwoGaussian.h"
#include "ThreeGaussian.h"
#include "ExpPdf.h"
#include "TwoExpPdf.h"
#include "ThreeExpPdf.h"
#include "TrapPdf.h"
#include "GrainPdf.h"
#include "FileUtils.h"
#include "Constants.h"
//#include "MyPdfCache.h"
//#include "MyPdf.h"
#include "ConvPdf.h"
#include "ChannelConvolutionPdf.h"
//#include <unistd.h>
#include <iostream>
#include <sstream>

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
    const Int_t wingBins = 20;
    Double_t fullInt = hist->Integral(1, nBins);
    Double_t leftWingAverage = (hist->Integral(1, wingBins)) / (Double_t)(wingBins);
//    Double_t rightWingAverage = (hist->Integral(nBins - wingBins, nBins)) / (Double_t)(wingBins);
//    Double_t bgInt = nBins * rightWingAverage;
    Double_t bgInt = nBins * leftWingAverage;
    return bgInt / fullInt;
}

TSystem* tSystem = NULL;
Int_t getNumCpu(){
    // Get number of CPUs
    if (tSystem == NULL) tSystem = new TUnixSystem();
    SysInfo_t sysInfo;
    tSystem->GetSysInfo(&sysInfo);
    std::cout << "NumCpu: " << sysInfo.fCpus << std::endl;
    return (sysInfo.fCpus >= 0) ? sysInfo.fCpus : 1;
}

// ASCII font generator is here: http://patorjk.com/software/taag/#p=display&f=Graffiti&t=Resolution%0AFunction
int run(int argc, char* argv[], Bool_t isRoot = kFALSE){
    // Output current time
	// std::cout << "Current date and time is " << getCurrentTime() << std::endl;

        TStopwatch watch;
        watch.Start();    
    
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
        
        if (constants->isNew()){
            std::cout << "Constants file `constants.txt` created. Make sure values are ok. Press CTRL+C." << std::endl;
            return 0;            
        }
        
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

                std::cout << "  Total number of events:" << fullTH1F[i]->Integral() << std::endl;                
                
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
	RooConstVar* channelWidth = new RooConstVar("channelWidth", "Bin channel width, ns", CHANNEL_WIDTH);
        
	// Convert TH1F spectra to RooDataHist
	RooDataHist** histSpectrum = new RooDataHist*[iNumberOfFiles];
	for (unsigned i = 0; i < iNumberOfFiles; i++){
            histSpectrum[i] = new RooDataHist(TString::Format("histSpectrum_%d", i + 1), sFileNames[i].c_str(), RooArgSet(*rChannels), RooFit::Import(*fullTH1F[i]));
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

	// Read resolution function parameters from file
        // parameters filename is "parameters_XXX.txt", where XXX is model name (1exp, 2exp etc)
        std::string suffix = constants->getDecayModel() + "_" + constants->getResolutionFunctionModel();
	ParamStorage* storage = new ParamStorage(suffix);
        
	RooConstVar* fwhm2disp = new RooConstVar("fwhm2disp", "Coefficient to convert fwhm to dispersion", 1./(2.*sqrt(2.*log(2.))));

	// Zero channel value is relative to the MIN_CHANNEL value
	RooRealVar** zero_ch = new RooRealVar*[iNumberOfFiles];
	for (unsigned i = 0; i < iNumberOfFiles; i++){
            Double_t zeroChannelVal = iTopChannel[i];
            Double_t zeroChannelDelta = 50;
            TString name = TString::Format("zero_ch_%d", i + 1);
            TString description = TString::Format("zero_channel_%d", i + 1);
            // Don't store zero_channel in parameters
//            zero_ch[i] = storage->getOrMakeNew(name, description, zeroChannelVal, zeroChannelVal - zeroChannelDelta, zeroChannelVal + zeroChannelDelta, "ch");
            zero_ch[i] = new RooRealVar(name, description, zeroChannelVal, zeroChannelVal - zeroChannelDelta, zeroChannelVal + zeroChannelDelta, "ch");
	}

	// 1st Gauss FWHM
	RooRealVar* g1_fwhm = storage->getOrMakeNew("g1_fwhm", "1st_gauss_fwhm", 0.3, 0.1, 1.0, "ns");
	RooFormulaVar* gauss_1_dispersion = new RooFormulaVar("gauss_1_dispersion", "@0*@1/@2", RooArgList(*g1_fwhm, *fwhm2disp, *channelWidth));

	// 2nd gauss FWHM
	RooRealVar* g2_fwhm = storage->getOrMakeNew("g2_fwhm", "2nd_gauss_fwhm", 0.7, 0.1, 1.5, "ns");
	RooFormulaVar* gauss_2_dispersion = new RooFormulaVar("gauss_2_dispersion", "@0*@1/@2", RooArgList(*g2_fwhm, *fwhm2disp, *channelWidth));

       	// Fraction of the 2nd gauss
	RooRealVar* gauss_2_fraction_pct = storage->getOrMakeNew("g2_frac", "2nd_gauss_fraction", 10, 0, 100, "%");
	RooFormulaVar* gauss_2_fraction = new RooFormulaVar("g2_fraction", "@0/100", *gauss_2_fraction_pct);

	// 2nd Gauss shift
//	RooRealVar** g2_shift = new RooRealVar*[iNumberOfFiles];
//	RooFormulaVar** zero_ch_relative_2 = new RooFormulaVar*[iNumberOfFiles];
//	for (unsigned i = 0; i < iNumberOfFiles; i++) {
//		g2_shift[i] = storage->getOrMakeNew(TString::Format("g2_shift_%d", i + 1), TString::Format("2nd_gauss_shift_%d", i + 1), 0, -10, 10, "ch");
//		zero_ch_relative_2[i] = new RooFormulaVar(TString::Format("zero_ch_relative_2_%d", i + 1), "@0+@1", RooArgList(*zero_ch[i], *g2_shift[i]));
//	}

	// Two-Gauss PDF
	RooAbsPdf** res_funct = new RooAbsPdf*[iNumberOfFiles];
        
        if (constants->getResolutionFunctionModel() == "2gauss"){
            for (unsigned i = 0; i<iNumberOfFiles; i++){
//                res_funct[i] = new TwoGaussian(TString::Format("res_funct_%d", i + 1), "Resolution Function for Convolution (2 Gauss)", *rChannels, *zero_ch[i], *gauss_1_dispersion, *zero_ch_relative_2[i], *gauss_2_dispersion, *gauss_2_fraction);
                res_funct[i] = new TwoGaussian(TString::Format("res_funct_%d", i + 1), "Resolution Function for Convolution (2 Gauss)", *rChannels, *zero_ch[i], *gauss_1_dispersion, *zero_ch[i], *gauss_2_dispersion, *gauss_2_fraction);
            }
        } else {
            // 3rd gauss dispertion
            RooRealVar* g3_fwhm = storage->getOrMakeNew("g3_fwhm", "3rd_gauss_fwhm", 1.5, 1, 3, "ns");
            RooFormulaVar* gauss_3_dispersion = new RooFormulaVar("gauss_3_dispersion", "@0*@1/@2", RooArgList(*g3_fwhm, *fwhm2disp, *channelWidth));

            // Fraction of the 2nd gauss
            RooRealVar* gauss_3_fraction_pct = storage->getOrMakeNew("g3_frac", "3rd_gauss_fraction", 1, 0, 10, "%");
            RooFormulaVar* gauss_3_fraction = new RooFormulaVar("g3_fraction", "@0/100", *gauss_3_fraction_pct);
        
            for (unsigned i = 0; i<iNumberOfFiles; i++){
//                res_funct[i] = new ThreeGaussian(TString::Format("res_funct_%d", i + 1), "Resolution Function for Convolution (3 Gauss)", *rChannels, *zero_ch[i], *gauss_1_dispersion, *zero_ch_relative_2[i], *gauss_2_dispersion, *gauss_3_dispersion, *gauss_2_fraction, *gauss_3_fraction);
                res_funct[i] = new ThreeGaussian(TString::Format("res_funct_%d", i + 1), "Resolution Function for Convolution (3 Gauss)", *rChannels, *zero_ch[i], *gauss_1_dispersion, *zero_ch[i], *gauss_2_dispersion, *gauss_3_dispersion, *gauss_2_fraction, *gauss_3_fraction);
            }            
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
            RooRealVar* tau2 = storage->getOrMakeNew("tau2", "2nd_exponent_lifetime", 0.5, 0.2, 3.0, "ns");
            RooFormulaVar* tau2_ch = new RooFormulaVar("tau2_ch", "@0/@1", RooArgList(*tau2, *channelWidth));
            RooRealVar* I_tau2 = storage->getOrMakeNew("I_tau2", "2nd_exponent_fraction", 10, 0, 100, "%");
            RooFormulaVar* I_tau2_ = new RooFormulaVar("I_tau2_", "@0/100.", *I_tau2);
            decay_model = new TwoExpPdf("decay_model", "decay_model", *rChannels, *tau1_ch, *tau2_ch, *I_tau2_);        
        }
        // Two-exponential model
        else if(constants->getDecayModel() == "3exp"){
            RooRealVar* tau1 = storage->getOrMakeNew("tau1", "1st_exponent_lifetime", 0.15, 0.1, 0.7, "ns");
            RooFormulaVar* tau1_ch = new RooFormulaVar("tau1_ch", "@0/@1", RooArgList(*tau1, *channelWidth));
            RooRealVar* tau2 = storage->getOrMakeNew("tau2", "2nd_exponent_lifetime", 0.5, 0.2, 3.0, "ns");
            RooFormulaVar* tau2_ch = new RooFormulaVar("tau2_ch", "@0/@1", RooArgList(*tau2, *channelWidth));
            RooRealVar* tau3 = storage->getOrMakeNew("tau3", "3rd_exponent_lifetime", 1, 0.5, 15, "ns");
            RooFormulaVar* tau3_ch = new RooFormulaVar("tau3_ch", "@0/@1", RooArgList(*tau3, *channelWidth));
            RooRealVar* I_tau2 = storage->getOrMakeNew("I_tau2", "2nd_exponent_fraction", 10, 0, 100, "%");
            RooFormulaVar* I_tau2_ = new RooFormulaVar("I_tau2_", "@0/100.", *I_tau2);
            RooRealVar* I_tau3 = storage->getOrMakeNew("I_tau3", "3rd_exponent_fraction", 2, 0, 100, "%");
            RooFormulaVar* I_tau3_ = new RooFormulaVar("I_tau3_", "@0/100.", *I_tau3);
            decay_model = new ThreeExpPdf("decay_model", "decay_model", *rChannels, *tau1_ch, *tau2_ch, *tau3_ch, *I_tau2_, *I_tau3_);
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
//            MyPdfCache* myPdfCache = new MyPdfCache(100, MIN_CHANNEL, MAX_CHANNEL);
//            MyPdf::myPdfCache = myPdfCache;

//            decay_model = new MyPdf("decay_model", "decay_model", *rChannels, *lambdaJ_ch, *lambdaBulk_ch, *lambdaGrain_ch, *lambdaVac_ch, *kappaVac_ch);
            // add cache to model - counts 10x faster
            
            decay_model = new GrainPdf("decay_model", "decay_model", *rChannels, *lambdaJ_ch, *lambdaBulk_ch, *lambdaGrain_ch, *lambdaVac_ch, *kappaVac_ch);            
        }
        
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
        
        RooAddPdf* decay_model_sum = new RooAddPdf("decay_model_sum", "decay_model_sum", RooArgList(*decay_source, *decay_model), *I_source_, kTRUE);        

	RooFFTConvPdf** decay_model_with_source = new RooFFTConvPdf*[iNumberOfFiles];
//	rChannels->setBins(constants->getConvolutionBins(), "cache");
        rChannels->setBins(MAX_CHANNEL - MIN_CHANNEL + 1, "cache");
	for (unsigned i = 0; i<iNumberOfFiles; i++){
            decay_model_with_source[i] = new RooFFTConvPdf(TString::Format("decay_model_with_source_%d", i + 1), TString::Format("Grain Boundary Model N%d", i + 1), *rChannels, *decay_model_sum, *res_funct[i]);
	}

//      RooNumConvPdf** decay_model_with_source = new RooNumConvPdf*[iNumberOfFiles];
//      for (unsigned i = 0; i<iNumberOfFiles; i++){
//          decay_model_with_source[i] = new RooNumConvPdf(TString::Format("decay_model_with_source_%d", i + 1), TString::Format("Convoluted model N%d", i + 1), *rChannels, *decay_model_sum, *res_funct[i]);
////        ((RooNumConvPdf*) decay_model_with_source[i])->setConvolutionWindow(*zero_ch[i],*g1_fwhm,2);
//      }  
        
//        rChannels->setBins(constants->getConvolutionBins(), "cache");    
//        rChannels->setBins(MAX_CHANNEL - MIN_CHANNEL + 1, "cache");        
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
		bg[i] = new RooPolynomial(TString::Format("bg_%d", i + 1), "y=1", *rChannels, RooArgSet());
		// bg[i] = new RooPolynomial(TString::Format("bg_%d", i + 1), "Background Polynom", *rChannels);
	}

	// Background fractions array
	RooConstVar** I_bg = new RooConstVar*[iNumberOfFiles];
        Double_t* bgFraction = new Double_t[iNumberOfFiles];
	for (unsigned i = 0; i < iNumberOfFiles; i++){
		bgFraction[i] = getConstBackgroundFraction(fullTH1F[i]);
		I_bg[i] = new RooConstVar(TString::Format("I_bg_%d", i + 1), "background_fraction", bgFraction[i]); //, bgFraction[i]/2, bgFraction[i]*2);
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
	RooDataHist* combData; // = new RooDataHist();

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
        RooChi2Var* simChi2;
//        Int_t EXCLUDE_MIN_CHANNEL = constants->getExcludeMinChannel();
//        Int_t EXCLUDE_MAX_CHANNEL = constants->getExcludeMaxChannel();
//        Bool_t doRange = kFALSE;
//        if (EXCLUDE_MIN_CHANNEL > 1 && EXCLUDE_MIN_CHANNEL < EXCLUDE_MAX_CHANNEL && EXCLUDE_MAX_CHANNEL < MAX_CHANNEL - MIN_CHANNEL){
//            doRange = kTRUE;
//        }
        
//        if (doRange){
//            std::cout << "Doing Ranges!" << std::endl;
//            rChannels->setRange("LEFT", 1, EXCLUDE_MIN_CHANNEL);
//            rChannels->setRange("RIGHT", EXCLUDE_MAX_CHANNEL, rChannels->getBins());
//            simChi2 = new RooChi2Var("simChi2", "chi2", *simPdf, *combData, RooFit::Range("LEFT,RIGHT"), NumCPU(getNumCpu()));
//        }
//        else {
            simChi2 = new RooChi2Var("simChi2", "chi2", *simPdf, *combData, NumCPU(getNumCpu()));
//        }                 

//	RooMinuit m(*simChi2);
//        m.optimizeConst(1);
//	m.migrad();
//	m.improve();
//	m.hesse();

        RooMinimizer* m = new RooMinimizer(*simChi2);
        // m->setStrategy(RooMinimizer::Speed);
        m->setMinimizerType("Minuit");
        Int_t resultMigrad = m->migrad();
        Int_t resultHesse = m->hesse();
        std::cout << "RooMinimizer: migrad=" << resultMigrad << ", hesse=" << resultHesse << std::endl;

        RooFitResult* fitResult = m->save();
        
        
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

            histSpectrum[i]->plotOn(graphFrame[i], LineStyle(kSolid), LineColor(kBlack), LineWidth(0), MarkerSize(0.2), MarkerColor(kBlack));

            // Draw Resolution Function summed with Background
//            RooRealVar* bgFractionReal = new RooRealVar(TString::Format("bg_fraction_real_%d", i+1), "", bgFraction[i]);
//            RooAddPdf* resFuncPlusBg = new RooAddPdf(TString::Format("res_funct_with_bg_%d", i+1), TString::Format("res_funct_with_bg_%d", i+1), RooArgList(*bg[i], *res_funct[i]), RooArgList(*bgFractionReal));// , kTRUE);
//            resFuncPlusBg->plotOn(graphFrame[i], LineStyle(3), LineColor(kGray + 3), LineWidth(1), Name("resolution"));

            // Draw Resolution Function
            res_funct[i]->plotOn(graphFrame[i], LineStyle(3), LineColor(kGray + 3), LineWidth(1), Name("resolution"));
            
            // Draw complete fit
            decay_model_with_source_bg[i]->plotOn(graphFrame[i], LineStyle(kSolid), LineColor(kPink - 4), LineWidth(2), Name("fit"));
//                std::string legendLabel = constants->getDecayModel() + " model parameters";
            decay_model_with_source_bg[i]->paramOn(graphFrame[i], Layout(0.78, 0.97, 0.9), Format("NEU", AutoPrecision(3)), ShowConstants(kTRUE));// , Label(legendLabel.c_str()) Parameters(decay_model_with_source_bg[i] -> getParameters(histSpectrum[i]);
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
                hresid[i]->SetLineWidth(0);
                hresid[i]->SetMarkerSize(0.2);
		chiFrame[i] = rChannels->frame(Title(" ")); //Title(TString::Format("Goodness of fit: chi^2 = %.1f / %d = %.3f", chi2[i]->getVal(), n_Degree[i], chi2Value[i])));
//                ((RooDataHist*)hresid[i])->plotOn(chiFrame[i],DataError(RooAbsData::None));
                chiFrame[i]->addPlotable(hresid[i]);
		chiFrame[i]->GetXaxis()->SetRangeUser(0, MAX_CHANNEL-MIN_CHANNEL+1);

                // Write RooDataHist statistics on chi frame (to create TPaveStats object)
		// histSpectrum[i]->statOn(chiFrame[i]);                
	}
	std::cout << "Chi2 Frames Created OK!" << std::endl;


	// POPUP CANVAS WITH BOTH FRAMES
	TCanvas** canvas = new TCanvas*[iNumberOfFiles];
	for (unsigned i = 0; i<iNumberOfFiles; i++){
            canvas[i] = new TCanvas(TString::Format("canvas-%d", i + 1), TString::Format("Spectrum N%d \"%s\"", i + 1, sFileNames[i].c_str()), constants->getImageWidth(), constants->getImageHeight());
            canvas[i]->SetFillColor(0);
            canvas[i]->Divide(1, 2);
//            canvas[i]->cd(1)->SetPad(0, 0.4, 1, 1); // xlow ylow xup yup
            canvas[i]->cd(1)->SetMargin(0.08, 0.03, 0.05, 0.1); // left right bottom top
            canvas[i]->cd(1)->SetLogy();
            Double_t fontSize = 0.06;
            graphFrame[i]->GetXaxis()->SetLabelColor(0);
            graphFrame[i]->GetXaxis()->SetTitleSize(0);
            graphFrame[i]->GetYaxis()->SetLabelSize(fontSize - 0.01);
            graphFrame[i]->GetYaxis()->SetRangeUser(1, iUpperLimit[i]);
            graphFrame[i]->GetYaxis()->SetTitleSize(fontSize);             
            graphFrame[i]->GetYaxis()->SetTitleOffset(0.5);  
            graphFrame[i]->Draw();

//            canvas[i]->cd(2)->SetPad(0, 0, 1, 0.4);
            canvas[i]->cd(2)->SetMargin(0.08, 0.03, 0.3, 0.05); // left right bottom top - margin for bottom title space

            chiFrame[i]->GetXaxis()->SetLabelSize(fontSize - 0.01);
            chiFrame[i]->GetYaxis()->SetLabelSize(fontSize - 0.01);            
            chiFrame[i]->GetYaxis()->SetLabelOffset(0.01);            
            chiFrame[i]->GetXaxis()->SetLabelOffset(0.04);  
            chiFrame[i]->GetXaxis()->SetTitleSize(fontSize); 
            chiFrame[i]->GetXaxis()->SetTitleOffset(2); 

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

            TString imageFilename = TString::Format("%sfit-%s-%s-%d.png", 
                    sOutputPath.c_str(), 
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
        TString dataFilename = TString::Format("%sdata-%s-%s-%d.txt", 
                sOutputPath.c_str(), 
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
            hresid[i]->GetPoint(j-MIN_CHANNEL+1, channel, chi);
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

    watch.Stop();
    watch.Print();
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

void positronfit(){
    // If running app like `root -l positronfit.c` the instance of gApplication is created
    run(gApplication->Argc(), gApplication->Argv(), kTRUE);
}