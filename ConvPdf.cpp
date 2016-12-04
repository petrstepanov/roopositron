/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ConvPdf.cpp
 * Author: petrstepanov
 * 
 * Created on December 4, 2016, 12:14 AM
 */

#include "ConvPdf.h"

ConvPdf::ConvPdf(const char *name, const char *title,
	RooAbsReal& _t,
	RooAbsPdf* _f,
        RooAbsPdf* _g) :
	RooAbsPdf(name, title),
	t("t", "t", this, _t){
    f = _f;
    g = _g;
}

ConvPdf::ConvPdf(const ConvPdf& other, const char* name) :
RooAbsPdf(other, name),
t("t", this, other.t){
}

Double_t ConvPdf::evaluate() const {
    Double_t integral = 0;
    RooArgSet* set = f->getVariables();
    TIterator* it = set->createIterator();
    RooRealVar* tPtr = (RooRealVar*)it->Next();
    tPtr->Print();
    
    for (unsigned c=minChannel; c<=maxChannel; c++){
        tPtr->setVal(t);
        Double_t f_t = f->getValV();
        tPtr->setVal(t-c);
        Double_t g_t_tau = g->getValV();
        integral += f_t*g_t_tau;
    }
    return integral;
}

void ConvPdf::init (int _minChannel, int _maxChannel){
    minChannel = _minChannel;
    maxChannel = _maxChannel;
}