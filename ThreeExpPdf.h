/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ThreeExpPdf.h
 * Author: petrstepanov
 *
 * Created on November 9, 2016, 7:52 PM
 */

#ifndef MY_THREEEXPPDF
#define MY_THREEEXPPDF

#include "RooAbsReal.h"
#include "RooAbsPdf.h"
#include "TObject.h"
#include "RooRealProxy.h"
#include "TMath.h"

class ThreeExpPdf : public RooAbsPdf {
public:
    ThreeExpPdf() {};
    ThreeExpPdf(const char *name, const char *title,
        RooAbsReal& _t,
        RooAbsReal& _tau1,
        RooAbsReal& _tau2,
        RooAbsReal& _tau3,
        RooAbsReal& _i2,
        RooAbsReal& _i3            
        );
    ThreeExpPdf(const ThreeExpPdf& other, const char* name = 0);
    virtual TObject* clone(const char* newname) const { return new ThreeExpPdf(*this, newname); }
    inline virtual ~ThreeExpPdf() { }

    Int_t getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* rangeName=0) const ;
    Double_t analyticalIntegral(Int_t code, const char* rangeName=0) const ;
        
protected:
    RooRealProxy t;
    RooRealProxy tau1;
    RooRealProxy tau2;
    RooRealProxy tau3;
    RooRealProxy i2;
    RooRealProxy i3;
    
    Double_t evaluate() const;
    Double_t indefiniteIntegral(Double_t y) const;
private:

	// ClassDef(ThreeExpPdf, 1) // Your description goes here...
};

#endif /* MY_THREEEXPPDF */

