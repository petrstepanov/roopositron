/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ExpProvider.cpp
 * Author: petrstepanov
 * 
 * Created on December 5, 2018, 8:43 AM
 */

#include "ExpProvider.h"
#include "RooFormulaVar.h"
#include "../pdfs/ExpPdf.h"
#include "../../model/RootConstants.h"


ExpProvider::ExpProvider(RooRealVar* _observable) : AbstractProvider(_observable) {}

ExpProvider::~ExpProvider() {
}

RooAbsPdf* ExpProvider::initPdf() {
    RooConstVar* channelWidth = RootConstants::getInstance()->channelWidth;
    
    // Instantiate RooRealVar parameters
    RooRealVar* tau = new RooRealVar("tau", "positron lifetime", 0.2, 0.05, 2, "ns");
    RooFormulaVar* tauCh = new RooFormulaVar("tauCh", "@0/@1", RooArgList(*tau, *channelWidth));
    
    // Instantinate model
    return new ExpPdf("exp", "exponential model", *observable, *tauCh);
}