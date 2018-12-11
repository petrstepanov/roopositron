/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   OneGaussProvider.h
 * Author: petrstepanov
 *
 * Created on December 6, 2018, 5:10 AM
 */

#ifndef ONEGAUSSPROVIDER_H
#define ONEGAUSSPROVIDER_H

#include "AbstractProvider.h"
#include "RooRealVar.h"

class OneGaussProvider : public AbstractProvider {
public:
    OneGaussProvider(RooRealVar* observable);
    virtual ~OneGaussProvider();
    
private:
    RooAbsPdf* initPdf(int i);
};

#endif /* ONEGAUSSPROVIDER_H */

