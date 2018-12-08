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

AbstractProvider::AbstractProvider(RooRealVar* _observable) {
    observable = _observable;
}

RooAbsPdf* AbstractProvider::getPdf() {
    if(pdf == NULL){
	pdf = initPdf();
    }
    return pdf;
}

