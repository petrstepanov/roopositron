/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   BackgroundProvider.h
 * Author: petrstepanov
 *
 * Created on December 8, 2018, 1:32 PM
 */

#ifndef BACKGROUNDPROVIDER_H
#define BACKGROUNDPROVIDER_H

#include "AbstractProvider.h"
#include "RooRealVar.h"
#include "RooAbsPdf.h"

class BackgroundProvider : public AbstractProvider {
public:
    BackgroundProvider(RooRealVar* observable, TH1F* histogram);
    virtual ~BackgroundProvider();

    RooAbsPdf* initPdf();
    
private: 
    TH1F* histogram;
};


#endif /* BACKGROUNDPROVIDER_H */

