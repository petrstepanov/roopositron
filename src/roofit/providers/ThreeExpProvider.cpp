/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ThreeExpProvider.cpp
 * Author: petrstepanov
 * 
 * Created on December 5, 2018, 8:43 AM
 */

#include "ThreeExpProvider.h"
#include "RooRealVar.h"
#include "RooFormulaVar.h"
#include "../pdfs/ThreeExpPdf.h"
#include "../../model/Constants.h"
#include "../../util/StringUtils.h"
#include <iostream>

ThreeExpProvider::ThreeExpProvider(RooRealVar* _observable) : AbstractProvider(_observable) {}

ThreeExpProvider::~ThreeExpProvider() {
}

RooAbsPdf* ThreeExpProvider::initPdf(int i) {
    RooConstVar* channelWidth = Constants::getInstance()->getRooChannelWidth();
    
    // Instantiate RooRealVar parameters
    RooRealVar* tau1 = new RooRealVar("#tau1", "1st positron lifetime", 0.2, 0.1, 2, "ns");
    RooFormulaVar* tau1Ch = new RooFormulaVar("tau1Ch", "1st positron lifetime, channels", "@0/@1", RooArgList(*tau1, *channelWidth));

    RooRealVar* tau2 = new RooRealVar("#tau2", "2nd positron lifetime", 0.5, 0.1, 5, "ns");
    RooFormulaVar* tau2Ch = new RooFormulaVar("tau2Ch", "2nd positron lifetime, channels", "@0/@1", RooArgList(*tau2, *channelWidth));

    RooRealVar* tau3 = new RooRealVar("#tau3", "3rd positron lifetime", 1.0, 0.1, 10, "ns");
    RooFormulaVar* tau3Ch = new RooFormulaVar("tau3Ch", "3rd positron lifetime, channels", "@0/@1", RooArgList(*tau3, *channelWidth));

    RooRealVar* int2 = new RooRealVar("Int2", "2nd exponent fraction", 20, 0, 100, "%");
    RooFormulaVar* int2Norm = new RooFormulaVar("int2Norm", "@0/100", *int2);

    RooRealVar* int3 = new RooRealVar("Int3", "3rd exponent fraction", 20, 0, 100, "%");
    RooFormulaVar* int3Norm = new RooFormulaVar("int3Norm", "@0/100", *int3);

    // Instantinate model
    return new ThreeExpPdf("threeExpPdf", "three exponential component", *observable, *tau1Ch, *tau2Ch, *tau3Ch, *int2Norm, *int3Norm);
}
