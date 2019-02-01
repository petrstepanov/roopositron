/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GrainPdf.h
 * Author: petrstepanov
 *
 * Created on November 9, 2016, 8:48 PM
 */

#ifndef MY_GRAINPDF
#define MY_GRAINPDF

#include "RooAbsReal.h"
#include "RooAbsPdf.h"
#include "TObject.h"
#include "RooRealProxy.h"
#include "TH1F.h"
#include "TAxis.h"
#include "TMath.h"

class GrainPdf : public RooAbsPdf {
public:
    GrainPdf() {};
    GrainPdf(const char *name, const char *title,
            RooAbsReal& _t,
            RooAbsReal& _lambdaJ,
            RooAbsReal& _lambdaBulk,
            RooAbsReal& _lambdaGrain,
            RooAbsReal& _lambdaVac,
            RooAbsReal& _kappaVac
            );
    GrainPdf(const GrainPdf& other, const char* name = 0);
    virtual TObject* clone(const char* newname) const { return new GrainPdf(*this, newname); }
    inline virtual ~GrainPdf() { }     

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
    Double_t sum(Double_t a) const;     
    Double_t Power(Double_t a, Double_t b) const;
    Double_t indefiniteIntegral(Double_t y) const; 
    
private:
    ClassDef(GrainPdf, 1)
};

#endif /* MY_GRAINPDF */

