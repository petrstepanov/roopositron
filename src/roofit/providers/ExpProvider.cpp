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
#include "RooRealVar.h"
#include "RooFormulaVar.h"
#include "../pdfs/ExpPdf.h"
#include "../../model/Constants.h"
#include <iostream>

ExpProvider::ExpProvider(RooRealVar* _observable) : AbstractProvider(_observable) {}

ExpProvider::~ExpProvider() {
}

RooAbsPdf* ExpProvider::initPdf(int i) {
    RooConstVar* channelWidth = Constants::getInstance()->getRooChannelWidth();
    
    // Instantiate RooRealVar parameters
    RooRealVar* tau = new RooRealVar((i==0)?"tau":TString::Format("tau%d",i+1).Data(), "positron lifetime", 0.2, 0.05, 2, "ns");
    RooFormulaVar* tauCh = new RooFormulaVar((i==0)?"tauCh":TString::Format("tauCh%d",i+1).Data(), "@0/@1", RooArgList(*tau, *channelWidth));
    
    // Instantinate model
    return new ExpPdf((i==0)?"exp":TString::Format("exp%d",i+1).Data(), "Exponential model", *observable, *tauCh);
}