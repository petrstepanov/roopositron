/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ThreeExpPdfProvider.cpp
 * Author: petrstepanov
 * 
 * Created on December 6, 2018, 1:10 PM
 */

#include "ThreeExpPdfProvider.h"

ThreeExpPdfProvider::ThreeExpPdfProvider(RooRealVar* _observable) : AbstractProvider(_observable) {}

ThreeExpPdfProvider::~ThreeExpPdfProvider() {
}

RooAbsPdf* ThreeExpPdfProvider::initPdf(int i) {
//    RooRealVar* tau1 = storage->getOrMakeNew("tau1", "1st_exponent_lifetime", 0.15, 0.1, 0.7, "ns");
//    RooFormulaVar* tau1_ch = new RooFormulaVar("tau1_ch", "@0/@1", RooArgList(*tau1, *channelWidth));
//    RooRealVar* tau2 = storage->getOrMakeNew("tau2", "2nd_exponent_lifetime", 0.5, 0.2, 3.0, "ns");
//    RooFormulaVar* tau2_ch = new RooFormulaVar("tau2_ch", "@0/@1", RooArgList(*tau2, *channelWidth));
//    RooRealVar* tau3 = storage->getOrMakeNew("tau3", "3rd_exponent_lifetime", 1, 0.5, 15, "ns");
//    RooFormulaVar* tau3_ch = new RooFormulaVar("tau3_ch", "@0/@1", RooArgList(*tau3, *channelWidth));
//    RooRealVar* I_tau2 = storage->getOrMakeNew("I_tau2", "2nd_exponent_fraction", 10, 0, 100, "%");
//    RooFormulaVar* I_tau2_ = new RooFormulaVar("I_tau2_", "@0/100.", *I_tau2);
//    decay_model = new ThreeExpPdf("decay_model", "decay_model", *rChannels, *tau1_ch, *tau2_ch, *tau3_ch, *I_tau2_, *I_tau3_);
    
    return NULL;
}