/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PowderProvider.cpp
 * Author: petrstepanov
 * 
 * Created on December 6, 2018, 1:03 PM
 */

#include "PowderProvider.h"
#include "RooFormulaVar.h"
#include "RooConstVar.h"
#include "../pdfs/PowderPdf.h"
#include "../../model/Constants.h"

PowderProvider::PowderProvider(RooRealVar* _observable, RooRealVar* _channelWidth) : AbstractProvider(_observable, _channelWidth) {}

PowderProvider::~PowderProvider() {
}

RooAbsPdf* PowderProvider::initPdf(int i) {
	// L("L", this, other.L),
	// kv("kv", this, other.kv),
	// Pps("Pps", this, other.Pps),
	// lb("lb", this, other.lb),
	// lv("lv", this, other.lv),
	// lPs("lPs", this, other.lPs),
	// Mr("Mr", this, other.Mr),
	// mu("mu", this, other.mu)

	RooRealVar* l2g = new RooRealVar("#lambda_2#gamma", "Two-gamma annihilation rate", 8, "1/ns");
	l2g->setConstant(kTRUE);
	// RooRealVar* l3g = new RooRealVar("#lambda_3#gamma", "Three-gamma annihilation rate", 0.007, "1/ns");
	// l3g->setConstant(kTRUE);

	RooRealVar* V0 = new RooRealVar("V0", "Ps speed when leaving the powder", 5.9E6, "A/ns");
	V0->setConstant(kTRUE);
	RooRealVar* Vth = new RooRealVar("Vth", "Thermalized Ps speed ", 8E5, "A/ns");
	Vth->setConstant(kTRUE);

	RooRealVar* L = new RooRealVar("l", "Distance between powders", 50, 1, 50000, "nm"); // 1nm - 50mkm
	RooFormulaVar* Langstrom = new RooFormulaVar("Langstrom", "10*@0", *L);

	RooRealVar* pPs = new RooRealVar("pPs", "Probability of forming qf-Ps", 0.5, 0, 1, "");

	RooRealVar* tauBulk = new RooRealVar("#tau_bulk", "e+ lifetime in bulk", 0.2, 0.1, 0.4, "ns");
	RooFormulaVar* lambdaBulk = new RooFormulaVar("lambdaBulk", "1/@0", *tauBulk);

	RooRealVar* tauVac = new RooRealVar("#tau_vac", "e+ lifetime in vacancy", 0.3, 0.1, 0.4, "ns");
	RooFormulaVar* lambdaVac = new RooFormulaVar("lambdaVac", "1/@0", *tauVac);

	RooRealVar* kappaVac = new RooRealVar("#kappa", "e+ trapping rate into vacancy", 1, 1E-3, 1E3, "1/ns");

	// Here we can assume: tauPs = tauBulk
	// RooRealVar* tauPs = new RooRealVar("tauPs", "Ps lifetime in vacancy", 1, 0.01, 100, "ns");
	// RooFormulaVar* lambdaPs = new RooFormulaVar("lambdaPs", "1/@0", *tauPs);

	RooRealVar* Mratio = new RooRealVar("M/m", "Powder mass to positron mass ratio", 1E5, 1E2, 1E7, "");
	RooRealVar* mu = new RooRealVar("#mu", "Probability of o-p conversion near surface", 1E-5, 0, 1E-3, "");

	return new PowderPdf("powder", "Powder model", *observable, *Langstrom, *kappaVac, *pPs, *lambdaBulk, *lambdaVac, *lambdaBulk/* *lambdaPs*/, *Mratio, *mu, *l2g, /**l3g,*/ *V0, *Vth, *channelWidth);
}
