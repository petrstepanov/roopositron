/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MyConvPdf.cpp
 * Author: petrstepanov
 * 
 * Created on November 9, 2016, 7:49 PM
 */

#include "MyConvPdf.h"
#include <unistd.h>

MyConvPdf::MyConvPdf(const char *name, const char *title,
	RooAbsReal& _t,
	RooAbsPdf* _f,
	RooAbsPdf* _g) :
	RooAbsPdf(name, title),
	t("t", "t", this, _t),
	f(_f),
	g(_g){
    // Set default precision
    np = 500;
    
    obsName = _t.GetName();
    
    // Get observable from G PDF with the same name as _t (time variable)
    RooRealVar* gObs = this->getPdfObservable(g, _t.GetName());
    std::cout << "Found observable for function G:" << std:: endl;
    gObs->Print();

    // Determine the integration limits for g
    std::cout << "Finding integration minimum" << std::endl;
    
    for (Double_t x = gObs->getMin(); x < gObs->getMax(); x+=0.1){
        gObs->setVal(x);
        if (std::abs(g->getValV()) > 1E-3){
            break;
        } 
//        std::cout << gObs->getVal() << " " << g->getValV() << std::endl;
    }
    gMin = gObs->getVal();

    for (Double_t x = gObs->getMax(); x > gObs->getMin(); x-=0.1){
        gObs->setVal(x);
        if (std::abs(g->getValV()) > 1E-3){
            break;
        } 
//        std::cout << gObs->getVal() << " " << g->getValV() << std::endl;
    }
    gMax = gObs->getVal();
    
    step = (gMax - gMin) / 500;
    std::cout << "Convolution PDF created. Range [" << gMin << ", " << gMax << "]" << std::endl;
}

MyConvPdf::MyConvPdf(const MyConvPdf& other, const char* name) :
RooAbsPdf(other, name),
t("t", this, other.t),
f(other.f),
g(other.f){
}

RooRealVar* MyConvPdf::getPdfObservable(RooAbsPdf* pdf, const char* name) const {
    RooArgSet* set = pdf->getVariables();
    TIterator* it = set->createIterator();
    TObject* tempObj=0;
    RooRealVar* r;
    while((tempObj=it->Next())){
        r = dynamic_cast<RooRealVar*>(tempObj);
        if(r){
            if (strcmp(r->GetName(), name) == 0) {
                return r;
            }            
        }
    }    
    return NULL;
}

void MyConvPdf::setPrecision(Int_t np) {
    this->np = np;
}


Double_t MyConvPdf::evaluate() const {
    // Range of convolution integrals
    Double_t tLow = t - gMin;
    Double_t tUpp = t + gMax;

    Double_t sum = 0;

    RooRealVar* fObs = this->getPdfObservable(f, obsName);
    fObs->Print();
    fObs->setVal(t);
    RooRealVar* gObs = this->getPdfObservable(g, obsName);
    // Convolution integral of Background and Gaussian by sum
    for (Double_t _t=tLow; _t<=tUpp; _t+=step) {
        gObs->setVal(_t);
        sum += f->getVal() * g->getVal() * step;
    }
    return sum;
}

