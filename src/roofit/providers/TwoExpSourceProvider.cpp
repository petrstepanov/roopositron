/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TwoExpSourceProvider.cpp
 * Author: petrstepanov
 * 
 * Created on December 5, 2018, 8:43 AM
 */

#include "TwoExpSourceProvider.h"
#include "RooRealVar.h"
#include "RooFormulaVar.h"
#include "../pdfs/TwoExpPdf.h"
#include "../../model/Constants.h"
#include "../../util/StringUtils.h"
#include <iostream>

TwoExpSourceProvider::TwoExpSourceProvider(RooRealVar* _observable) : AbstractProvider(_observable) {}

TwoExpSourceProvider::~TwoExpSourceProvider() {
}

RooAbsPdf* TwoExpSourceProvider::initPdf(int i) {
    RooConstVar* channelWidth = Constants::getInstance()->getRooChannelWidth();
    
    // Instantiate RooRealVar parameters
    RooRealVar* tauSource1 = new RooRealVar("#tauSource1", "1st source lifetime", 0.2, 0.1, 2, "ns");
    RooFormulaVar* tauSource1Ch = new RooFormulaVar("tauSource1Ch", "1st source lifetime, channels", "@0/@1", RooArgList(*tauSource1, *channelWidth));

    RooRealVar* tauSource2 = new RooRealVar("#tauSource2", "2nd source lifetime", 0.5, 0.3, 5, "ns");
    RooFormulaVar* tau2Ch = new RooFormulaVar("tau2Ch", "2nd source lifetime, channels", "@0/@1", RooArgList(*tauSource2, *channelWidth));

    RooRealVar* int2 = new RooRealVar("Int_expSource2", "2nd source component fraction", 1, 0, 20, "%");
    RooFormulaVar* int2Norm = new RooFormulaVar("int2Norm", "@0/100", *int2);

    // Instantinate model
    return new TwoExpPdf("twoExpComponent", "two exponential component", *observable, *tauSource2, *tauSource2, *int2Norm);
}
