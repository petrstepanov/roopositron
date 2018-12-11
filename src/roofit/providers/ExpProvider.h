/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ExpProvider.h
 * Author: petrstepanov
 *
 * Created on December 5, 2018, 8:43 AM
 */

#ifndef EXPPROVIDER_H
#define EXPPROVIDER_H

#include "AbstractProvider.h"
#include "RooRealVar.h"
#include "RooAbsPdf.h"

class ExpProvider : public AbstractProvider {
public:
    ExpProvider(RooRealVar* observable);
    virtual ~ExpProvider();

    RooAbsPdf* initPdf(int i);
};

#endif /* EXPPROVIDER_H */

