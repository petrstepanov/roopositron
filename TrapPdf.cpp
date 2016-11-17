/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TrapPdf.cpp
 * Author: petrstepanov
 * 
 * Created on November 17, 2016, 1:59 AM
 */

#include "TrapPdf.h"

TrapPdf::TrapPdf(const char *name, const char *title,
	RooAbsReal& _t,
	RooAbsReal& _tauBulk,
	RooAbsReal& _tauVac,
	RooAbsReal& _kappaVac) :
	RooAbsPdf(name, title),
	t("t", "t", this, _t),
	tauBulk("tauBulk", "tauBulk", this, _tauBulk),
	tauVac("tauVac", "tauVac", this, _tauVac),
	kappaVac("kappaVac", "kappaVac", this, _kappaVac){
}

TrapPdf::TrapPdf(const TrapPdf& other, const char* name) :
RooAbsPdf(other, name),
t("t", this, other.t),
tauBulk("tauBulk", this, other.tauBulk),
tauVac("tauVac", this, other.tauVac),
kappaVac("kappaVac", this, other.kappaVac){
}

Double_t TrapPdf::evaluate() const {
    if (t < 0) return 0.;

    Double_t lambdaBulk = 1/tauBulk;
    Double_t lambdaDefect = 1/tauVac;

    Double_t I_1 = (lambdaBulk-lambdaDefect)/(lambdaBulk+kappaVac-lambdaDefect);
    Double_t I_2 = kappaVac/(lambdaBulk+kappaVac-lambdaDefect);

    return I_1*(lambdaBulk+kappaVac)*exp((-1.)*(lambdaBulk+kappaVac)*t) + I_2*lambdaDefect*exp((-1.)*lambdaDefect*t);
}

