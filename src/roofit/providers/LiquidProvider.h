/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ParaProvider.h
 * Author: petrstepanov
 *
 * Created on December 6, 2018, 1:03 PM
 */

#ifndef LIQUIDPROVIDER_H
#define LIQUIDPROVIDER_H

#include "AbstractProvider.h"
#include "RooRealVar.h"

class LiquidProvider : public AbstractProvider{
public:
    LiquidProvider(RooRealVar* observable, RooRealVar* channelWidth);
    virtual ~LiquidProvider();

private:
    RooAbsPdf* initPdf(int i);
};

#endif /* LIQUIDPROVIDER_H */

