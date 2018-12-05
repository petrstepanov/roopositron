/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ExpPdfProvider.cpp
 * Author: petrstepanov
 * 
 * Created on December 5, 2018, 8:43 AM
 */

#include "ExpPdfProvider.h"
#include "../ExpPdf.h"
#include "../../util/ObjectNamer.h"

ExpPdfProvider::ExpPdfProvider(RooRealVar* _observable, ParamStorage* _paramStorage) : AbstractProvider(_observable, _paramStorage) {}

ExpPdfProvider::~ExpPdfProvider() {
}

RooAbsPdf* ExpPdfProvider::initPdf() {
    // Instantiate RooRealVar parameters
    RooRealVar* tau = paramStorage->getOrMakeNew("tau", "positron lifetime");

    // Instantiate model with unique name
    ObjectNamer* namer = ObjectNamer::getInstance();
    return new ExpPdf(namer->getUniqueName("exp"), "exponential model", *observable, *tau);
}




