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
#include "TMath.h"

ClassImp(TwoGaussian);

TwoGaussian::TwoGaussian(const char *name, const char *title, RooAbsReal& _x, RooAbsReal& _mean, RooAbsReal& _sigma1, RooAbsReal& _sigma2, RooAbsReal& _i2) :
		RooAbsPdf(name, title), x("x", "Observable", this, _x), mean("mean", "Mean", this, _mean), sigma1("sigma1", "Width1", this, _sigma1), sigma2("sigma2",
				"Width2", this, _sigma2), i2("i2", "I1", this, _i2) {
}

TwoGaussian::TwoGaussian(const TwoGaussian& other, const char* name) :
		RooAbsPdf(other, name), x("x", this, other.x), mean("mean", this, other.mean), sigma1("sigma1", this, other.sigma1), sigma2("sigma2", this,
				other.sigma2), i2("i2", this, other.i2) {
}

// Here PDFs next to the coefficients should be normalized - so we return: gauss/(sqrt2pi*sigma)
Double_t TwoGaussian::evaluate() const {
	Double_t arg = x - mean;
	Double_t sqrt2pi = sqrt(2 * TMath::Pi());

	Double_t gauss1 = exp(-arg * arg / sigma1 / sigma1 / 2);
	Double_t gauss2 = exp(-arg * arg / sigma2 / sigma2 / 2);
	return (1. - i2) * gauss1 / (sqrt2pi * sigma1) + i2 * gauss2 / (sqrt2pi * sigma2);
}

// Indefinite integral (see "gauss.nb")
Double_t TwoGaussian::indefiniteIntegral(Double_t y) const {
	Double_t sqrtPiOver2 = TMath::Sqrt(TMath::PiOver2());
	Double_t int1 = 0.5 * TMath::Erf((y - mean) / (sigma1 * TMath::Sqrt2()));
	Double_t int2 = 0.5 * TMath::Erf((y - mean) / (sigma2 * TMath::Sqrt2()));
	return (1. - i2) * int1 + i2 * int2;
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

	if (code == 1) {
		Double_t x1 = x.min(rangeName);
		Double_t x2 = x.max(rangeName);
		if (x2 <= 0)
			return 0;
		x1 = TMath::Max(0., x1);
		return indefiniteIntegral(x2) - indefiniteIntegral(x1);
	}
	std::cout << "Error in TwoGaussian::analyticalIntegral" << std::endl;
	return 0;
}
