/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GrainProvider.h
 * Author: petrstepanov
 *
 * Created on December 6, 2018, 1:03 PM
 */

#ifndef GRAINPROVIDER_H
#define GRAINPROVIDER_H

#include "AbstractProvider.h"
#include "RooRealVar.h"

class GrainProvider : public AbstractProvider{
public:
    GrainProvider(RooRealVar *observable);
    virtual ~GrainProvider();

private:
    RooAbsPdf* initPdf() override;
};

#endif /* GRAINPROVIDER_H */

