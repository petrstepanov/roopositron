/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   AbstractProvider.h
 * Author: petrstepanov
 *
 * Created on December 5, 2018, 6:52 AM
 */

#ifndef ABSTRACTPROVIDER_H
#define ABSTRACTPROVIDER_H

#include "RooArgList.h"
#include "RooRealVar.h"
#include "RooAbsPdf.h"
#include "../../model/ParametersPool.h"

class AbstractProvider {
public:
    AbstractProvider(RooRealVar* observable);
    RooAbsPdf* getPdf();
    
protected:
    virtual RooAbsPdf* initPdf()=0;  
    RooRealVar* observable;
    RooAbsPdf* pdf = NULL;   
};

#endif /* ABSTRACTPROVIDER_H */

