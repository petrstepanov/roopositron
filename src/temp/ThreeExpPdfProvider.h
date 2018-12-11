/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ThreeExpPdfProvider.h
 * Author: petrstepanov
 *
 * Created on December 6, 2018, 1:10 PM
 */

#ifndef THREEEXPPDFPROVIDER_H
#define THREEEXPPDFPROVIDER_H

#include "../roofit/providers/AbstractProvider.h"

class ThreeExpPdfProvider : public AbstractProvider {
public:
    ThreeExpPdfProvider(RooRealVar* observable);
    virtual ~ThreeExpPdfProvider();
    
private:
//    RooArgList* initParameters() override;
    RooAbsPdf* initPdf(int i) override;
};

#endif /* THREEEXPPDFPROVIDER_H */

