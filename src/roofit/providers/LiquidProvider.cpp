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

#include <RooFormulaVar.h>
#include <RooConstVar.h>
#include <RooArgSet.h>
#include "../../model/Constants.h"
#include "../pdfs/LiquidPdf.h"
#include "../../util/RootHelper.h"


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
	RooRealVar* kopc = new RooRealVar("k_opc", "Ortho-para conversion rate constant", 10, 0, 100, "l/(M*ns)");
	RooRealVar* kox = new RooRealVar("k_ox", "Oxidation reaction rate constant", 10, 0, 100, "l/(M*ns)");
	RooRealVar* l2g = new RooRealVar("#lambda_2#gamma", "Two-gamma annihilation rate", 8, 8, 8, "1/ns");

    RooFormulaVar* lopc = new RooFormulaVar("#lambda_opc", "@0*@1", RooArgList(*cO2, *kopc));
	RooFormulaVar* lox = new RooFormulaVar("#lambda_ox", "@0*@1", RooArgList(*cO2, *kox));

	l2g->setConstant(kTRUE);

	LiquidPdf* pdf = new LiquidPdf("liquid", "Liquid pdf", *observable, *Pqf, *lb, *lqf, *lplus, *lpo, *lopc, *lox, *l2g, *channelWidth);
	return pdf;
}

RooArgSet* LiquidProvider::getIndirectParameters(RooAbsPdf* pdf){
    RooArgSet* indirectParameters = new RooArgSet();

    // pdf parameters might have suffixed names so we account on that
    // for "Int_exp2" we pull "Int_exp2_##" as well

	RooRealVar* Pqf = RootHelper::getParameterByNameCommonOrLocal(pdf, "Pqf");
	RooRealVar* lb = RootHelper::getParameterByNameCommonOrLocal(pdf, "#lambda_b");
	RooRealVar* lqf = RootHelper::getParameterByNameCommonOrLocal(pdf, "#lambda_qf");
	RooRealVar* lplus = RootHelper::getParameterByNameCommonOrLocal(pdf, "#lambda_+");
	RooRealVar* lpo = RootHelper::getParameterByNameCommonOrLocal(pdf, "#lambda_po");

	RooRealVar* cO2 = RootHelper::getParameterByNameCommonOrLocal(pdf, "c_O2");
	RooRealVar* kopc = RootHelper::getParameterByNameCommonOrLocal(pdf, "k_opc");
	RooRealVar* kox = RootHelper::getParameterByNameCommonOrLocal(pdf, "k_ox");
	RooRealVar* l2g = RootHelper::getParameterByNameCommonOrLocal(pdf, "#lambda_2#gamma");

	if (Pqf && lb && lqf && lplus && lpo && cO2 && kopc && kox && l2g){
		RooFormulaVar* lopc = new RooFormulaVar("_#lambda_opc", "@0*@1", RooArgList(*cO2, *kopc));
		RooFormulaVar* lox = new RooFormulaVar("_#lambda_ox", "@0*@1", RooArgList(*cO2, *kox));

		RooFormulaVar* lq = new RooFormulaVar("lq", "@0+@1", RooArgList(*lqf, *lb));
    	RooFormulaVar* lo = new RooFormulaVar("lo", "@0+@1/4+@2", RooArgList(*lpo, *lopc, *lox));
    	RooFormulaVar* lp = new RooFormulaVar("lp", "@0+@1", RooArgList(*l2g, *lpo));
    	RooFormulaVar* nu = new RooFormulaVar("nu", "3*@0/4*@1/(@2-@3)", RooArgList(*Pqf, *lb, *lq, *lo));

    	RooArgList* I0ArgList = new RooArgList();
    	I0ArgList->add(*Pqf);  /*0*/
    	I0ArgList->add(*lqf);  /*1*/
    	I0ArgList->add(*lq);   /*2*/
    	I0ArgList->add(*lp);   /*3*/
    	I0ArgList->add(*lb);   /*4*/
    	I0ArgList->add(*nu);   /*5*/
    	I0ArgList->add(*lopc); /*6*/
    	I0ArgList->add(*lplus);/*7*/
    	I0ArgList->add(*lox);  /*8*/
    	I0ArgList->add(*lpo);  /*9*/

    	RooFormulaVar* I0 = new RooFormulaVar("I_0", "@0*@1/@2-@0/4*@3*@4/@2/(@2-@3)+@5*@3*@6/4/@2/(@2-@3)+@5*@7*@8/@2/(@2-@7)-@5*@9/@2", *I0ArgList);
    	RooFormulaVar* I1 = new RooFormulaVar("I_1", "@0/4*@1/(@2-@3)-@4*@5/4/(@3-@6)-@4", RooArgList(*Pqf /*0*/, *lb /*1*/, *lq /*2*/, *lp /*3*/, *nu /*4*/, *lopc /*5*/, *lo /*6*/));
    	RooFormulaVar* I2 = new RooFormulaVar("I_2", "1-@0-@1*@2/(@3-@4)-@1*@2/(@q-@3)", RooArgList(*Pqf /*0*/, *nu /*1*/, *lox /*2*/, *lplus /*3*/, *lo /*4*/, *lq /*5*/));
    	RooFormulaVar* I3 = new RooFormulaVar("I_3", "@0*@1*@2/4/@3/(@4-@3)+@0*@5*@6/@3/(@5-@3)+@0*@7/@3", RooArgList(*nu /*0*/, *lp /*1*/, *lopc /*2*/, *lo /*3*/, *lp /*4*/, *lplus /*5*/, *lox /*6*/, *lpo /*7*/));

    	indirectParameters->add(*I0);
    	indirectParameters->add(*I1);
    	indirectParameters->add(*I2);
    	indirectParameters->add(*I3);
    }
    return indirectParameters;
}
