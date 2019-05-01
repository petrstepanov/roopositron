/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ConstantPdf.cpp
 * Author: petrstepanov
 * 
 * Created on November 9, 2016, 7:49 PM
 */

#include "ConstantPdf.h"
//#include <iostream>

ClassImp(ConstantPdf);

ConstantPdf::ConstantPdf(const char *name, const char *title, RooAbsReal& _t, RooAbsReal& _bins) :
		RooAbsPdf(name, title), t("t", "t", this, _t), bins("bins", "bins", this, _bins) {
}

ConstantPdf::ConstantPdf(const ConstantPdf& other, const char* name) :
		RooAbsPdf(other, name), t("t", this, other.t), bins("bins", this, other.bins) {
}

// Doing like rooexponential
Double_t ConstantPdf::evaluate() const {
	if (t < 0)
		return 0;
	if (t > bins)
		return 0;
	return 1/bins;
}

Double_t ConstantPdf::indefiniteIntegral(Double_t y) const {
	if (y < 0)
		return 0;
	if (y > bins)
		return 0;
	return y/bins;
}

// Get analytical integral
Int_t ConstantPdf::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* rangeName) const {
	if (matchArgs(allVars, analVars, t))
		return 1;
	return 0;
}

// Analytical integral
Double_t ConstantPdf::analyticalIntegral(Int_t code, const char* rangeName) const {
	assert(code == 1);

	if (code == 1) {
		Double_t x1 = t.min(rangeName);
		Double_t x2 = t.max(rangeName);
		if (x2 <= 0)
			return 0;
		x1 = TMath::Max(0., x1);
		return indefiniteIntegral(x2) - indefiniteIntegral(x1);
	}
	std::cout << "Error in ConstantPdf::analyticalIntegral" << std::endl;
	return 0;
}
