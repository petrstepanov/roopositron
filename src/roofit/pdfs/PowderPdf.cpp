/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PowderPdf.cpp
 * Author: petrstepanov
 * 
 * Created on November 9, 2016, 8:48 PM
 */

#include "PowderPdf.h"

PowderPdf::PowderPdf(const char *name, const char *title,
    RooAbsReal& _t,
    RooAbsReal& _L,
    RooAbsReal& _kv,
    RooAbsReal& _Pps,
    RooAbsReal& _lb,
    RooAbsReal& _lv,
    RooAbsReal& _lPs,
    RooAbsReal& _Mr,
    RooAbsReal& _mu,
    RooAbsReal& _l2g,
    RooAbsReal& _l3g,	
    RooAbsReal& _V0,
    RooAbsReal& _Vth,
    Double_t _chW
    ) :
    RooAbsPdf(name, title),
    t("t", "t", this, _t),
    L("L", "L", this, _L),
    kv("kv", "kv", this, _kv),
    Pps("Pps", "Pps", this, _Pps),
    lb("lb", "lb", this, _lb),
    lv("lv", "lv", this, _lv),
    lPs("lPs", "lPs", this, _lPs),
    Mr("Mr", "Mr", this, _Mr),
    mu("mu", "mu", this, _mu),
    l2g("l2g", "l2g", this, _l2g),
    l3g("l3g", "l3g", this, _l3g),
    V0("V0", "V0", this, _V0),
    Vth("Vth", "Vth", this, _Vth),
    chW(_chW) {
}

PowderPdf::PowderPdf(const PowderPdf& other, const char* name) :
RooAbsPdf(other, name),
t("t", this, other.t),
L("L", this, other.L),
kv("kv", this, other.kv),
Pps("Pps", this, other.Pps),
lb("lb", this, other.lb),
lv("lv", this, other.lv),
lPs("lPs", this, other.lPs),
Mr("Mr", this, other.Mr),
mu("mu", this, other.mu),
l2g("l2g", this, other.l2g),
l3g("l3g", this, other.l3g),	
V0("V0", this, other.V0),
Vth("Vth", this, other.Vth),
chW(other.chW){
}

Double_t PowderPdf::evaluate() const {   
    if (t < 0) return 0.;  
    Double_t E = TMath::E();
    Double_t Pi = TMath::Pi();
    Double_t val = 
chW*Power(E,chW*(-kv-lb)*t)*lb*(1-Pps)+(chW*(-Power(E,chW*(-kv-lb)*t)+Power(E,-(chW*lv*t)))*kv*lv*(1-Pps))/
(kv+lb-lv)+(chW*(0.61/Power(E,(chW*Power(Pi,2)*t)/Power(L,2))+
0.39/Power(E,10.273007977238624*Sqrt((chW*t)/Power(L,2))))*lPs*Pps)/Power(E,chW*lPs*t)+
(3*chW*Pps*(Power(E,(-4*chW*Power(Pi,2)*t)/Power(L,2))+Power(E,-(chW*Power(Pi,2)*t)/Power(L,2))+
(0.2663171578205889*Sqrt(Power(L,2)/(chW*t)))/
(1+(19*chW*Power(Pi,2)*t)/Power(L,2)+(112*Power(chW,2)*Power(Pi,4)*Power(t,2))/Power(L,4))))/
(2.*Power(E,chW*lPs*t)*Power(L,2))+(9*Power(E,
(-6.64*chW*t)/L-chW*l3g*t+(-((chW*mu*t*Vth)/L)-
Mr*mu*Log((1-(V0-Vth)/(Power(E,(2*chW*t*Vth)/(L*Mr))*(V0+Vth)))/(1-(V0-Vth)/(V0+Vth))))/4.)*Pps*
((6.64*chW)/L+chW*l3g+(chW*mu*Vth*(1+(V0-Vth)/(Power(E,(2*chW*t*Vth)/(L*Mr))*(V0+Vth))))/
(4.*L*(1-(V0-Vth)/(Power(E,(2*chW*t*Vth)/(L*Mr))*(V0+Vth)))))*(-1+L*Sqrt(lPs)*Coth(L*Sqrt(lPs))))/
(4.*Power(L,2)*lPs);
    
//    std::cout << "powder(" << t << ") = " << val << std::endl;
    return val;
}

Double_t PowderPdf::Power(Double_t a, Double_t b) const{
    return TMath::Power(a, b);
}

Double_t PowderPdf::Sqrt(Double_t a) const{
    return TMath::Sqrt(a);
}

Double_t PowderPdf::ArcTan(Double_t a) const{
    return TMath::ATan(a);
}

Double_t PowderPdf::Coth(Double_t a) const{
    return 1/TMath::TanH(a);
}

Double_t PowderPdf::Log(Double_t a) const{
    return TMath::Log(a);
}

// Indefinite integral
//Double_t PowderPdf::indefiniteIntegral(Double_t y) const {   
//    Double_t ret = ((-3600*lj)/(Power(E,lg*y)*(k+lb-lg+lj))+(3600*lg*lj)/(Power(E,(k+lb+lj)*y)*(k+lb+lj)*(k+lb-lg+lj))+(3600*lg*lj)/(Power(E,(k+lb+4*lj)*y)*(k+lb+4*lj)*(k+lb-lg+4*lj))+(3600*lg*lj)/(Power(E,(k+lb+9*lj)*y)*(k+lb+9*lj)*(k+lb-lg+9*lj))+(3600*lg*lj)/(Power(E,(k+lb+16*lj)*y)*(k+lb+16*lj)*(k+lb-lg+16*lj))+(3600*lg*lj)/(Power(E,(k+lb+25*lj)*y)*(k+lb+25*lj)*(k+lb-lg+25*lj))+(lb*((-3600*Power(E,24*lj*y))/(k+lb+lj)-(900*Power(E,21*lj*y))/(k+lb+4*lj)-(400*Power(E,16*lj*y))/(k+lb+9*lj)-(225*Power(E,9*lj*y))/(k+lb+16*lj)-144/(k+lb+25*lj)))/Power(E,(k+lb+25*lj)*y)-(7200*lj*(2*Power(k,3)+2*Power(lb,3)-2*Power(lg,3)+81*Power(lg,2)*lj-969*lg*Power(lj,2)+3338*Power(lj,3)+Power(lb,2)*(-6*lg+81*lj)+Power(k,2)*(6*lb-6*lg+81*lj)+3*lb*(2*Power(lg,2)-54*lg*lj+323*Power(lj,2))+3*k*(2*Power(lb,2)-4*lb*lg+2*Power(lg,2)+54*lb*lj-54*lg*lj+323*Power(lj,2))))/(Power(E,lg*y)*(k+lb-lg+4*lj)*(k+lb-lg+9*lj)*(k+lb-lg+16*lj)*(k+lb-lg+25*lj))+(3600*k*lv)/(Power(E,(k+lb+lj)*y)*(k+lb+lj)*(k+lb+lj-lv))+(900*k*lv)/(Power(E,(k+lb+4*lj)*y)*(k+lb+4*lj)*(k+lb+4*lj-lv))+(400*k*lv)/(Power(E,(k+lb+9*lj)*y)*(k+lb+9*lj)*(k+lb+9*lj-lv))+(225*k*lv)/(Power(E,(k+lb+16*lj)*y)*(k+lb+16*lj)*(k+lb+16*lj-lv))+(144*k*lv)/(Power(E,(k+lb+25*lj)*y)*(k+lb+25*lj)*(k+lb+25*lj-lv))-(k*(5269*Power(k,4)+5269*Power(lb,4)+56005444*Power(lj,4)+Power(lb,3)*(271795*lj-21076*lv)+Power(k,3)*(21076*lb+271795*lj-21076*lv)-29233105*Power(lj,3)*lv+4598187*Power(lj,2)*Power(lv,2)-271795*lj*Power(lv,3)+5269*Power(lv,4)+Power(lb,2)*(4598187*Power(lj,2)-815385*lj*lv+31614*Power(lv,2))+Power(k,2)*(31614*Power(lb,2)+815385*lb*lj+4598187*Power(lj,2)-63228*lb*lv-815385*lj*lv+31614*Power(lv,2))+lb*(29233105*Power(lj,3)-9196374*Power(lj,2)*lv+815385*lj*Power(lv,2)-21076*Power(lv,3))+k*(21076*Power(lb,3)+29233105*Power(lj,3)+Power(lb,2)*(815385*lj-63228*lv)-9196374*Power(lj,2)*lv+815385*lj*Power(lv,2)-21076*Power(lv,3)+6*lb*(1532729*Power(lj,2)-271795*lj*lv+10538*Power(lv,2)))))/(Power(E,lv*y)*(k+lb+lj-lv)*(k+lb+4*lj-lv)*(k+lb+9*lj-lv)*(k+lb+16*lj-lv)*(k+lb+25*lj-lv)))/(600.*Power(pi,2));
//    return ret;
//}

// Get analytical integral
//Int_t PowderPdf::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* rangeName) const {
//   if (matchArgs(allVars,analVars,t)) return 1;
//   return 0;
//}

// Analytical integral
//Double_t PowderPdf::analyticalIntegral(Int_t code, const char* rangeName) const {
//    assert(code == 1);
//    
//    Double_t ret = 0;
//    if (code==1){
//        Double_t x1 = t.min(rangeName);
//        Double_t x2 = t.max(rangeName);
//        if (x2 <= 0) return 0;
//        x1 = TMath::Max(0.,x1);
//        
//        ret = indefiniteIntegral(x2)-indefiniteIntegral(x1);
//    }
//    else {
//        std::cout << "Error in PowderPdf::analyticalIntegral" << std::endl;
//    }
//    return ret;
//}