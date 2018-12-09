/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ThreeGaussProvider.cpp
 * Author: petrstepanov
 * 
 * Created on December 6, 2018, 10:48 AM
 */

#include "ThreeGaussProvider.h"
#include "RooFormulaVar.h"
#include "../pdfs/ThreeGaussian.h"
#include "../../model/Constants.h"

ThreeGaussProvider::ThreeGaussProvider(RooRealVar* observable) : AbstractProvider(observable) {}
    
ThreeGaussProvider::~ThreeGaussProvider() {
}

RooAbsPdf* ThreeGaussProvider::initPdf() {
    RooConstVar* fwhm2disp = Constants::fwhm2disp;
    RooConstVar* channelWidth = Constants::getInstance()->getRooChannelWidth();

    // 1st Gauss FWHM
    RooRealVar* g1FWHM = new RooRealVar("gauss1FWHM", "1st gauss FWHM", 0.3, 0.1, 0.5, "ns");
    RooFormulaVar* g1Dispersion = new RooFormulaVar("gauss1Dispersion", "@0*@1/@2", RooArgList(*g1FWHM, *fwhm2disp, *channelWidth));

    // 2nd gauss FWHM
    RooRealVar* g2FWHM = new RooRealVar("gauss2FWHM", "2nd gauss FWHM", 0.7, 0.4, 2.0, "ns");
    RooFormulaVar* g2Dispersion = new RooFormulaVar("gauss2Dispersion", "@0*@1/@2", RooArgList(*g2FWHM, *fwhm2disp, *channelWidth));

    // Fraction of the 2nd gauss
    RooRealVar* g2FractionPct = new RooRealVar("gauss2Frac", "2nd gauss fraction", 1, 0, 10, "%");
    RooFormulaVar* g2Fraction = new RooFormulaVar("gauss2Frac", "@0/100", *g2FractionPct);

    // 2nd Gauss shift
//	RooRealVar** g2_shift = new RooRealVar*[iNumberOfFiles];
//	RooFormulaVar** zero_ch_relative_2 = new RooFormulaVar*[iNumberOfFiles];
//	for (unsigned i = 0; i < iNumberOfFiles; i++) {
//		g2_shift[i] = storage->getOrMakeNew(TString::Format("g2_shift_%d", i + 1), TString::Format("2nd_gauss_shift_%d", i + 1), 0, -10, 10, "ch");
//		zero_ch_relative_2[i] = new RooFormulaVar(TString::Format("zero_ch_relative_2_%d", i + 1), "@0+@1", RooArgList(*zero_ch[i], *g2_shift[i]));
//	}

    // 3rd gauss dispertion
    RooRealVar* g3FWHM = new RooRealVar("g3FWHM", "3rd gauss FWHM", 1.5, 0.8, 3, "ns");
    RooFormulaVar* g3Dispersion = new RooFormulaVar("gauss3Dispersion", "@0*@1/@2", RooArgList(*g3FWHM, *fwhm2disp, *channelWidth));

    // Fraction of the 2nd gauss
    RooRealVar* g3FractionPct = new RooRealVar("gauss3Frac", "3rd gauss fraction", 1, 0, 10, "%");
    RooFormulaVar* g3Fraction = new RooFormulaVar("gauss3Frac", "@0/100", *g3FractionPct);

    // Zero channel start values are assigned later (relative to the MIN_CHANNEL value)
    RooRealVar* gMean = new RooRealVar("gaussMean", "Resolution function mean", 300, 250, 350, "ch");
	
    return new ThreeGaussian("threeGauss", "Three gauss model", *observable, *gMean, *g1Dispersion, *gMean, *g2Dispersion, *g3Dispersion, *g2Fraction, *g3Fraction);	
}
