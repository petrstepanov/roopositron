/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TwoExpPdf.cpp
 * Author: petrstepanov
 * 
 * Created on November 9, 2016, 7:52 PM
 */

#include "TwoExpPdf.h"

ClassImp(TwoExpPdf);

TwoExpPdf::TwoExpPdf(const char *name, const char *title, RooAbsReal& _t, RooAbsReal& _tau1, RooAbsReal& _tau2, RooAbsReal& _i2) :
		RooAbsPdf(name, title), t("t", "t", this, _t), tau1("tau1", "tau1", this, _tau1), tau2("tau2", "tau2", this, _tau2), i2("i2", "i2", this, _i2) {
}

TwoExpPdf::TwoExpPdf(const TwoExpPdf& other, const char* name) :
		RooAbsPdf(other, name), t("t", this, other.t), tau1("tau1", this, other.tau1), tau2("tau2", this, other.tau2), i2("i2", this, other.i2) {
}

// Evaluate without normalization
Double_t TwoExpPdf::evaluate() const {
	if (t < 0)
		return 0.;
	return (1. - i2) / tau1 * exp(-t / tau1) + i2 / tau2 * exp(-t / tau2);
}

// Indefinite integral
Double_t TwoExpPdf::indefiniteIntegral(Double_t y) const {
	if (y < 0)
		return 0.;
	Double_t int1 = -exp(-y / tau1);
	Double_t int2 = -exp(-y / tau2);
	return (1. - i2) * int1 + i2 * int2;
}

// Get analytical integral
Int_t TwoExpPdf::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* rangeName) const {
	if (matchArgs(allVars, analVars, t))
		return 1;
	return 0;
}

// Analytical integral
Double_t TwoExpPdf::analyticalIntegral(Int_t code, const char* rangeName) const {
	assert(code == 1);

	if (code == 1) {
		Double_t x1 = t.min(rangeName);
		Double_t x2 = t.max(rangeName);
		if (x2 <= 0)
			return 0;
		x1 = TMath::Max(0., x1);
		return indefiniteIntegral(x2) - indefiniteIntegral(x1);
	}
	std::cout << "Error in TwoExpPdf::analyticalIntegral" << std::endl;
	return 0;
}
