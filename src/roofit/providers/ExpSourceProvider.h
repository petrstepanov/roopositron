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

#ifndef EXPSOURCEPROVIDER_H
#define EXPSOURCEPROVIDER_H

#include "AbstractProvider.h"
#include "RooRealVar.h"

class ExpSourceProvider : public AbstractProvider {
public:
    ExpSourceProvider(RooRealVar* observable);
    virtual ~ExpSourceProvider();
    
private:
//    RooArgList* initParameters() override;
    RooAbsPdf* initPdf(int i);
};

#endif /* EXPSOURCEPROVIDER_H */

