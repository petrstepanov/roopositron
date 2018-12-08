/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   BackgroundProvider.cpp
 * Author: petrstepanov
 * 
 * Created on December 8, 2018, 1:32 PM
 */

#include "BackgroundProvider.h"
#include "RooRealVar.h"
#include "TH1F.h"
#include "RooPolynomial.h"
#include "src/util/HistProcessor.h"

BackgroundProvider::BackgroundProvider(RooRealVar* _observable, TH1F* histogram) : AbstractProvider(_observable) {
    this->histogram = histogram;
}

BackgroundProvider::~BackgroundProvider() {
}

RooAbsPdf* BackgroundProvider::initPdf() {

}