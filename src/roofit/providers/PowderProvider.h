/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PowderProvider.h
 * Author: petrstepanov
 *
 * Created on December 6, 2018, 1:03 PM
 */

#ifndef POWDERPROVIDER_H
#define POWDERPROVIDER_H

#include "AbstractProvider.h"
#include "RooRealVar.h"

class PowderProvider : public AbstractProvider{
public:
    PowderProvider(RooRealVar *observable);
    virtual ~PowderProvider();

private:
    RooAbsPdf* initPdf(int i);
};

#endif /* POWDERPROVIDER_H */

