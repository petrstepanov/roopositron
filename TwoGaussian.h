/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TwoGaussian.h
 * Author: petrstepanov
 *
 * Created on November 9, 2016, 7:42 PM
 */

#ifndef MY_TWOGAUSSIAN
#define MY_TWOGAUSSIAN

#include "RooAbsReal.h"
#include "RooAbsPdf.h"
#include "TObject.h"
#include "RooRealProxy.h"

class TwoGaussian : public RooAbsPdf {
public:
	TwoGaussian() {    };
	TwoGaussian(const char *name, const char *title,
		RooAbsReal& _x, RooAbsReal& _mean1, RooAbsReal& _sigma1, RooAbsReal& _mean2, RooAbsReal& _sigma2, RooAbsReal& _i2);
	TwoGaussian(const TwoGaussian& other, const char* name = 0);
	virtual TObject* clone(const char* newname) const { return new TwoGaussian(*this, newname); }
	inline virtual ~TwoGaussian() {  }

protected:

	RooRealProxy x;
	RooRealProxy mean1;
	RooRealProxy sigma1;
	RooRealProxy mean2;
	RooRealProxy sigma2;
	RooRealProxy i2;

	Double_t evaluate() const;

private:

	// ClassDef(TwoGaussian, 1) // Gaussian PDF
};

#endif /* MY_TWOGAUSSIAN */

