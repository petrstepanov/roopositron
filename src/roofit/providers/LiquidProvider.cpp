/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ParaProvider.cpp
 * Author: petrstepanov
 * 
 * Created on December 6, 2018, 1:03 PM
 */

#include "LiquidProvider.h"

#include "RooFormulaVar.h"
#include "RooConstVar.h"
#include "../../model/Constants.h"
#include "../pdfs/LiquidPdf.h"

LiquidProvider::LiquidProvider(RooRealVar* _observable, RooRealVar* _channelWidth) : AbstractProvider(_observable, _channelWidth) {}

LiquidProvider::~LiquidProvider() {
}

RooAbsPdf* LiquidProvider::initPdf(int i) {
//	RooAbsReal& _t,              // Time observable
//	RooAbsReal& _Pqf,            // Probability of forming qf-Ps
//	RooAbsReal& _lb,             // Transformation rate of qf-Ps in to Ps
//	RooAbsReal& _lqf,            // Annihilation rate of qf-Ps
//	RooAbsReal& _lplus,          // Annihilation rate of solvated positrons
//	RooAbsReal& _lpo,            // Pick-off annihilation rate
//	RooAbsReal& _lopc,           // Ortho-para conversion rate
//	RooAbsReal& _lox,            // Oxidation rate
//	RooAbsReal& _l2g,            // Two-gamma annihilation rate of p-Ps
//	RooAbsReal& _chW);

	RooRealVar* Pqf = new RooRealVar("Pqf", "Probability of forming qf-Ps", 0.5, 0, 1, "");
	RooRealVar* lb = new RooRealVar("#lambda_b", " Transformation rate of qf-Ps in to Ps", 1, 1E-3, 1E3, "1/ns");
	RooRealVar* lqf = new RooRealVar("#lambda_qf", "Annihilation rate of qf-Ps", 1, 1E-3, 1E3, "1/ns");
	RooRealVar* lplus = new RooRealVar("#lambda_+", "Annihilation rate of solvated positron", 1, 1E-3, 1E3, "1/ns");
	RooRealVar* lpo = new RooRealVar("#lambda_po", "Pick-off annihilation rate", 1, 1E-3, 1E3, "1/ns");

	RooRealVar* cO2 = new RooRealVar("c_O2", "Concentration of oxygen molecules", 1, 1E-5, 0.1, "M/l");
	RooRealVar* kopc = new RooRealVar("k_opc", "Ortho-para conversion rate constant", 5E10, 1E8, 1E12, "l/(M*ns)");
    RooFormulaVar* lopc = new RooFormulaVar("#lambda_opc", "@0*@1", RooArgList(*cO2, *kopc));
	RooRealVar* kox = new RooRealVar("k_ox", "Oxidation reaction rate constant", 5E10, 1E8, 1E12, "l/(M*ns)");
    RooFormulaVar* lox = new RooFormulaVar("#lambda_ox", "@0*@1", RooArgList(*cO2, *kox));

	RooRealVar* l2g = new RooRealVar("#lambda_2#gamma", "Two-gamma annihilation rate", 8, "1/ns");
	l2g->setConstant(kTRUE);

	return new LiquidPdf("liquid", "Liquid pdf", *observable, *Pqf, *lb, *lqf, *lplus, *lpo, *lopc, *lox, *l2g, *channelWidth);
}
