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
		RooAbsReal& _lplus,
		RooAbsReal& _lpo,
		RooAbsReal& _lopc,
		RooAbsReal& _lox,
		RooAbsReal& _l2g,
		RooAbsReal& _chW
    ) :
	MyRooAbsPdf(name, title),
    t("t", "t", this, _t),
    Pqf("Pqf", "Pqf", this, _Pqf),
    lb("lb", "lb", this, _lb),
    lqf("lqf", "lqf", this, _lqf),
    lplus("lplus", "lplus", this, _lplus),
    lpo("lpo", "lpo", this, _lpo),
    lopc("lopc", "lopc", this, _lopc),
    lox("lox", "lox", this, _lox),
    l2g("l2g", "l2g", this, _l2g),
	chW("chW", "chW", this, _chW) {
}

LiquidPdf::LiquidPdf(const LiquidPdf& other, const char* name) :
MyRooAbsPdf(other, name),
t("t", this, other.t),
Pqf("Pqf", this, other.Pqf),
lb("lb", this, other.lb),
lqf("lqf", this, other.lqf),
lplus("lplus", this, other.lplus),
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
    
    return
    		Power(E,chW*(-lopc/4. - lox - lpo)*t)*(lopc/4. + lox + lpo)*((3*lb*lox*lplus*Pqf)/(4.*(-lopc/4. - lox + lplus - lpo)*(lopc/4. + lox + lpo)*(lb - lopc/4. - lox - lpo + lqf)) +
    		      (3*lb*lpo*Pqf)/(4.*(lopc/4. + lox + lpo)*(lb - lopc/4. - lox - lpo + lqf)) + (3*lb*lopc*(l2g + lpo)*Pqf)/(16.*(l2g - lopc/4. - lox)*(lopc/4. + lox + lpo)*(lb - lopc/4. - lox - lpo + lqf))) +
    		   (lplus*(1 - Pqf - (3*lb*lox*Pqf)/(4.*(-lopc/4. - lox + lplus - lpo)*(lb - lopc/4. - lox - lpo + lqf)) - (3*lb*lox*Pqf)/(4.*(lb - lplus + lqf)*(lb - lopc/4. - lox - lpo + lqf))))/Power(E,chW*lplus*t) +
    		   Power(E,chW*(-l2g - lpo)*t)*(l2g + lpo)*((lb*Pqf)/(4.*(-l2g + lb - lpo + lqf)) - (3*lb*lopc*Pqf)/(16.*(l2g - lopc/4. - lox)*(lb - lopc/4. - lox - lpo + lqf)) -
    		      (3*lb*lopc*Pqf)/(16.*(-l2g + lb - lpo + lqf)*(lb - lopc/4. - lox - lpo + lqf))) + Power(E,chW*(-lb - lqf)*t)*(lb + lqf)*
    		    ((lqf*Pqf)/(lb + lqf) - (lb*(l2g + lpo)*Pqf)/(4.*(lb + lqf)*(-l2g + lb - lpo + lqf)) - (3*lb*lpo*Pqf)/(4.*(lb + lqf)*(lb - lopc/4. - lox - lpo + lqf)) +
    		      (3*lb*lox*lplus*Pqf)/(4.*(lb + lqf)*(lb - lplus + lqf)*(lb - lopc/4. - lox - lpo + lqf)) + (3*lb*lopc*(l2g + lpo)*Pqf)/(16.*(lb + lqf)*(-l2g + lb - lpo + lqf)*(lb - lopc/4. - lox - lpo + lqf)))
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

Double_t LiquidPdf::indefiniteIntegral(Double_t x) const{
	Double_t E = TMath::E();
	return
			-(((lb*(lox*(-4 + Pqf) + lopc*(-1 + Pqf) - 4*(lplus - lpo)*(-1 + Pqf)) - (lopc + 4*(lox - lplus + lpo))*(lplus - lqf)*(-1 + Pqf))/(Power(E,chW*lplus*x)*(lopc + 4*(lox - lplus + lpo))*(-lb + lplus - lqf)) +
			       (12*lb*(4*lox*(lplus - lpo) + l2g*(lopc - 4*lplus + 4*lpo))*Pqf)/(Power(E,(chW*(lopc + 4*(lox + lpo))*x)/4.)*(-4*l2g + lopc + 4*lox)*(lopc + 4*(lox - lplus + lpo))*(-4*lb + lopc + 4*(lox + lpo - lqf))) +
			       (lb*(l2g - lopc - lox)*Pqf)/(Power(E,chW*(l2g + lpo)*x)*(4*l2g - lopc - 4*lox)*(-l2g + lb - lpo + lqf)) +
			       ((lb*(lox*(3*lplus + lpo - 4*lqf) + (lopc + 4*(lplus + lpo - 2*lqf))*(lpo - lqf)) - 4*Power(lb,2)*(lpo - lqf) + (lopc + 4*(lox + lpo - lqf))*(lpo - lqf)*(-lplus + lqf) +
			            l2g*(-Power(lb,2) + lb*(lopc + lox + lplus + 4*lpo - 5*lqf) + (lopc + 4*(lox + lpo - lqf))*(-lplus + lqf)))*Pqf)/(Power(E,chW*(lb + lqf)*x)*(4*lb - lopc - 4*(lox + lpo - lqf))*(lb - lplus + lqf)*(-l2g + lb - lpo + lqf)))
			      /chW)
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
