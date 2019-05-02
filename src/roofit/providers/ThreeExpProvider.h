/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ThreeExpProvider.h
 * Author: petrstepanov
 *
 * Created on December 5, 2018, 8:43 AM
 */

#ifndef THREEEXPPROVIDER_H
#define THREEEXPPROVIDER_H

#include "AbstractProvider.h"
#include "RooRealVar.h"
#include "RooAbsPdf.h"

class ThreeExpProvider : public AbstractProvider {
public:
    ThreeExpProvider(RooRealVar* observable, RooRealVar* channelWidth);
    virtual ~ThreeExpProvider();

    RooAbsPdf* initPdf(int i);
};

#endif /* THREEEXPPROVIDER_H */

