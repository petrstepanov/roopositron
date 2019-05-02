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
#include "../../util/StringUtils.h"
#include <iostream>

ExpProvider::ExpProvider(RooRealVar* _observable, RooRealVar* _channelWidth) : AbstractProvider(_observable, _channelWidth) {}

ExpProvider::~ExpProvider() {
}

RooAbsPdf* ExpProvider::initPdf(int i) {
    // Instantiate RooRealVar parameters
    Double_t tauVal=0.2;
    Double_t tauValMin=0.1;
    Double_t tauValMax=3;

    if(i==2){
    	tauVal=0.3;
    	tauValMin=0.2;
    	tauValMax=5;
    }
    if(i>=3){
    	tauVal=1.5;
    	tauValMin=0.3;
    	tauValMax=10;
    }

    RooRealVar* tau = new RooRealVar(StringUtils::suffix("#tau", i).c_str(), StringUtils::ordinal("positron lifetime", i).c_str(), tauVal, tauValMin, tauValMax, "ns");
    RooFormulaVar* tauCh = new RooFormulaVar(StringUtils::suffix("tauCh", i).c_str(), StringUtils::ordinal("positron lifetime, channels", i).c_str(), "@0/@1", RooArgList(*tau, *channelWidth));
    
    // Instantinate model
    return new ExpPdf(StringUtils::suffix("exp", i).c_str(), StringUtils::ordinal("exponential pdf", i).c_str(), *observable, *tauCh);
}
