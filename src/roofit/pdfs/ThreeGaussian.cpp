/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ThreeGaussian.cpp
 * Author: petrstepanov
 * 
 * Created on November 9, 2016, 7:42 PM
 */

#include "ThreeGaussian.h"

ClassImp(ThreeGaussian);

ThreeGaussian::ThreeGaussian(const char *name, const char *title, RooAbsReal& _x, RooAbsReal& _mean, RooAbsReal& _sigma1, RooAbsReal& _sigma2,
		RooAbsReal& _sigma3, RooAbsReal& _i2, RooAbsReal& _i3) :
		RooAbsPdf(name, title), x("x", "Observable", this, _x), mean("mean", "mean", this, _mean), sigma1("sigma1", "Width1", this, _sigma1), sigma2("sigma2",
				"Width2", this, _sigma2), sigma3("sigma3", "Width3", this, _sigma3), i2("i2", "I2", this, _i2), i3("i3", "I3", this, _i3) {
}

ThreeGaussian::ThreeGaussian(const ThreeGaussian& other, const char* name) :
		RooAbsPdf(other, name), x("x", this, other.x), mean("mean", this, other.mean), sigma1("sigma1", this, other.sigma1), sigma2("sigma2", this,
				other.sigma2), sigma3("sigma3", this, other.sigma3), i2("i2", this, other.i2), i3("i3", this, other.i3) {
}

// Here PDFs next to the coefficients should be normalized - so we return: gauss/(sqrt2pi*sigma)
Double_t ThreeGaussian::evaluate() const {
	Double_t arg = x - mean;
	Double_t sqrt2pi = sqrt(2 * TMath::Pi());

	Double_t gauss1 = exp(-arg * arg / sigma1 / sigma1 / 2);
	Double_t gauss2 = exp(-arg * arg / sigma2 / sigma2 / 2);
	Double_t gauss3 = exp(-arg * arg / sigma3 / sigma3 / 2);
	return (1. - i2 - i3) * gauss1 / (sqrt2pi * sigma1) + i2 * gauss2 / (sqrt2pi * sigma2) + i3 * gauss3 / (sqrt2pi * sigma3);
}

Double_t ThreeGaussian::indefiniteIntegral(Double_t y) const {
	Double_t sqrtPiOver2 = TMath::Sqrt(TMath::PiOver2());
	Double_t int1 = 0.5 * TMath::Erf((y - mean) / (sigma1 * TMath::Sqrt2()));
	Double_t int2 = 0.5 * TMath::Erf((y - mean) / (sigma2 * TMath::Sqrt2()));
	Double_t int3 = 0.5 * TMath::Erf((y - mean) / (sigma3 * TMath::Sqrt2()));
	return (1. - i2 - i3) * int1 + i2 * int2 + i3 * int3;
}

// Get analytical integral -- important to return 0 and 1!
Int_t ThreeGaussian::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* rangeName) const {
	if (matchArgs(allVars, analVars, x))
		return 1;
	return 0;
}

// Analytical integral
Double_t ThreeGaussian::analyticalIntegral(Int_t code, const char* rangeName) const {
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
