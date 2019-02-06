/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TwoGaussian.cpp
 * Author: petrstepanov
 * 
 * Created on November 9, 2016, 7:42 PM
 */

#include "TwoGaussian.h"

ClassImp(TwoGaussian);

TwoGaussian::TwoGaussian(const char *name, const char *title, RooAbsReal& _x, RooAbsReal& _mean, RooAbsReal& _sigma1, RooAbsReal& _sigma2, RooAbsReal& _i2) :
		RooAbsPdf(name, title), x("x", "Observable", this, _x), mean("mean", "Mean", this, _mean), sigma1("sigma1", "Width1", this, _sigma1), sigma2("sigma2", "Width2", this, _sigma2), i2("i2", "I1",
				this, _i2) {
}

TwoGaussian::TwoGaussian(const TwoGaussian& other, const char* name) :
		RooAbsPdf(other, name), x("x", this, other.x), mean("mean", this, other.mean), sigma1("sigma1", this, other.sigma1), sigma2("sigma2", this, other.sigma2), i2("i2", this, other.i2) {
}

// Evaluate without normalization
Double_t TwoGaussian::evaluate() const {
	Double_t arg = x - mean;

	Double_t ret1 = exp(-arg * arg / sigma1 / sigma1 / 2);
	Double_t ret2 = exp(-arg * arg / sigma2 / sigma2 / 2);
	return (1. - i2) * ret1 + i2 * ret2;
}

// Indefinite integral
Double_t TwoGaussian::indefiniteIntegral(Double_t y) const {
	Double_t sqrtPiOver2 = TMath::Sqrt(TMath::PiOver2());

	// see "gauss.nb"
	Double_t ret1 = sqrtPiOver2 * sigma1 * TMath::Erf((y - mean) / (sigma1 * TMath::Sqrt2()));
	Double_t ret2 = sqrtPiOver2 * sigma2 * TMath::Erf((y - mean) / (sigma2 * TMath::Sqrt2()));
	return (1. - i2) * ret1 + i2 * ret2;
}

// Get analytical integral -- important to return 0 and 1!
Int_t TwoGaussian::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* rangeName) const {
	if (matchArgs(allVars, analVars, x))
		return 1;
	return 0;
}

// Analytical integral (definite)
Double_t TwoGaussian::analyticalIntegral(Int_t code, const char* rangeName) const {
	assert(code == 1);

	Double_t ret = 0;
	if (code == 1) {
		Double_t x1 = x.min(rangeName);
		Double_t x2 = x.max(rangeName);

		ret = indefiniteIntegral(x2) - indefiniteIntegral(x1);
	} else {
		std::cout << "Error in TwoGaussian::analyticalIntegral" << std::endl;
	}
	return ret;
}
