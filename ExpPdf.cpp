/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ExpPdf.cpp
 * Author: petrstepanov
 * 
 * Created on November 9, 2016, 7:49 PM
 */

#include "ExpPdf.h"

ExpPdf::ExpPdf(const char *name, const char *title,
    RooAbsReal& _t,
    RooAbsReal& _tau) :
    RooAbsPdf(name, title),
    t("t", "t", this, _t),
    tau("tau", "tau", this, _tau){
}

ExpPdf::ExpPdf(const ExpPdf& other, const char* name) :
RooAbsPdf(other, name),
t("t", this, other.t),
tau("tau", this, other.tau){
}

// No normalization here: https://root.cern.ch/doc/master/classRooAbsPdf.html
Double_t ExpPdf::evaluate() const {
    if (t < 0) return 0.;
    return exp(-t/tau);
}

// Indefinite integral without normalization [0;+infty)
Double_t ExpPdf::indefiniteIntegral(Double_t y) const {
    return -exp(-y/tau)*tau;
}

// Get analytical integral
Int_t ExpPdf::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* rangeName) const {
   if (matchArgs(allVars,analVars,t)) return 1;
   return 0;
}

// Analytical integral
Double_t ExpPdf::analyticalIntegral(Int_t code, const char* rangeName) const {
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