/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SourceProvider.h
 * Author: petrstepanov
 *
 * Created on December 5, 2018, 8:43 AM
 */

#ifndef EXPPROVIDER_H
#define EXPPROVIDER_H

#include "AbstractProvider.h"
#include "RooRealVar.h"

class SourceProvider : public AbstractProvider {
public:
    SourceProvider(RooRealVar* observable);
    virtual ~SourceProvider();
    
private:
//    RooArgList* initParameters() override;
    RooAbsPdf* initPdf() override;
};

#endif /* EXPPROVIDER_H */

