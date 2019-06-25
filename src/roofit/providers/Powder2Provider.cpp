/*
 * Powder2Provider.cpp
 *
 *  Created on: Feb 8, 2019
 *      Author: student
 */

#include "Powder2Provider.h"
#include "RooFormulaVar.h"
#include "RooConstVar.h"
#include "../pdfs/Powder2Pdf.h"
#include "../../model/Constants.h"

Powder2Provider::Powder2Provider(RooRealVar* _observable, RooRealVar* _channelWidth) : AbstractProvider(_observable, _channelWidth) {}

Powder2Provider::~Powder2Provider() {
}

RooAbsPdf* Powder2Provider::initPdf(int i) {
	RooRealVar* l2g = new RooRealVar("#lambda_2#gamma", "Two-gamma annihilation rate", 8, "1/ns");
	l2g->setConstant(kTRUE);

	RooRealVar* V0 = new RooRealVar("V0", "Ps speed when leaving the powder", 5.9E6, "A/ns");
	V0->setConstant(kTRUE);
	RooRealVar* Vth = new RooRealVar("Vth", "Thermalized Ps speed ", 8E5, "A/ns");
	Vth->setConstant(kTRUE);

	RooRealVar* L = new RooRealVar("l", "Distance between powders", 50, 1, 50000, "nm"); // 1nm - 50mkm
	RooFormulaVar* Langstrom = new RooFormulaVar("Langstrom", "10*@0", *L);

	RooRealVar* pPs = new RooRealVar("pPs", "Probability of forming qf-Ps", 0.5, 0, 1, "");

	RooRealVar* tauBulk = new RooRealVar("#tau_bulk", "e+ lifetime in bulk", 0.2, 0.1, 0.4, "ns");
	RooFormulaVar* lambdaBulk = new RooFormulaVar("lambdaBulk", "1/@0", *tauBulk);

	RooRealVar* tauDefect1 = new RooRealVar("#tau_def1", "e+ lifetime in defect 1", 0.3, 0.2, 0.4, "ns");
	RooFormulaVar* lambdaD1 = new RooFormulaVar("lambdaD1", "1/@0", *tauDefect1);

	RooRealVar* tauDefect2 = new RooRealVar("#tau_def2", "e+ lifetime in defect 2", 0.6, 0.3, 1.5, "ns");
	RooFormulaVar* lambdaD2 = new RooFormulaVar("lambdaD2", "1/@0", *tauDefect2);

	RooRealVar* kappaVac1 = new RooRealVar("#kappa1", "e+ trapping rate into defect 1", 1, 1E-3, 1E3, "1/ns");
	RooRealVar* kappaVac2 = new RooRealVar("#kappa2", "e+ trapping rate into defect 2", 1, 1E-3, 1E3, "1/ns");

	RooRealVar* Mratio = new RooRealVar("M/m", "Powder mass to positron mass ratio", 1E5, 1E2, 1E7, "");
	RooRealVar* mu = new RooRealVar("#mu", "Probability of o-p conversion near surface", 1E-5, 0, 1E-3, "");

	return new Powder2Pdf("powder2", "Powder model two-defect", *observable, *Langstrom, *kappaVac1, *kappaVac2, *pPs, *lambdaBulk, *lambdaD1, *lambdaD2, *lambdaBulk, *Mratio, *mu, *l2g, *V0, *Vth, *channelWidth);
}
