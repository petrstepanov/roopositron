/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TwoTwoExpPdfProvider.h
 * Author: petrstepanov
 *
 * Created on December 6, 2018, 1:09 PM
 */

#ifndef TWOEXPPDFPROVIDER_H
#define TWOEXPPDFPROVIDER_H

#include "../roofit/providers/AbstractProvider.h"

class TwoExpPdfProvider : public AbstractProvider {
public:
    TwoExpPdfProvider(RooRealVar* observable);
    virtual ~TwoExpPdfProvider();
    
private:
//    RooArgList* initParameters() override;
    RooAbsPdf* initPdf(int i) override;
};

#endif /* TWOEXPPDFPROVIDER_H */

