/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ParaPdf.h
 * Author: petrstepanov
 *
 * Created on November 9, 2016, 8:48 PM
 */

#ifndef MY_PARAPDF
#define MY_PARAPDF

#include "RooAbsReal.h"
#include "RooAbsPdf.h"
#include "TObject.h"
#include "RooRealProxy.h"
#include "TH1F.h"
#include "TAxis.h"
#include "TMath.h"

class ParaPdf: public RooAbsPdf {
public:
	ParaPdf() {
	}
	;
	ParaPdf(const char *name, const char *title, RooAbsReal& _t,
			RooAbsReal& _Pps,          // Probability of forming qf-Ps
			RooAbsReal& _lplus,        // Probability of observation a thermalized positron
			RooAbsReal& _lpo,          // Pick-off annihilation speed
			RooAbsReal& _l2g,          // Two-gamma annihilation rate
			RooAbsReal& _lopc,         // Ortho-para conversion speed
			RooAbsReal& _chW);
	ParaPdf(const ParaPdf& other, const char* name = 0);
	virtual TObject* clone(const char* newname) const {
		return new ParaPdf(*this, newname);
	}
	inline virtual ~ParaPdf() {
	}

	Int_t getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars,
			const char* rangeName = 0) const;
	Double_t analyticalIntegral(Int_t code, const char* rangeName = 0) const;

protected:
	RooRealProxy t;
	RooRealProxy Pps;
	RooRealProxy lplus;
	RooRealProxy lpo;
	RooRealProxy l2g;
	RooRealProxy lopc;
	RooRealProxy chW;

	Double_t evaluate() const;
	Double_t sum(Double_t a) const;
	Double_t Power(Double_t a, Double_t b) const;
	Double_t Sqrt(Double_t a) const;
	Double_t ArcTan(Double_t a) const;
	Double_t Coth(Double_t a) const;
	Double_t Log(Double_t a) const;
	Double_t indefiniteIntegral(Double_t y) const;

private:
ClassDef(ParaPdf, 1)
};

#endif /* MY_PARAPDF */

