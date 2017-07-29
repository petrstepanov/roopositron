/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MyConvPdf.h
 * Author: petrstepanov
 *
 * Created on November 9, 2016, 7:49 PM
 */

#ifndef MY_CONVPDF
#define MY_CONVPDF

#include "RooRealVar.h"
#include "RooAbsReal.h"
#include "RooAbsPdf.h"
#include "TObject.h"
#include "RooRealProxy.h"

class MyConvPdf : public RooAbsPdf {
public:
	MyConvPdf() {};
	MyConvPdf(const char *name, const char *title,
		RooAbsReal& _t,
		RooAbsPdf* _f,
		RooAbsPdf* _g
		);
	MyConvPdf(const MyConvPdf& other, const char* name = 0);
	virtual TObject* clone(const char* newname) const { return new MyConvPdf(*this, newname); }
	inline virtual ~MyConvPdf() { }
        void setPrecision(Int_t np);

protected:
	RooRealProxy t;
	RooAbsPdf* f;
	RooAbsPdf* g;
        Double_t gMin;
        Double_t gMax;
        Double_t step;
        Int_t np;
	Double_t evaluate() const;
        const char* obsName;
        RooRealVar* getPdfObservable(RooAbsPdf*, const char* name) const;
        
private:

	// ClassDef(MyConvPdf, 1) // Your description goes here...
};

#endif /* MY_CONVPDF */

