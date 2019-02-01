/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ThreeExpPdf.cpp
 * Author: petrstepanov
 * 
 * Created on November 9, 2016, 7:52 PM
 */

#include "ThreeExpPdf.h"

ClassImp(ThreeExpPdf);

ThreeExpPdf::ThreeExpPdf(const char *name, const char *title,
	RooAbsReal& _t,
	RooAbsReal& _tau1,
	RooAbsReal& _tau2,
	RooAbsReal& _tau3,
        RooAbsReal& _i2,
	RooAbsReal& _i3) :
	RooAbsPdf(name, title),
	t("t", "t", this, _t),
	tau1("tau1", "tau1", this, _tau1),
	tau2("tau2", "tau2", this, _tau2),
	tau3("tau3", "tau3", this, _tau3),
        i2("i2", "i2", this, _i2),
	i3("i3", "i3", this, _i3){
}

ThreeExpPdf::ThreeExpPdf(const ThreeExpPdf& other, const char* name) :
RooAbsPdf(other, name),
t("t", this, other.t),
tau1("tau1", this, other.tau1),
tau2("tau2", this, other.tau2),
tau3("tau3", this, other.tau3),
i2("i2", this, other.i2),
i3("i3", this, other.i3){
}

// Evaluate without normalization
Double_t ThreeExpPdf::evaluate() const {
    if (t < 0) return 0.;
    Double_t ret1 = exp(-t/tau1);
    Double_t ret2 = exp(-t/tau2);
    Double_t ret3 = exp(-t/tau3);
    return (1. - i2 - i3)*ret1 + i2*ret2+ i3*ret3;
}

// Indefinite integral without normalization [0;+infty)
Double_t ThreeExpPdf::indefiniteIntegral(Double_t y) const {
    Double_t int1 = -exp(-y/tau1)*tau1;
    Double_t int2 = -exp(-y/tau2)*tau2;    
    Double_t int3 = -exp(-y/tau3)*tau3; 
    return (1. - i2 - i3)*int1 + i2*int2 + i3*int3;
}

// Get analytical integral
Int_t ThreeExpPdf::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* rangeName) const {
   if (matchArgs(allVars,analVars,t)) return 1;
   return 0;
}

// Analytical integral
Double_t ThreeExpPdf::analyticalIntegral(Int_t code, const char* rangeName) const {
    assert(code == 1);
    
    Double_t ret = 0;
    if (code==1){
        Double_t x1 = t.min(rangeName);
        Double_t x2 = t.max(rangeName);
        if (x2 <= 0) return 0;
        x1 = TMath::Max(0.,x1);

        ret = indefiniteIntegral(x2)-indefiniteIntegral(x1);
    }
    else {
        std::cout << "Error in RooGaussian::analyticalIntegral" << std::endl;
    }
    return ret;    
}

