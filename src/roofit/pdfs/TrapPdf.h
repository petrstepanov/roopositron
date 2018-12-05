/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TrapPdf.h
 * Author: petrstepanov
 *
 * Created on November 17, 2016, 1:59 AM
 */

#ifndef MY_TRAPPDF
#define MY_TRAPPDF

#include "RooAbsReal.h"
#include "RooAbsPdf.h"
#include "TObject.h"
#include "RooRealProxy.h"

class TrapPdf : public RooAbsPdf {
public:
    TrapPdf() {};
    TrapPdf(const char *name, const char *title,
        RooAbsReal& _t,
        RooAbsReal& _tauBulk,
        RooAbsReal& _tauDef,
        RooAbsReal& _kappaDef
        );
    TrapPdf(const TrapPdf& other, const char* name = 0);
    virtual TObject* clone(const char* newname) const { return new TrapPdf(*this, newname); }
    inline virtual ~TrapPdf() { }

    Int_t getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* rangeName=0) const ;
    Double_t analyticalIntegral(Int_t code, const char* rangeName=0) const ;        
        
protected:
    RooRealProxy t;
    RooRealProxy tauBulk;
    RooRealProxy tauDef;
    RooRealProxy kappaDef;

    Double_t evaluate() const;
    Double_t indefiniteIntegral(Double_t y) const;
    
private:
    // ClassDef(TrapPdf, 1) // Your description goes here...
};

#endif /* MY_TRAPPDF */
