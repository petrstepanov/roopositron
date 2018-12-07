/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TwoExpPdfProvider.cpp
 * Author: petrstepanov
 * 
 * Created on December 6, 2018, 1:09 PM
 */

#include "TwoExpPdfProvider.h"

TwoExpPdfProvider::TwoExpPdfProvider(RooRealVar* _observable) : AbstractProvider(_observable) {}

TwoExpPdfProvider::~TwoExpPdfProvider() {
}

RooAbsPdf* TwoExpPdfProvider::initPdf() {
//    RooRealVar* tau1 = storage->getOrMakeNew("tau1", "1st_exponent_lifetime", 0.15, 0.1, 0.7, "ns");
//    RooFormulaVar* tau1_ch = new RooFormulaVar("tau1_ch", "@0/@1", RooArgList(*tau1, *channelWidth));
//    RooRealVar* tau2 = storage->getOrMakeNew("tau2", "2nd_exponent_lifetime", 0.5, 0.2, 3.0, "ns");
//    RooFormulaVar* tau2_ch = new RooFormulaVar("tau2_ch", "@0/@1", RooArgList(*tau2, *channelWidth));
//    RooRealVar* I_tau2 = storage->getOrMakeNew("I_tau2", "2nd_exponent_fraction", 10, 0, 100, "%");
//    RooFormulaVar* I_tau2_ = new RooFormulaVar("I_tau2_", "@0/100.", *I_tau2);
//    decay_model = new TwoExpPdf("decay_model", "decay_model", *rChannels, *tau1_ch, *tau2_ch, *I_tau2_);    
    return NULL;
}