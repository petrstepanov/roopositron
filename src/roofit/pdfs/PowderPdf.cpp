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

ClassImp(PowderPdf);

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
//    RooAbsReal& _l3g,
    RooAbsReal& _V0,
    RooAbsReal& _Vth,
	RooAbsReal& _chW
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
//    l3g("l3g", "l3g", this, _l3g),
    V0("V0", "V0", this, _V0),
    Vth("Vth", "Vth", this, _Vth),
	chW("chW", "chW", this, _chW) {
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
//l3g("l3g", this, other.l3g),
V0("V0", this, other.V0),
Vth("Vth", this, other.Vth),
chW("chW", this, other.chW){
}

Double_t PowderPdf::evaluate() const {   
    if (t < 0) return 0.;  
    Double_t E = TMath::E();
    Double_t Pi = TMath::Pi();
    
    Double_t DPs = 1E7;
    Double_t lJ=Pi*Pi*DPs/L/L;
    Double_t l = L;
    
    return
Power(E,chW*(-kv - lb)*t)*lb*(1 - Pps) +
   ((-Power(E,chW*(-kv - lb)*t) + Power(E,-(chW*lv*t)))*kv*lv*(1 - Pps))/
	(kv + lb - lv) + (l2g*Pps)/(4.*Power(E,chW*l2g*t)) +
   (3*Power(E,(-((chW*mu*t*Vth)/l) -
		  Mr*mu*Log((1 - (V0 - Vth)/(Power(E,(2*chW*t*Vth)/(l*Mr))*(V0 + Vth)))/
			 (1 - (V0 - Vth)/(V0 + Vth))))/4.)*mu*Pps*Vth*
	  (1 + (V0 - Vth)/(Power(E,(2*chW*t*Vth)/(l*Mr))*(V0 + Vth))))/
	(16.*l*(1 - (V0 - Vth)/(Power(E,(2*chW*t*Vth)/(l*Mr))*(V0 + Vth))))
				;
    
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


Int_t PowderPdf::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* rangeName) const {
   if (matchArgs(allVars,analVars,t)) return 1;
   return 0;
}

Double_t PowderPdf::indefiniteIntegral(Double_t y) const{
    Double_t l = L;
	Double_t E = TMath::E();
	return
((4*kv*(-1 + Pps))/(Power(E,chW*lv*y)*(kv + lb - lv)) +
	 (4*(lb - lv)*(-1 + Pps))/(Power(E,chW*(kv + lb)*y)*(kv + lb - lv)) -
	 Pps/Power(E,chW*l2g*y) - (3*Power(2,(Mr*mu)/4.)*Pps)/
	  (Power(E,(chW*mu*Vth*y)/(4.*l))*
		Power(((V0 + Vth)*(1 + (-V0 + Vth)/
			   (Power(E,(2*chW*Vth*y)/(l*Mr))*(V0 + Vth))))/Vth,(Mr*mu)/4.)))/(4.*chW)
			;
}

Double_t PowderPdf::analyticalIntegral(Int_t code, const char* rangeName) const {
    assert(code == 1);

    if (code==1){
        // Range always called [AXIS_MIN-100, AXIS_MAX+100] : (-100; 2100) if 2000 bins
        Double_t t1 = t.min(rangeName);
        Double_t t2 = t.max(rangeName);

//        std::cout << "range(" << t1 << ", " << t2 << ")" << std::endl;

        if (t2 <= 0) return 0;
        t1 = TMath::Max(0.,t1);
        Double_t ret = indefiniteIntegral(t2)-indefiniteIntegral(t1);
        return ret;
    }
    else {
        std::cout << "Error in PowderPdf::analyticalIntegral" << std::endl;
    }
    return 0;
}
