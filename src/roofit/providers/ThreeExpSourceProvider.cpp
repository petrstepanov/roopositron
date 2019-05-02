/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ThreeExpSourceProvider.cpp
 * Author: petrstepanov
 * 
 * Created on December 5, 2018, 8:43 AM
 */

#include "ThreeExpSourceProvider.h"
#include "RooRealVar.h"
#include "RooFormulaVar.h"
#include "../pdfs/ThreeExpPdf.h"
#include "../../model/Constants.h"
#include "../../util/StringUtils.h"
#include <iostream>

ThreeExpSourceProvider::ThreeExpSourceProvider(RooRealVar* _observable, RooRealVar* _channelWidth) : AbstractProvider(_observable, _channelWidth) {}

ThreeExpSourceProvider::~ThreeExpSourceProvider() {
}

RooAbsPdf* ThreeExpSourceProvider::initPdf(int i) {
    // Instantiate RooRealVar parameters
    RooRealVar* tau1Source = new RooRealVar("#tau1_source", "1st positron lifetime in source", 0.2, 0.1, 2, "ns");
    RooFormulaVar* tau1SourceCh = new RooFormulaVar("tau1SourceCh", "1st positron lifetime in source, channels", "@0/@1", RooArgList(*tau1Source, *channelWidth));

    RooRealVar* tau2Source = new RooRealVar("#tau2_source", "2nd positron lifetime in source", 0.5, 0.1, 5, "ns");
    RooFormulaVar* tau2SourceCh = new RooFormulaVar("tau2SourceCh", "2nd positron lifetime in source, channels", "@0/@1", RooArgList(*tau2Source, *channelWidth));

    RooRealVar* tau3Source = new RooRealVar("#tau3_source", "3rd positron lifetime in source", 1.0, 0.1, 10, "ns");
    RooFormulaVar* tau3SourceCh = new RooFormulaVar("tau3SourceCh", "3rd positron lifetime in source, channels", "@0/@1", RooArgList(*tau3Source, *channelWidth));

    RooRealVar* Int_expSource2 = new RooRealVar("Int_expSource2", "2nd exponent intensity in source", 20, 0, 100, "%");
    RooFormulaVar* intExpSource2Norm = new RooFormulaVar("intExpSource2Norm", "@0/100", *Int_expSource2);

    RooRealVar* Int_expSource3= new RooRealVar("Int_expSource3", "3rd exponent intensity in source", 20, 0, 100, "%");
    RooFormulaVar* intExpSource3Norm = new RooFormulaVar("intExpSource3Norm", "@0/100", *Int_expSource3);

    // Instantiate model
    return new ThreeExpPdf("threeExpSource", "three exponential source pdf", *observable, *tau1SourceCh, *tau2SourceCh, *tau3SourceCh, *intExpSource2Norm, *intExpSource3Norm);
}
