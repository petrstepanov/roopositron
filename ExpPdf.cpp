/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ExpPdf.cpp
 * Author: petrstepanov
 * 
 * Created on November 9, 2016, 7:49 PM
 */

#include "ExpPdf.h"

ExpPdf::ExpPdf(const char *name, const char *title,
	RooAbsReal& _t,
	RooAbsReal& _tau) :
	RooAbsPdf(name, title),
	t("t", "t", this, _t),
	tau("tau", "tau", this, _tau){
}

ExpPdf::ExpPdf(const ExpPdf& other, const char* name) :
RooAbsPdf(other, name),
t("t", this, other.t),
tau("tau", this, other.tau){
}

Double_t ExpPdf::evaluate() const {
	if (t < 0) return 0.;
	return exp(-(t / tau)) / tau;
}

