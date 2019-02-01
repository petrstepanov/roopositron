/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GrainPdf.cpp
 * Author: petrstepanov
 * 
 * Created on November 9, 2016, 8:48 PM
 */

#include "GrainPdf.h"

ClassImp(GrainPdf);

GrainPdf::GrainPdf(const char *name, const char *title,
    RooAbsReal& _t,
    RooAbsReal& _lambdaJ,
    RooAbsReal& _lambdaBulk,
    RooAbsReal& _lambdaGrain,
    RooAbsReal& _lambdaVac,
    RooAbsReal& _kappaVac
    ) :
    RooAbsPdf(name, title),
    t("t", "t", this, _t),
    lambdaJ("lambdaJ", "lambdaJ", this, _lambdaJ),
    lambdaBulk("lambdaBulk", "lambdaBulk", this, _lambdaBulk),
    lambdaGrain("lambdaGrain", "lambdaGrain", this, _lambdaGrain),
    lambdaVac("lambdaVac", "lambdaVac", this, _lambdaVac),
    kappaVac("kappaVac", "kappaVac", this, _kappaVac){
}

GrainPdf::GrainPdf(const GrainPdf& other, const char* name) :
RooAbsPdf(other, name),
t("t", this, other.t),
lambdaJ("lambdaJ", this, other.lambdaJ),
lambdaBulk("lambdaBulk", this, other.lambdaBulk),
lambdaGrain("lambdaGrain", this, other.lambdaGrain),
lambdaVac("lambdaVac", this, other.lambdaVac),
kappaVac("kappaVac", this, other.kappaVac){
}

// Evaluate without sums
Double_t GrainPdf::evaluate() const {   
    if (t < 0) return 0.;
    Double_t iterations = 5;
    Double_t piSquare = TMath::Pi()*TMath::Pi();
    
    // n_b
    Double_t n_b = 0;
    for (unsigned n = 1; n <= iterations; n++){
        n_b += exp(-lambdaJ*n*n*t) / (n*n);
    }
    n_b = 6 * exp(-(lambdaBulk + kappaVac)*t) / piSquare * n_b;

    // n_v
    Double_t n_v = 0;
    for (unsigned n = 1; n <= iterations; n++){
        n_v += (exp(-lambdaVac*t) - exp(-(lambdaBulk + kappaVac + lambdaJ*n*n)*t)) / (n*n*(lambdaBulk + kappaVac + lambdaJ*n*n - lambdaVac));
    }
    n_v = 6 * kappaVac / piSquare * n_v;

    // n_g
    Double_t n_g = 0;
    for (unsigned n = 1; n <= iterations; n++){
        n_g += (exp(-lambdaGrain*t) - exp(-(lambdaBulk + kappaVac + lambdaJ*n*n)*t)) / (lambdaBulk + kappaVac + lambdaJ*n*n - lambdaGrain);
    }
    n_g = 6 * lambdaJ / piSquare * n_g;

    return lambdaBulk*n_b + lambdaVac*n_v + lambdaGrain*n_g;
}

// Original evaluate
//Double_t GrainPdf::evaluate() const {   
//    if (t < 0) return 0.;
//    Double_t iterations = 5;
//    Double_t piSquare = TMath::Pi()*TMath::Pi();
//    
//    // n_b
//    Double_t n_b = 0;
//    for (unsigned n = 1; n <= iterations; n++){
//        n_b += exp(-lambdaJ*n*n*t) / (n*n);
//    }
//    n_b = 6 * exp(-(lambdaBulk + kappaVac)*t) / piSquare * n_b;
//
//    // n_v
//    Double_t n_v = 0;
//    for (unsigned n = 1; n <= iterations; n++){
//        n_v += (exp(-lambdaVac*t) - exp(-(lambdaBulk + kappaVac + lambdaJ*n*n)*t)) / (n*n*(lambdaBulk + kappaVac + lambdaJ*n*n - lambdaVac));
//    }
//    n_v = 6 * kappaVac / piSquare * n_v;
//
//    // n_g
//    Double_t n_g = 0;
//    for (unsigned n = 1; n <= iterations; n++){
//        n_g += (exp(-lambdaGrain*t) - exp(-(lambdaBulk + kappaVac + lambdaJ*n*n)*t)) / (lambdaBulk + kappaVac + lambdaJ*n*n - lambdaGrain);
//    }
//    n_g = 6 * lambdaJ / piSquare * n_g;
//
//    return lambdaBulk*n_b + lambdaVac*n_v + lambdaGrain*n_g;
//}


// With Sums 
//Double_t GrainPdf::sum(Double_t a) const {
//    Double_t piSquare = TMath::Pi()*TMath::Pi();
//    return 6*exp(-a)/piSquare + (1 - 6*exp(-3*sqrt(a))/piSquare);
//}
//
//Double_t GrainPdf::evaluate() const {   
//    if (t < 0) return 0.;
//    Double_t iterations = 5;
//    Double_t piSquare = TMath::Pi()*TMath::Pi();
//    
//    // n_b
//    Double_t n_b = 0;
//    n_b = exp(-(lambdaBulk + kappaVac)*t)*sum(lambdaJ*t);
//
//    // n_v
//    Double_t n_v = 0;
//    for (unsigned n = 1; n <= iterations; n++){
//        n_v += (exp(-lambdaVac*t) - exp(-(lambdaBulk + kappaVac + lambdaJ*n*n)*t)) / (n*n*(lambdaBulk + kappaVac + lambdaJ*n*n - lambdaVac));
//    }
//    n_v = 6 * kappaVac / piSquare * n_v;
//
//    // n_g
//    Double_t n_g = 0;
//    for (unsigned n = 1; n <= iterations; n++){
//        n_g += (exp((-1.)*lambdaGrain*t) - exp((-1.)*(lambdaBulk + kappaVac + lambdaJ*n*n)*t)) / (lambdaBulk + kappaVac + lambdaJ*n*n - lambdaGrain);
//    }
//    n_g = 6 * lambdaJ / piSquare * n_g;
//
//    return lambdaBulk*n_b + lambdaVac*n_v + lambdaGrain*n_g;
//}

Double_t GrainPdf::Power(Double_t a, Double_t b) const{
    return TMath::Power(a, b);
}

// Indefinite integral
Double_t GrainPdf::indefiniteIntegral(Double_t y) const {
    Double_t lj = lambdaJ;
    Double_t lb = lambdaBulk;
    Double_t lg = lambdaGrain;
    Double_t lv = lambdaVac;
    Double_t k = kappaVac;
    Double_t pi = TMath::Pi();
    Double_t E = TMath::E();
    
    Double_t ret = ((-3600*lj)/(Power(E,lg*y)*(k+lb-lg+lj))+(3600*lg*lj)/(Power(E,(k+lb+lj)*y)*(k+lb+lj)*(k+lb-lg+lj))+(3600*lg*lj)/(Power(E,(k+lb+4*lj)*y)*(k+lb+4*lj)*(k+lb-lg+4*lj))+(3600*lg*lj)/(Power(E,(k+lb+9*lj)*y)*(k+lb+9*lj)*(k+lb-lg+9*lj))+(3600*lg*lj)/(Power(E,(k+lb+16*lj)*y)*(k+lb+16*lj)*(k+lb-lg+16*lj))+(3600*lg*lj)/(Power(E,(k+lb+25*lj)*y)*(k+lb+25*lj)*(k+lb-lg+25*lj))+(lb*((-3600*Power(E,24*lj*y))/(k+lb+lj)-(900*Power(E,21*lj*y))/(k+lb+4*lj)-(400*Power(E,16*lj*y))/(k+lb+9*lj)-(225*Power(E,9*lj*y))/(k+lb+16*lj)-144/(k+lb+25*lj)))/Power(E,(k+lb+25*lj)*y)-(7200*lj*(2*Power(k,3)+2*Power(lb,3)-2*Power(lg,3)+81*Power(lg,2)*lj-969*lg*Power(lj,2)+3338*Power(lj,3)+Power(lb,2)*(-6*lg+81*lj)+Power(k,2)*(6*lb-6*lg+81*lj)+3*lb*(2*Power(lg,2)-54*lg*lj+323*Power(lj,2))+3*k*(2*Power(lb,2)-4*lb*lg+2*Power(lg,2)+54*lb*lj-54*lg*lj+323*Power(lj,2))))/(Power(E,lg*y)*(k+lb-lg+4*lj)*(k+lb-lg+9*lj)*(k+lb-lg+16*lj)*(k+lb-lg+25*lj))+(3600*k*lv)/(Power(E,(k+lb+lj)*y)*(k+lb+lj)*(k+lb+lj-lv))+(900*k*lv)/(Power(E,(k+lb+4*lj)*y)*(k+lb+4*lj)*(k+lb+4*lj-lv))+(400*k*lv)/(Power(E,(k+lb+9*lj)*y)*(k+lb+9*lj)*(k+lb+9*lj-lv))+(225*k*lv)/(Power(E,(k+lb+16*lj)*y)*(k+lb+16*lj)*(k+lb+16*lj-lv))+(144*k*lv)/(Power(E,(k+lb+25*lj)*y)*(k+lb+25*lj)*(k+lb+25*lj-lv))-(k*(5269*Power(k,4)+5269*Power(lb,4)+56005444*Power(lj,4)+Power(lb,3)*(271795*lj-21076*lv)+Power(k,3)*(21076*lb+271795*lj-21076*lv)-29233105*Power(lj,3)*lv+4598187*Power(lj,2)*Power(lv,2)-271795*lj*Power(lv,3)+5269*Power(lv,4)+Power(lb,2)*(4598187*Power(lj,2)-815385*lj*lv+31614*Power(lv,2))+Power(k,2)*(31614*Power(lb,2)+815385*lb*lj+4598187*Power(lj,2)-63228*lb*lv-815385*lj*lv+31614*Power(lv,2))+lb*(29233105*Power(lj,3)-9196374*Power(lj,2)*lv+815385*lj*Power(lv,2)-21076*Power(lv,3))+k*(21076*Power(lb,3)+29233105*Power(lj,3)+Power(lb,2)*(815385*lj-63228*lv)-9196374*Power(lj,2)*lv+815385*lj*Power(lv,2)-21076*Power(lv,3)+6*lb*(1532729*Power(lj,2)-271795*lj*lv+10538*Power(lv,2)))))/(Power(E,lv*y)*(k+lb+lj-lv)*(k+lb+4*lj-lv)*(k+lb+9*lj-lv)*(k+lb+16*lj-lv)*(k+lb+25*lj-lv)))/(600.*Power(pi,2));
    return ret;
}

// Get analytical integral
Int_t GrainPdf::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* rangeName) const {
   if (matchArgs(allVars,analVars,t)) return 1;
   return 0;
}

// Analytical integral
Double_t GrainPdf::analyticalIntegral(Int_t code, const char* rangeName) const {
    assert(code == 1);
    
    Double_t ret = 0;
    if (code==1){
        Double_t x1 = t.min(rangeName);
        Double_t x2 = t.max(rangeName);
        if (x2 <= 0) return 0;
        x1 = TMath::Max(0.,x1);
        
        ret = indefiniteIntegral(x2)-indefiniteIntegral(x1);
    }
    else {
        std::cout << "Error in GrainPdf::analyticalIntegral" << std::endl;
    }
    return ret;
}
