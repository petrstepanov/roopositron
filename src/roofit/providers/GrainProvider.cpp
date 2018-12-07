/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GrainProvider.cpp
 * Author: petrstepanov
 * 
 * Created on December 6, 2018, 1:03 PM
 */

#include "GrainProvider.h"
#include "RooFormulaVar.h"
#include "../../model/RootConstants.h"
#include "../pdfs/GrainPdf.h"

GrainProvider::GrainProvider(RooRealVar* observable) : AbstractProvider(observable) {}
    
GrainProvider::~GrainProvider() {
}

RooAbsPdf* GrainProvider::initPdf() {
    RooConstVar* fwhm2disp = RootConstants::getInstance()->fwhm2disp;
    RooConstVar* channelWidth = RootConstants::getInstance()->channelWidth;
	
    RooRealVar* tauBulk = new RooRealVar("tauBulk", "e+_lifetime_in_source", 0.120, 0.120, 0.120, "ns");
    RooFormulaVar* lambdaBulk = new RooFormulaVar("lambdaBulk", "1/@0", *tauBulk);
    RooFormulaVar* lambdaBulkCh = new RooFormulaVar("lambdaBulkCh", "@0*@1", RooArgList(*lambdaBulk, *channelWidth));

    RooRealVar* tauGrain = new RooRealVar("tauGrain", "e+_lifetime_in_grain", 0.140, 0.140, 0.140, "ns");
    RooFormulaVar* lambdaGrain = new RooFormulaVar("lambdaGrain", "1/@0", *tauGrain);
    RooFormulaVar* lambdaGrainCh = new RooFormulaVar("lambdaGrainCh", "@0*@1", RooArgList(*lambdaGrain, *channelWidth));

    RooRealVar* tauVac = new RooRealVar("tauVac", "e+_lifetime_in_vacancy", 0.180, 0.180, 0.180, "ns");
    RooFormulaVar* lambdaVac = new RooFormulaVar("lambdaVac", "1/@0", *tauVac);
    RooFormulaVar* lambdaVacCh = new RooFormulaVar("lambdaVacCh", "@0*@1", RooArgList(*lambdaVac, *channelWidth));

    RooRealVar* kappaVac = new RooRealVar("kappaVac", "vacancy_trapping_rate", 1, 1E-2, 1E2, "1/ns");
    RooFormulaVar* kappaVacCh = new RooFormulaVar("kappaVacCh", "@0*@1", RooArgList(*kappaVac, *channelWidth));

    RooRealVar* lambdaJ = new RooRealVar("lambdaJ", "grain_trapping_rate", 1, 1E-2, 1E2, "1/ns");
    RooFormulaVar* lambdaJCh = new RooFormulaVar("lambdaJCh", "@0*@1", RooArgList(*lambdaJ, *channelWidth));

    // Initialize cache
//            MyPdfCache* myPdfCache = new MyPdfCache(100, MIN_CHANNEL, MAX_CHANNEL);
//            MyPdf::myPdfCache = myPdfCache;

//            decay_model = new MyPdf("decay_model", "decay_model", *rChannels, *lambdaJ_ch, *lambdaBulk_ch, *lambdaGrain_ch, *lambdaVac_ch, *kappaVac_ch);
    // add cache to model - counts 10x faster

    return new GrainPdf("decay_model", "decay_model", *observable, *lambdaJCh, *lambdaBulkCh, *lambdaGrainCh, *lambdaVacCh, *kappaVacCh);
}
