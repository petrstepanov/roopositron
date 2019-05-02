/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ExpSourceProvider.cpp
 * Author: petrstepanov
 * 
 * Created on December 5, 2018, 8:43 AM
 */

#include "RooFormulaVar.h"
#include "../pdfs/ExpPdf.h"
#include "../../model/Constants.h"
#include "../../util/StringUtils.h"
#include "ExpSourceProvider.h"

ExpSourceProvider::ExpSourceProvider(RooRealVar* _observable, RooRealVar* _channelWidth) : AbstractProvider(_observable, _channelWidth) {}

ExpSourceProvider::~ExpSourceProvider() {
}

RooAbsPdf* ExpSourceProvider::initPdf(int i) {
    Double_t tauVal=0.385;
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
    
    // Instantiate RooRealVar parameters
    RooRealVar* tauSource = new RooRealVar(StringUtils::suffix("#tau_source",i).c_str(), StringUtils::ordinal("positron lifetime in source", i).c_str(), 0.385, 0.1, 5, "ns");
    if(i==1) tauSource->setConstant(kTRUE);

    RooFormulaVar* tauSourceCh = new RooFormulaVar(StringUtils::suffix("tauSourceCh",i).c_str(), StringUtils::ordinal("positron lifetime in source, channels", i).c_str(), "@0/@1", RooArgList(*tauSource, *channelWidth));

    // Instantiate model
    return new ExpPdf(StringUtils::suffix("expSource",i).c_str(), StringUtils::ordinal("exponential source pdf", i).c_str(), *observable, *tauSourceCh);
}
