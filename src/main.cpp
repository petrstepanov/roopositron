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
    TH1F* histogram;     // root histogram
    Int_t counts;        // total counts
    Int_t minimumBin;    // bin number with minimum event count
    Int_t maximumBin;    // bin number with maximum event count
    Int_t minimumCount;  // minimum count across all bins
    Int_t maximumCount;  // maximum count across all bins
};

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

        // Get spectra filenames
        FileUtils* fileUtils = new FileUtils();
	
	// TODO: move directory structure fetching to FileUtils
	std::string sInputPath="";
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

//	// Y axis plotting range
//	Int_t* iUpperLimit = new Int_t[iNumberOfFiles];
//	Int_t* iLowerLimit = new Int_t[iNumberOfFiles];

        // Constants object reads values from "constants.txt" file
        Constants* constants = Constants::getInstance();
	
	// TODO: drop these variables in favor of histogram struct
	
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
