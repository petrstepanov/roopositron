/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TrapPdf.cpp
 * Author: petrstepanov
 * 
 * Created on November 17, 2016, 1:59 AM
 */

#include "TrapPdf.h"

ClassImp(TrapPdf);

TrapPdf::TrapPdf(const char *name, const char *title,
	RooAbsReal& _t,
	RooAbsReal& _tauBulk,
	RooAbsReal& _tauDef,
	RooAbsReal& _kappaDef) :
	RooAbsPdf(name, title),
	t("t", "t", this, _t),
	tauBulk("tauBulk", "tauBulk", this, _tauBulk),
	tauDef("tauDef", "tauDef", this, _tauDef),
	kappaDef("kappaDef", "kappaDef", this, _kappaDef){
}

TrapPdf::TrapPdf(const TrapPdf& other, const char* name) :
RooAbsPdf(other, name),
t("t", this, other.t),
tauBulk("tauBulk", this, other.tauBulk),
tauDef("tauDef", this, other.tauDef),
kappaDef("kappaDef", this, other.kappaDef){
}

Double_t TrapPdf::evaluate() const {
    if (t < 0) return 0.;
    Double_t lambdaB = 1/tauBulk;
    Double_t lambdaD = 1/tauDef;

    Double_t I_1 = (lambdaB-lambdaD)/(lambdaB+kappaDef-lambdaD);
    Double_t I_2 = kappaDef/(lambdaB+kappaDef-lambdaD);

    return I_1*(lambdaB+kappaDef)*exp(-(lambdaB+kappaDef)*t) + I_2*lambdaD*exp(-lambdaD*t);
}

Double_t TrapPdf::indefiniteIntegral(Double_t y) const {
    Double_t lambdaB = 1/tauBulk;
    Double_t lambdaD = 1/tauDef;
    
    Double_t ret = -exp(-y*lambdaD) * kappaDef;
    ret += exp(-y*(kappaDef + lambdaB))*(kappaDef + lambdaB)*(lambdaB - lambdaD)/(-kappaDef - lambdaB);
    ret = ret/(kappaDef + lambdaB - lambdaD);

    return ret;
}

// Get analytical integral -- important to return 0 and 1!
Int_t TrapPdf::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* rangeName) const {
   if (matchArgs(allVars,analVars,t)) return 1;
   return 0;
}

// Analytical integral
Double_t TrapPdf::analyticalIntegral(Int_t code, const char* rangeName) const {
    assert(code == 1);
    
    Double_t ret = 0;
    if (code==1){
        Double_t t1 = t.min(rangeName);
        Double_t t2 = t.max(rangeName);

        ret = indefiniteIntegral(t2)-indefiniteIntegral(t1);
    }
    else {
        std::cout << "Error in TrapPdf::analyticalIntegral" << std::endl;
    }
    return ret;      
}
