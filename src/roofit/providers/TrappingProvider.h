/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TrappingProvider.h
 * Author: petrstepanov
 *
 * Created on December 6, 2018, 1:02 PM
 */

#ifndef TRAPPINGPROVIDER_H
#define TRAPPINGPROVIDER_H

#include "AbstractProvider.h"
#include "RooRealVar.h"

class TrappingProvider : public AbstractProvider {
public:
    TrappingProvider(RooRealVar* observable);
    virtual ~TrappingProvider();
    
private:
    RooAbsPdf* initPdf() override;
};

#endif /* TRAPPINGPROVIDER_H */

