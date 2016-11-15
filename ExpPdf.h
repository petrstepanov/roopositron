/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ExpPdf.h
 * Author: petrstepanov
 *
 * Created on November 9, 2016, 7:49 PM
 */

#ifndef MY_EXPPDF
#define MY_EXPPDF

#include "RooAbsReal.h"
#include "RooAbsPdf.h"
#include "TObject.h"
#include "RooRealProxy.h"

class ExpPdf : public RooAbsPdf {
public:
	ExpPdf() {};
	ExpPdf(const char *name, const char *title,
		RooAbsReal& _t,
		RooAbsReal& _tau
		);
	ExpPdf(const ExpPdf& other, const char* name = 0);
	virtual TObject* clone(const char* newname) const { return new ExpPdf(*this, newname); }
	inline virtual ~ExpPdf() { }

protected:
	RooRealProxy t;
	RooRealProxy tau;

	Double_t evaluate() const;

private:

	// ClassDef(ExpPdf, 1) // Your description goes here...
};

#endif /* MY_EXPPDF */

