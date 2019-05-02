/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TwoGaussProvider.cpp
 * Author: petrstepanov
 * 
 * Created on December 6, 2018, 5:10 AM
 */

#include "TwoGaussProvider.h"
#include "RooFormulaVar.h"
#include "RooGaussian.h"
#include "RooAddPdf.h"
#include "../pdfs/TwoGaussian.h"
#include "../../model/Constants.h"

TwoGaussProvider::TwoGaussProvider(RooRealVar* _observable, RooRealVar* _channelWidth) : AbstractProvider(_observable, _channelWidth) {}
    
TwoGaussProvider::~TwoGaussProvider() {
}

RooAbsPdf* TwoGaussProvider::initPdf(int i) {
    RooConstVar* fwhm2disp = Constants::fwhm2disp;
//    RooConstVar* channelWidth = Constants::getInstance()->getRooChannelWidth();

    // 1st Gauss FWHM
    RooRealVar* g1FWHM = new RooRealVar("FWHM_gauss1", "1st gauss FWHM", 0.3, 0.1, 0.5, "ns");
    RooFormulaVar* g1Dispersion = new RooFormulaVar("gauss1Dispersion", "@0*@1/@2", RooArgList(*g1FWHM, *fwhm2disp, *channelWidth));

    // 2nd gauss FWHM
    RooRealVar* g2FWHM = new RooRealVar("FWHM_gauss2", "2nd gauss FWHM", 0.8, 0.4, 2.0, "ns");
    RooFormulaVar* g2Dispersion = new RooFormulaVar("gauss2Dispersion", "@0*@1/@2", RooArgList(*g2FWHM, *fwhm2disp, *channelWidth));

    // Fraction of the 2nd gauss
    RooRealVar* g2Fraction = new RooRealVar("Int_gauss2", "2nd gauss fraction", 1, 0, 20, "%");
    RooFormulaVar* g2FractionNorm = new RooFormulaVar("gauss2FracNorm", "@0/100", *g2Fraction);

    // 2nd Gauss shift
//	RooRealVar** g2_shift = new RooRealVar*[iNumberOfFiles];
//	RooFormulaVar** zero_ch_relative_2 = new RooFormulaVar*[iNumberOfFiles];
//	for (unsigned i = 0; i < iNumberOfFiles; i++) {
//		g2_shift[i] = storage->getOrMakeNew(TString::Format("g2_shift_%d", i + 1), TString::Format("2nd_gauss_shift_%d", i + 1), 0, -10, 10, "ch");
//		zero_ch_relative_2[i] = new RooFormulaVar(TString::Format("zero_ch_relative_2_%d", i + 1), "@0+@1", RooArgList(*zero_ch[i], *g2_shift[i]));
//	}

    // Zero channel start values are assigned later (relative to the MIN_CHANNEL value)
    RooRealVar* gMean = new RooRealVar("mean_gauss", "Resolution function mean", 1, "ch");

//    RooGaussian* gauss1 = new RooGaussian("gauss1", "1st gauss", *observable, *gMean, *g1Dispersion);
//    RooGaussian* gauss2 = new RooGaussian("gauss2", "2nd gauss", *observable, *gMean, *g2Dispersion);

//    RooAddPdf* gauss = new RooAddPdf("twoGauss", "Two gauss model", RooArgList(*gauss2, *gauss1), RooArgList(*g2FractionNorm));
//    gauss->fixAddCoefNormalization(RooArgSet(*observable));
//	return gauss;

    // Works much faster with TwoGaussian rather than with sum of two RooGaussian
    return new TwoGaussian("twoGauss", "Two gauss model", *observable, *gMean, *g1Dispersion, *g2Dispersion, *g2FractionNorm);
}
