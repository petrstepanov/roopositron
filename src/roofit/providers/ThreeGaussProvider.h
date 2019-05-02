/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ThreeGaussProvider.h
 * Author: petrstepanov
 *
 * Created on December 6, 2018, 10:48 AM
 */

#ifndef THREEGAUSSPROVIDER_H
#define THREEGAUSSPROVIDER_H

#include "AbstractProvider.h"
#include "RooRealVar.h"

class ThreeGaussProvider : public AbstractProvider {
public:
    ThreeGaussProvider(RooRealVar* observable, RooRealVar* channelWidth);
    virtual ~ThreeGaussProvider();
    
private:
    RooAbsPdf* initPdf(int i);
};

#endif /* THREEGAUSSPROVIDER_H */

