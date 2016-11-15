/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TwoExpPdf.cpp
 * Author: petrstepanov
 * 
 * Created on November 9, 2016, 7:52 PM
 */

#include "TwoExpPdf.h"

TwoExpPdf::TwoExpPdf(const char *name, const char *title,
	RooAbsReal& _t,
	RooAbsReal& _tau1,
	RooAbsReal& _tau2,
	RooAbsReal& _i2) :
	RooAbsPdf(name, title),
	t("t", "t", this, _t),
	tau1("tau1", "tau1", this, _tau1),
	tau2("tau2", "tau2", this, _tau2),
	i2("i2", "i2", this, _i2){
}

TwoExpPdf::TwoExpPdf(const TwoExpPdf& other, const char* name) :
RooAbsPdf(other, name),
t("t", this, other.t),
tau1("tau1", this, other.tau1),
tau2("tau2", this, other.tau2),
i2("i2", this, other.i2){
}

Double_t TwoExpPdf::evaluate() const {
	if (t < 0) return 0.;
	return (1. - i2)*exp(-(t / tau1)) / tau1 + (i2)*exp(-(t / tau2)) / tau2;
}


