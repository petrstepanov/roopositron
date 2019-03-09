/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ParaPdf.cpp
 * Author: petrstepanov
 * 
 * Created on November 9, 2016, 8:48 PM
 */

#include "ParaPdf.h"

ClassImp(ParaPdf);

ParaPdf::ParaPdf(const char *name, const char *title,
    RooAbsReal& _t,
    RooAbsReal& _Pps,
    RooAbsReal& _lplus,
    RooAbsReal& _lpo,
    RooAbsReal& _l2g,
    RooAbsReal& _lopc,
    Double_t _chW
    ) :
    RooAbsPdf(name, title),
    t("t", "t", this, _t),
    Pps("Pps", "Pps", this, _Pps),
    lplus("lplus", "lplus", this, _lplus),
    lpo("lpo", "lpo", this, _lpo),
    l2g("l2g", "l2g", this, _l2g),
    lopc("lopc", "lopc", this, _lopc),
    chW(_chW) {
}

ParaPdf::ParaPdf(const ParaPdf& other, const char* name) :
RooAbsPdf(other, name),
t("t", this, other.t),
Pps("Pps", this, other.Pps),
lplus("lplus", this, other.lplus),
lpo("lpo", this, other.lpo),
l2g("l2g", this, other.l2g),
lopc("lpoc", this, other.lopc),
chW(other.chW){
}

Double_t ParaPdf::evaluate() const {
    if (t < 0) return 0.;  
    Double_t E = TMath::E();
//    Double_t Pi = TMath::Pi();
    
    return
    		(lplus*(1 - Pps))/Power(E,chW*lplus*t) +
    		   0.75*Power(E,chW*(-lopc - lpo)*t)*lpo*Pps +
    		   l2g*((Power(E,chW*(-l2g - lpo)*t)*Pps)/4. -
    		      (0.75*(-Power(E,chW*(-l2g - lpo)*t) + Power(E,chW*(-lopc - lpo)*t))*lopc*
    		         Pps)/(l2g - lopc))
				;
    
}

Double_t ParaPdf::Power(Double_t a, Double_t b) const{
    return TMath::Power(a, b);
}

Double_t ParaPdf::Sqrt(Double_t a) const{
    return TMath::Sqrt(a);
}

Double_t ParaPdf::ArcTan(Double_t a) const{
    return TMath::ATan(a);
}

Double_t ParaPdf::Coth(Double_t a) const{
    return 1/TMath::TanH(a);
}

Double_t ParaPdf::Log(Double_t a) const{
    return TMath::Log(a);
}


Int_t ParaPdf::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* rangeName) const {
   if (matchArgs(allVars,analVars,t)) return 1;
   return 0;
}

Double_t ParaPdf::indefiniteIntegral(Double_t y) const{
	Double_t E = TMath::E();
	return
			(-1 + Pps)/(chW*Power(E,chW*lplus*y)) -
			   (0.25*l2g*(1.*l2g + 2.*lopc)*Pps)/
			    (chW*Power(E,1.*chW*(l2g + lpo)*y)*(l2g - 1.*lopc)*(l2g + lpo)) +
			   (0.75*(1.*l2g*lopc*Pps - 1.*l2g*lpo*Pps + 1.*lopc*lpo*Pps))/
			    (chW*Power(E,1.*chW*(lopc + lpo)*y)*(l2g - 1.*lopc)*(lopc + lpo))
			;
}

Double_t ParaPdf::analyticalIntegral(Int_t code, const char* rangeName) const {
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
        std::cout << "Error in ParaPdf::analyticalIntegral" << std::endl;
    }
    return 0;
}
