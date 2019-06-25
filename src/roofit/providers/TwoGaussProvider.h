/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TwoGaussProvider.h
 * Author: petrstepanov
 *
 * Created on December 6, 2018, 5:10 AM
 */

#ifndef TWOGAUSSPROVIDER_H
#define TWOGAUSSPROVIDER_H

#include "AbstractProvider.h"
#include "RooRealVar.h"

class TwoGaussProvider : public AbstractProvider {
public:
    TwoGaussProvider(RooRealVar* observable, RooRealVar* channelWidth);
    virtual ~TwoGaussProvider();
    
    static RooArgSet* getIndirectParameters(RooAbsPdf* pdf);

private:
    RooAbsPdf* initPdf(int i);
};

#endif /* TWOGAUSSPROVIDER_H */

