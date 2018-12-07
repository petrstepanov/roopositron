/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TrappingProvider.cpp
 * Author: petrstepanov
 * 
 * Created on December 6, 2018, 1:02 PM
 */

#include "TrappingProvider.h"
#include "RooFormulaVar.h"
#include "../../model/RootConstants.h"
#include "../pdfs/TrapPdf.h"

TrappingProvider::TrappingProvider(RooRealVar* observable) : AbstractProvider(observable) {}
    
TrappingProvider::~TrappingProvider() {
}

RooAbsPdf* TrappingProvider::initPdf() {
    RooConstVar* fwhm2disp = RootConstants::getInstance()->fwhm2disp;
    RooConstVar* channelWidth = RootConstants::getInstance()->channelWidth;
	
    RooRealVar* tauBulk = new RooRealVar("tauBulk", "e+ lifetime in source", 0.15, 0.1, 0.3, "ns");
    RooFormulaVar* tauBulkCh = new RooFormulaVar("tauBulkCh", "@0/@1", RooArgList(*tauBulk, *channelWidth));
    RooRealVar* tauVac = new RooRealVar("tauVac", "e+_lifetime_in_vacancy", 0.2, 0.1, 0.3, "ns");
    RooFormulaVar* tauVacCh = new RooFormulaVar("tauVacCh", "@0/@1", RooArgList(*tauVac, *channelWidth));
    RooRealVar* kappaVac = new RooRealVar("kappaVac", "vacancy_trapping_rate", 1, 1E-2, 1E2, "1/ns");
    RooFormulaVar* kappaVacCh = new RooFormulaVar("kappaVacCh", "@0*@1", RooArgList(*kappaVac, *channelWidth));
    return new TrapPdf("decay_model", "decay_model", *observable, *tauBulkCh, *tauVacCh, *kappaVacCh);
}
