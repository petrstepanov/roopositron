/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ConstantPdf.h
 * Author: petrstepanov
 *
 * Created on November 9, 2016, 7:49 PM
 */

#ifndef MY_CPDF
#define MY_CPDF

#include "RooAbsReal.h"
#include "RooAbsPdf.h"
#include "TObject.h"
#include "RooRealProxy.h"

class ConstantPdf : public RooAbsPdf {
public:
	ConstantPdf() {};
	ConstantPdf(const char *name, const char *title,
		RooAbsReal& _t,
		RooAbsReal& _bins
		);
	ConstantPdf(const ConstantPdf& other, const char* name = 0);
	virtual TObject* clone(const char* newname) const { return new ConstantPdf(*this, newname); }
	inline virtual ~ConstantPdf() { }

    Int_t getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* rangeName=0) const ;
    Double_t analyticalIntegral(Int_t code, const char* rangeName=0) const ;
 
protected:
    RooRealProxy t;
    RooRealProxy bins;

    Double_t evaluate() const;
    Double_t indefiniteIntegral(Double_t y) const;
private:

	ClassDef(ConstantPdf, 1)
};

#endif /* MY_CPDF */

