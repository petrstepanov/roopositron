/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ThreeGaussian.h
 * Author: petrstepanov
 *
 * Created on November 9, 2016, 7:42 PM
 */

#ifndef MY_THREEGAUSSIAN
#define MY_THREEGAUSSIAN

#include "RooAbsReal.h"
#include "RooAbsPdf.h"
#include "TObject.h"
#include "RooRealProxy.h"
#include "TMath.h"
#include "RooMath.h"

class ThreeGaussian : public RooAbsPdf {
public:
    ThreeGaussian() {};
    ThreeGaussian(const char *name, const char *title,
            RooAbsReal& _x, RooAbsReal& _mean, RooAbsReal& _sigma1, RooAbsReal& _sigma2, RooAbsReal& _sigma3, RooAbsReal& _i2, RooAbsReal& _i3);
    ThreeGaussian(const ThreeGaussian& other, const char* name = 0);
    virtual TObject* clone(const char* newname) const { return new ThreeGaussian(*this, newname); }
    inline virtual ~ThreeGaussian() {}

    Int_t getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* rangeName=0) const ;
    Double_t analyticalIntegral(Int_t code, const char* rangeName=0) const ;
    
protected:
    RooRealProxy x;
    RooRealProxy mean;
    RooRealProxy sigma1;
    RooRealProxy sigma2;
    RooRealProxy sigma3;
    RooRealProxy i2;
    RooRealProxy i3;        

    Double_t evaluate() const;
    Double_t indefiniteIntegral(Double_t y) const;    

private:
    ClassDef(ThreeGaussian, 1)
};

#endif /* MY_TWOGAUSSIAN */

