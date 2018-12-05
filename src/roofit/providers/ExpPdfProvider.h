/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ExpPdfProvider.h
 * Author: petrstepanov
 *
 * Created on December 5, 2018, 8:43 AM
 */

#ifndef EXPPDFPROVIDER_H
#define EXPPDFPROVIDER_H

#include "AbstractProvider.h"
#include "RooArgList.h"
#include "RooRealVar.h"

class ExpPdfProvider : AbstractProvider {
public:
    ExpPdfProvider(RooRealVar* observable, ParamStorage* paramStorage);
    virtual ~ExpPdfProvider();
    
private:
//    RooArgList* initParameters() override;
    RooAbsPdf* initPdf() override;
};

#endif /* EXPPDFPROVIDER_H */

