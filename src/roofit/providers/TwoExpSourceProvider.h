/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TwoExpSourceProvider.h
 * Author: petrstepanov
 *
 * Created on December 5, 2018, 8:43 AM
 */

#ifndef TWOEXPSOURCEPROVIDER_H
#define TWOEXPSOURCEPROVIDER_H

#include "AbstractProvider.h"
#include "RooRealVar.h"
#include "RooAbsPdf.h"

class TwoExpSourceProvider : public AbstractProvider {
public:
    TwoExpSourceProvider(RooRealVar* observable);
    virtual ~TwoExpSourceProvider();

    RooAbsPdf* initPdf(int i);
};

#endif /* EXPPROVIDER_H */

