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

#include "ParaProvider.h"
#include "RooFormulaVar.h"
#include "RooConstVar.h"
#include "../pdfs/ParaPdf.h"
#include "../../model/Constants.h"

ParaProvider::ParaProvider(RooRealVar* observable) :
		AbstractProvider(observable) {
}

ParaProvider::~ParaProvider() {
}

RooAbsPdf* ParaProvider::initPdf(int i) {
	RooConstVar* channelWidth = Constants::getInstance()->getRooChannelWidth();

//	Pps("Pps", this, other.Pps),
//	lplus("lplus", this, other.lplus),
//	lpo("lpo", this, other.lpo),
//	l2g("l2g", this, other.l2g),
//	lopc("lpoc", this, other.lopc),

	RooRealVar* pPs = new RooRealVar("pPs", "Probability of forming qf-Ps", 0.5, 0, 1, "");

	RooRealVar* lplus = new RooRealVar("#lambda_+", "Probability of observation a thermalized positron", 1, 1E-3, 1E3, "1/ns");

	RooRealVar* lpo = new RooRealVar("#lambda_po", "Pick-off annihilation speed", 1, 1E-3, 1E3, "1/ns");

	RooRealVar* l2g = new RooRealVar("#lambda_2#gamma", "Two-gamma annihilation rate", 8, "1/ns");
	l2g->setConstant(kTRUE);

	RooRealVar* lopc = new RooRealVar("#lambda_opc", "Ortho-para conversion speed", 1, 1E-3, 1E3, "1/ns");

	return new ParaPdf("para", "Paramagnetic model", *observable, *pPs, *lplus, *lpo, *l2g, *lopc, channelWidth->getValV());
}
