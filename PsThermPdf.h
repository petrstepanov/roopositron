/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PsThermPdf.h
 * Author: petrstepanov
 *
 * Created on December 3, 2018, 6:05 AM
 */

#ifndef PSTHERMPDF_H
#define PSTHERMPDF_H


#include "RooAbsReal.h"
#include "RooAbsPdf.h"
#include "TObject.h"
#include "RooRealProxy.h"
#include "TH1F.h"
#include "TAxis.h"
#include "TMath.h"

class PsThermPdf : public RooAbsPdf {
public:
    PsThermPdf() {};
    PsThermPdf(const char *name, const char *title,
            RooAbsReal& _t,
            RooAbsReal& _lambdaJ,
            RooAbsReal& _lambdaBulk,
            RooAbsReal& _lambdaGrain,
            RooAbsReal& _lambdaVac,
            RooAbsReal& _kappaVac
            );
    PsThermPdf(const PsThermPdf& other, const char* name = 0);
    virtual TObject* clone(const char* newname) const { return new PsThermPdf(*this, newname); }
    inline virtual ~PsThermPdf() { }     

    Int_t getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* rangeName=0) const ;
    Double_t analyticalIntegral(Int_t code, const char* rangeName=0) const ;
    
protected:
    RooRealProxy t;
    RooRealProxy lambdaJ;
    RooRealProxy lambdaBulk;
    RooRealProxy lambdaGrain;
    RooRealProxy lambdaVac;
    RooRealProxy kappaVac;

    Double_t evaluate() const;
//    Double_t sum(Double_t a) const;     
//    Double_t Power(Double_t a, Double_t b) const;
    Double_t indefiniteIntegral(Double_t y) const; 
    
private:
    // ClassDef(PsThermPdf, 1) // Your description goes here...
};

#endif /* PSTHERMPDF_H */

