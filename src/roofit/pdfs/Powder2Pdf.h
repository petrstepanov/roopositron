/*
 * Powder2.h
 *
 *  Created on: Feb 7, 2019
 *      Author: student
 */

#ifndef SRC_ROOFIT_PDFS_POWDER2PDF_H_
#define SRC_ROOFIT_PDFS_POWDER2PDF_H_

#include "RooAbsReal.h"
#include "RooAbsPdf.h"
#include "TObject.h"
#include "RooRealProxy.h"
#include "TH1F.h"
#include "TAxis.h"
#include "TMath.h"

class Powder2Pdf: public RooAbsPdf {
public:
	Powder2Pdf() {
	}
	;
	Powder2Pdf(const char *name, const char *title, //
			RooAbsReal& _t,            // Observable
			RooAbsReal& _L,            // Diameter of the powder
			RooAbsReal& _kd1,          // Probability of capturing in defect type 1
			RooAbsReal& _kd2,          // Probability of capturing in defect type 2
			RooAbsReal& _Pps,          // Probability of forming qf-Ps
			RooAbsReal& _lb,           // Annihilation rate in bulk
			RooAbsReal& _ld1,          // Annihilation rate in defect 1
			RooAbsReal& _ld2,          // Annihilation rate in defect 2
			RooAbsReal& _lPs,          // Annihilation rate of Ps atom
			RooAbsReal& _Mr,           // Powder mass to e+ mass ratio ~1E5?
			RooAbsReal& _mu,           // Probability of o-Ps -> p-Ps conversion due to paramagnetic O2
			RooAbsReal& _l2g,          // Two-gamma annihilation rate
			RooAbsReal& _V0,           // Two-gamma annihilation rate
			RooAbsReal& _Vth,          // Three-gamma annihilation rate
			Double_t chW);
	Powder2Pdf(const Powder2Pdf& other, const char* name = 0);
	virtual TObject* clone(const char* newname) const {
		return new Powder2Pdf(*this, newname);
	}
	inline virtual ~Powder2Pdf() {
	}

	Int_t getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* rangeName = 0) const;
	Double_t analyticalIntegral(Int_t code, const char* rangeName = 0) const;

protected:
	RooRealProxy t;
	RooRealProxy L;
	RooRealProxy kd1;
	RooRealProxy kd2;
	RooRealProxy Pps;
	RooRealProxy lb;
	RooRealProxy ld1;
	RooRealProxy ld2;
	RooRealProxy lPs;
	RooRealProxy Mr;
	RooRealProxy mu;
	RooRealProxy l2g;
	RooRealProxy V0;
	RooRealProxy Vth;

	Double_t chW = 0;
	Double_t evaluate() const;
	Double_t sum(Double_t a) const;
	Double_t Power(Double_t a, Double_t b) const;
	Double_t Sqrt(Double_t a) const;
	Double_t ArcTan(Double_t a) const;
	Double_t Coth(Double_t a) const;
	Double_t Log(Double_t a) const;
	Double_t indefiniteIntegral(Double_t y) const;

private:
ClassDef(Powder2Pdf, 1)
};

#endif /* SRC_ROOFIT_PDFS_POWDER2PDF_H_ */
