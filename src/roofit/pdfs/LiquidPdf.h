/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   LiquidPdf.h
 * Author: petrstepanov
 *
 * Created on November 9, 2016, 8:48 PM
 */

#ifndef MY_LIQUIDPDF
#define MY_LIQUIDPDF

#include "RooAbsReal.h"
#include "RooAbsPdf.h"
#include "TObject.h"
#include "RooRealProxy.h"
#include "TH1F.h"
#include "TAxis.h"
#include "TMath.h"

class LiquidPdf: public RooAbsPdf {
public:
	LiquidPdf() {
	}
	;
	LiquidPdf(const char *name, const char *title,
			RooAbsReal& _t,              // Time observable
			RooAbsReal& _Pqf,            // Probability of forming qf-Ps
			RooAbsReal& _lb,             // Transformation rate of qf-Ps in to Ps
			RooAbsReal& _lqf,            // Annihilation rate of qf-Ps
			RooAbsReal& _lplus,          // Annihilation rate of solvated positrons
			RooAbsReal& _lpo,            // Pick-off annihilation rate
			RooAbsReal& _lopc,           // Ortho-para conversion rate
			RooAbsReal& _lox,            // Oxidation rate
			RooAbsReal& _l2g,            // Two-gamma annihilation rate of p-Ps
			RooAbsReal& _chW);
	LiquidPdf(const LiquidPdf& other, const char* name = 0);
	virtual TObject* clone(const char* newname) const {
		return new LiquidPdf(*this, newname);
	}
	inline virtual ~LiquidPdf() {
	}

	Int_t getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars,
			const char* rangeName = 0) const;
	Double_t analyticalIntegral(Int_t code, const char* rangeName = 0) const;

protected:
	RooRealProxy t;
	RooRealProxy Pqf;
	RooRealProxy lb;
	RooRealProxy lqf;
	RooRealProxy lplus;
	RooRealProxy lpo;
	RooRealProxy lopc;
	RooRealProxy lox;
	RooRealProxy l2g;
	RooRealProxy chW;

	Double_t evaluate() const;
	Double_t sum(Double_t a) const;
	Double_t Power(Double_t a, Double_t b) const;
	Double_t Sqrt(Double_t a) const;
	Double_t ArcTan(Double_t a) const;
	Double_t Coth(Double_t a) const;
	Double_t Log(Double_t a) const;
	Double_t indefiniteIntegral(Double_t x) const;

private:
ClassDef(LiquidPdf, 1)
};

#endif /* MY_LIQUIDPDF */

