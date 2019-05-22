/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   LiquidPdf.cpp
 * Author: petrstepanov
 * 
 * Created on November 9, 2016, 8:48 PM
 */

#include "LiquidPdf.h"

ClassImp(LiquidPdf);

LiquidPdf::LiquidPdf(const char *name, const char *title,
	    RooAbsReal& _t,
		RooAbsReal& _Pqf,
		RooAbsReal& _lb,
		RooAbsReal& _lqf,
		RooAbsReal& _lpo,
		RooAbsReal& _lopc,
		RooAbsReal& _lox,
		RooAbsReal& _l2g,
		RooAbsReal& _chW
    ) :
    RooAbsPdf(name, title),
    t("t", "t", this, _t),
    Pqf("Pqf", "Pqf", this, _Pqf),
    lb("lb", "lb", this, _lb),
    lqf("lqf", "lqf", this, _lqf),
    lpo("lpo", "lpo", this, _lpo),
    lopc("lopc", "lopc", this, _lopc),
    lox("lox", "lox", this, _lox),
    l2g("l2g", "l2g", this, _l2g),
	chW("chW", "chW", this, _chW) {
}

LiquidPdf::LiquidPdf(const LiquidPdf& other, const char* name) :
RooAbsPdf(other, name),
t("t", this, other.t),
Pqf("Pqf", this, other.Pqf),
lb("lb", this, other.lb),
lqf("lqf", this, other.lqf),
lpo("lpo", this, other.lpo),
lopc("lopc", this, other.lopc),
lox("lox", this, other.lox),
l2g("l2g", this, other.l2g),
chW("chW", this, other.chW){
}

Double_t LiquidPdf::evaluate() const {
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

Double_t LiquidPdf::Power(Double_t a, Double_t b) const{
    return TMath::Power(a, b);
}

Double_t LiquidPdf::Sqrt(Double_t a) const{
    return TMath::Sqrt(a);
}

Double_t LiquidPdf::ArcTan(Double_t a) const{
    return TMath::ATan(a);
}

Double_t LiquidPdf::Coth(Double_t a) const{
    return 1/TMath::TanH(a);
}

Double_t LiquidPdf::Log(Double_t a) const{
    return TMath::Log(a);
}


Int_t LiquidPdf::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* rangeName) const {
   if (matchArgs(allVars,analVars,t)) return 1;
   return 0;
}

Double_t LiquidPdf::indefiniteIntegral(Double_t y) const{
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

Double_t LiquidPdf::analyticalIntegral(Int_t code, const char* rangeName) const {
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
        std::cout << "Error in LiquidPdf::analyticalIntegral" << std::endl;
    }
    return 0;
}
