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
		RooAbsReal& _tauVac,
		RooAbsReal& _kappaVac
		);
	TrapPdf(const TrapPdf& other, const char* name = 0);
	virtual TObject* clone(const char* newname) const { return new TrapPdf(*this, newname); }
	inline virtual ~TrapPdf() { }

protected:
	RooRealProxy t;
	RooRealProxy tauBulk;
	RooRealProxy tauVac;
	RooRealProxy kappaVac;
        
	Double_t evaluate() const;

private:

	// ClassDef(TrapPdf, 1) // Your description goes here...
};

#endif /* MY_EXPPDF */
