/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TwoExpPdf.h
 * Author: petrstepanov
 *
 * Created on November 9, 2016, 7:52 PM
 */

#ifndef MY_TWOEXPPDF
#define MY_TWOEXPPDF

#include "RooAbsReal.h"
#include "RooAbsPdf.h"
#include "TObject.h"
#include "RooRealProxy.h"
#include "TMath.h"

class TwoExpPdf : public RooAbsPdf {
public:
    TwoExpPdf() {};
    TwoExpPdf(const char *name, const char *title,
        RooAbsReal& _t,
        RooAbsReal& _tau1,
        RooAbsReal& _tau2,
        RooAbsReal& _i2
        );
    TwoExpPdf(const TwoExpPdf& other, const char* name = 0);
    virtual TObject* clone(const char* newname) const { return new TwoExpPdf(*this, newname); }
    inline virtual ~TwoExpPdf() { }

    Int_t getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* rangeName=0) const ;
    Double_t analyticalIntegral(Int_t code, const char* rangeName=0) const ;
        
protected:
    RooRealProxy t;
    RooRealProxy tau1;
    RooRealProxy tau2;
    RooRealProxy i2;

    Double_t evaluate() const;
    Double_t indefiniteIntegral(Double_t y) const;
private:

	// ClassDef(TwoExpPdf, 1) // Your description goes here...
};

#endif /* MY_TWOEXPPDF */

