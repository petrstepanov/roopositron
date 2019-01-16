/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PowderProvider.cpp
 * Author: petrstepanov
 * 
 * Created on December 6, 2018, 1:03 PM
 */

#include "PowderProvider.h"
#include "RooFormulaVar.h"
#include "../pdfs/PowderPdf.h"
#include "../../model/Constants.h"

PowderProvider::PowderProvider(RooRealVar* observable) : AbstractProvider(observable) {}
    
PowderProvider::~PowderProvider() {
}

RooAbsPdf* PowderProvider::initPdf(int i) {
    RooConstVar* channelWidth = Constants::getInstance()->getRooChannelWidth();

//    L("L", this, other.L),
//    kv("kv", this, other.kv),
//    Pps("Pps", this, other.Pps),
//    lb("lb", this, other.lb),
//    lv("lv", this, other.lv),
//    lPs("lPs", this, other.lPs),
//    Mr("Mr", this, other.Mr),
//    mu("mu", this, other.mu)

    RooFormulaVar* observableNs = new RooFormulaVar("observableNs", "@0*@1", RooArgList(*observable, *channelWidth));
    
    RooRealVar* L = new RooRealVar("L", "Powder diameter", 500, 10, 50000, "A"); // 10A - 50mkm

    RooRealVar* kappaVac = new RooRealVar("kappaVac", "Vacancy trapping rate", 1, 1E-2, 1E2, "1/ns");
//    RooFormulaVar* kappaVacCh = new RooFormulaVar("kappaVacCh", "@0*@1", RooArgList(*kappaVac, *channelWidth));
    
    RooRealVar* P_Ps = new RooRealVar("P_Ps", "Probability of forming qf-Ps", 0.5, 0, 1, "");
    
    RooRealVar* tauBulk = new RooRealVar("tauBulk", "e+ lifetime in bulk", 0.15, 0.1, 0.4, "ns");
    RooFormulaVar* lambdaBulk = new RooFormulaVar("lambdaBulk", "1/@0", *tauBulk);
//    RooFormulaVar* lambdaBulkCh = new RooFormulaVar("lambdaBulkCh", "@0*@1", RooArgList(*lambdaBulk, *channelWidth));

    RooRealVar* tauVac = new RooRealVar("tauVac", "e+ lifetime in vacancy", 0.2, 0.1, 0.4, "ns");
    RooFormulaVar* lambdaVac = new RooFormulaVar("lambdaVac", "1/@0", *tauVac);
//    RooFormulaVar* lambdaVacCh = new RooFormulaVar("lambdaVacCh", "@0*@1", RooArgList(*lambdaVac, *channelWidth));

    // Here we set tauPs = tauBulk
    RooRealVar* tauPs = new RooRealVar("tauPs", "Ps lifetime in vacancy", 1, 0.01, 100, "ns");
    RooFormulaVar* lambdaPs = new RooFormulaVar("lambdaPs", "1/@0", *tauPs);
//    RooFormulaVar* lambdaPsCh = new RooFormulaVar("lambdaPsCh", "@0*@1", RooArgList(*lambdaPs, *channelWidth));

    RooRealVar* Mratio = new RooRealVar("Mratio", "Powder mass to positron mass ratio", 1E5, 1E2, 1E7, "");   
    RooRealVar* mu = new RooRealVar("mu", "Probability of ortho-para conversion close to powder surface", 1E-5, 1E-7, 1E-3, "");

    return new PowderPdf("powder", "Powder model", *observableNs, *L, *kappaVac, *P_Ps, *lambdaBulk, *lambdaVac, *lambdaBulk/* *lambdaPs*/, *Mratio, *mu, channelWidth->getValV());
}
