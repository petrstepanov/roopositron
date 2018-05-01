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
    
protected:
    RooRealProxy t;
    RooRealProxy lambdaJ;
    RooRealProxy lambdaBulk;
    RooRealProxy lambdaGrain;
    RooRealProxy lambdaVac;
    RooRealProxy kappaVac;

    Double_t evaluate() const;
    
private:
    // ClassDef(GrainPdf, 1) // Your description goes here...
};

#endif /* MY_GRAINPDF */

