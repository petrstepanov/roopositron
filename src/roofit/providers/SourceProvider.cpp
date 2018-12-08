/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SourceProvider.cpp
 * Author: petrstepanov
 * 
 * Created on December 5, 2018, 8:43 AM
 */

#include "SourceProvider.h"
#include "RooFormulaVar.h"
#include "../pdfs/ExpPdf.h"
#include "../../model/Constants.h"

SourceProvider::SourceProvider(RooRealVar* _observable) : AbstractProvider(_observable) {}

SourceProvider::~SourceProvider() {
}

RooAbsPdf* SourceProvider::initPdf() {
    RooConstVar* channelWidth = Constants::getInstance()->getRooChannelWidth();
    
    // Instantiate RooRealVar parameters
    RooRealVar* tSource = new RooRealVar("tSource", "positron lifetime in source", 0.385, "ns");
    tSource->setConstant(kTRUE);
    RooFormulaVar* tSourceCh = new RooFormulaVar("tSourceCh", "@0/@1", RooArgList(*tSource, *channelWidth));
    // Instantinate model
    return new ExpPdf("source", "Source contribution model", *observable, *tSourceCh);
}