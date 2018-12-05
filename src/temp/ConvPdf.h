/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ConvPdf.h
 * Author: petrstepanov
 *
 * Created on December 4, 2016, 12:14 AM
 */

#ifndef MY_CONVPDF_H
#define MY_CONVPDF_H

#include "RooAbsReal.h"
#include "RooRealVar.h"
#include "RooAbsPdf.h"
#include "TObject.h"
#include "RooRealProxy.h"

class ConvPdf : public RooAbsPdf {
public:
	ConvPdf() {};
	ConvPdf(const char *name, const char *title,
		RooAbsReal& _t,
		RooAbsPdf* _f,
                RooAbsPdf* _g
		);
	ConvPdf(const ConvPdf& other, const char* name = 0);
	virtual TObject* clone(const char* newname) const { return new ConvPdf(*this, newname); }
	inline virtual ~ConvPdf() { }
        void init (int minChannel, int maxChannel);
        
protected:
	RooRealProxy t;
        RooAbsPdf* f;
        RooAbsPdf* g;
        int minChannel;
        int maxChannel;
        
	Double_t evaluate() const;

private:

	// ClassDef(ConvPdf, 1) // Your description goes here...
};

#endif /* MY_EXPPDF */

