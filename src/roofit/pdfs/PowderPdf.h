/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PowderPdf.h
 * Author: petrstepanov
 *
 * Created on November 9, 2016, 8:48 PM
 */

#ifndef MY_POWDERPDF
#define MY_POWDERPDF

#include "RooAbsReal.h"
#include "RooAbsPdf.h"
#include "TObject.h"
#include "RooRealProxy.h"
#include "TH1F.h"
#include "TAxis.h"
#include "TMath.h"

class PowderPdf : public RooAbsPdf {
public:
    PowderPdf() {};
    PowderPdf(const char *name, const char *title,
            RooAbsReal& _t,
            RooAbsReal& _L,            // Diameter of the powder
            RooAbsReal& _kv,           // Probability of capturing into vacancy
            RooAbsReal& _Pps,          // Probability of forming qf-Ps  
            RooAbsReal& _lb,           // Annihilation rate in bulk
            RooAbsReal& _lv,           // Annihilation rate in vacancy
            RooAbsReal& _lPs,          // Annihilation rate of Ps atom	    
            RooAbsReal& _Mr,           // Powder mass to e+ mass ratio ~1E5?
            RooAbsReal& _mu            // Probability of o-Ps -> p-Ps conversion due to paramagnetic O2 
            );
    PowderPdf(const PowderPdf& other, const char* name = 0);
    virtual TObject* clone(const char* newname) const { return new PowderPdf(*this, newname); }
    inline virtual ~PowderPdf() { }     

//    Int_t getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* rangeName=0) const ;
//    Double_t analyticalIntegral(Int_t code, const char* rangeName=0) const ;
    
protected:
    RooRealProxy t;
    RooRealProxy L;
    RooRealProxy kv;
    RooRealProxy Pps;
    RooRealProxy lb;
    RooRealProxy lv;
    RooRealProxy lPs;
    RooRealProxy Mr;
    RooRealProxy mu;    
    
    Double_t evaluate() const;
    Double_t sum(Double_t a) const;     
    Double_t Power(Double_t a, Double_t b) const;
//    Double_t indefiniteIntegral(Double_t y) const; 
    
private:
    // ClassDef(PowderPdf, 1) // Your description goes here...
};

#endif /* MY_POWDERPDF */

