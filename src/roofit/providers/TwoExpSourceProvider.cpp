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
    RooRealVar* tau1Source = new RooRealVar("#tau1_source", "1st positron lifetime in source", 0.2, 0.1, 2, "ns");
    RooFormulaVar* tau1SourceCh = new RooFormulaVar("tau1SourceCh", "1st positron lifetime in source, channels", "@0/@1", RooArgList(*tau1Source, *channelWidth));

    RooRealVar* tau2Source = new RooRealVar("#tau2_source", "2nd positron lifetime in source", 0.5, 0.3, 5, "ns");
    RooFormulaVar* tau2SourceCh = new RooFormulaVar("tau2SourceCh", "2nd positron lifetime in source, channels", "@0/@1", RooArgList(*tau2Source, *channelWidth));

    RooRealVar* Int_expSource2 = new RooRealVar("Int_expSource2", "2nd exponent intensity in source", 1, 0, 20, "%");
    RooFormulaVar* intExpSource2Norm = new RooFormulaVar("intExpSource2Norm", "@0/100", *Int_expSource2);

    // Instantinate model
    return new TwoExpPdf("twoExpSource", "two exponential source pdf", *observable, *tau1SourceCh, *tau2SourceCh, *intExpSource2Norm);
}
