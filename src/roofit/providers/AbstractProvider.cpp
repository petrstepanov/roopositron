/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   AbstractProvider.cpp
 * Author: petrstepanov
 * 
 * Created on December 5, 2018, 6:52 AM
 */

#include "AbstractProvider.h"

AbstractProvider::AbstractProvider(RooRealVar* _observable, ParamStorage* _paramStorage) {
    observable = _observable;
    paramStorage = _paramStorage;
    init();
}

//AbstractProvider::AbstractProvider(const AbstractProvider& orig) {
//}
//
//AbstractProvider::~AbstractProvider() {
//}

void AbstractProvider::init() {
//    parameters = initParameters();
    pdf = initPdf();
}

RooAbsPdf* AbstractProvider::getPdf() {
    return pdf;
}

//RooArgList* AbstractProvider::getParameters() {
//    return parameters;
//}
