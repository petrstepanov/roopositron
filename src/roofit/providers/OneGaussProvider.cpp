/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   OneGaussProvider.cpp
 * Author: petrstepanov
 * 
 * Created on December 6, 2018, 5:10 AM
 */

#include "OneGaussProvider.h"
#include "RooFormulaVar.h"
#include "RooGaussian.h"
#include "../../roofit/AdditiveConvolutionPdf.h"
#include "../../model/Constants.h"

OneGaussProvider::OneGaussProvider(RooRealVar* _observable, RooRealVar* _channelWidth) : AbstractProvider(_observable, _channelWidth) {}
    
OneGaussProvider::~OneGaussProvider() {
}

RooAbsPdf* OneGaussProvider::initPdf(int i) {
    RooConstVar* fwhm2disp = Constants::fwhm2disp;

    // 1st Gauss FWHM
    RooRealVar* g1FWHM = new RooRealVar("FWHM_gauss1", "1st gauss FWHM", 0.3, 0.1, 0.5, "ns");
    RooFormulaVar* g1Dispersion = new RooFormulaVar("gauss1Dispersion", "@0*@1/@2", RooArgList(*g1FWHM, *fwhm2disp, *channelWidth));

    RooRealVar* gMean = new RooRealVar(AdditiveConvolutionPdf::VAR_MEAN_GAUSS_NAME, "Resolution function mean", 1, "ch");
    return new RooGaussian("pdfResolutionGauss","Resolution function", *observable, *gMean, *g1Dispersion) ;
	
}
