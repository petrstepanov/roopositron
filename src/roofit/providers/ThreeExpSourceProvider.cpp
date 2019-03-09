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

ThreeExpSourceProvider::ThreeExpSourceProvider(RooRealVar* _observable) : AbstractProvider(_observable) {}

ThreeExpSourceProvider::~ThreeExpSourceProvider() {
}

RooAbsPdf* ThreeExpSourceProvider::initPdf(int i) {
    RooConstVar* channelWidth = Constants::getInstance()->getRooChannelWidth();
    
    // Instantiate RooRealVar parameters
    RooRealVar* tau1 = new RooRealVar("#tau1", "1st positron lifetime", 0.2, 0.1, 2, "ns");
    RooFormulaVar* tau1Ch = new RooFormulaVar("tau1Ch", "1st positron lifetime, channels", "@0/@1", RooArgList(*tau1, *channelWidth));

    RooRealVar* tau2 = new RooRealVar("#tau2", "2nd positron lifetime", 0.5, 0.1, 5, "ns");
    RooFormulaVar* tau2Ch = new RooFormulaVar("tau2Ch", "2nd positron lifetime, channels", "@0/@1", RooArgList(*tau2, *channelWidth));

    RooRealVar* int2 = new RooRealVar("Int_exp2", "2nd exponent fraction", 1, 0, 50, "%");
    RooFormulaVar* int2Norm = new RooFormulaVar("int2Norm", "@0/100", *int2);

    RooRealVar* tau3 = new RooRealVar("#tau3", "3rd positron lifetime", 1.0, 0.1, 5, "ns");
    RooFormulaVar* tau3Ch = new RooFormulaVar("tau3Ch", "2nd positron lifetime, channels", "@0/@1", RooArgList(*tau3, *channelWidth));

    RooRealVar* int3 = new RooRealVar("Int_exp3", "3rd exponent fraction", 1, 0, 20, "%");
    RooFormulaVar* int3Norm = new RooFormulaVar("int2Norm", "@0/100", *int3);

    // Instantinate model
    return new ThreeExpPdf("threeExpComponent", "three exponential component", *observable, *tau1, *tau2, *tau3, *int2Norm, *int3Norm);
}
