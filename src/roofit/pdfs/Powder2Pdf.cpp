/*
 * Powder2.cpp
 *
 *  Created on: Feb 7, 2019
 *      Author: student
 */

#include "Powder2Pdf.h"

ClassImp(Powder2Pdf);

Powder2Pdf::Powder2Pdf(const char *name, const char *title,
    RooAbsReal& _t,
    RooAbsReal& _L,
    RooAbsReal& _kd1,
    RooAbsReal& _kd2,
    RooAbsReal& _Pps,
    RooAbsReal& _lb,
    RooAbsReal& _ld1,
    RooAbsReal& _ld2,
    RooAbsReal& _lPs,
    RooAbsReal& _Mr,
    RooAbsReal& _mu,
    RooAbsReal& _l2g,
    RooAbsReal& _V0,
    RooAbsReal& _Vth,
    Double_t _chW
    ) :
    RooAbsPdf(name, title),
    t("t", "t", this, _t),
    L("L", "L", this, _L),
    kd1("kd1", "kd1", this, _kd1),
    kd2("kd2", "kd2", this, _kd2),
	Pps("Pps", "Pps", this, _Pps),
    lb("lb", "lb", this, _lb),
    ld1("ld1", "ld1", this, _ld1),
	ld2("ld2", "ld2", this, _ld2),
    lPs("lPs", "lPs", this, _lPs),
    Mr("Mr", "Mr", this, _Mr),
    mu("mu", "mu", this, _mu),
    l2g("l2g", "l2g", this, _l2g),
    V0("V0", "V0", this, _V0),
    Vth("Vth", "Vth", this, _Vth),
    chW(_chW) {
}

Powder2Pdf::Powder2Pdf(const Powder2Pdf& other, const char* name) :
RooAbsPdf(other, name),
t("t", this, other.t),
L("L", this, other.L),
kd1("kd1", this, other.kd1),
kd2("kd2", this, other.kd2),
Pps("Pps", this, other.Pps),
lb("lb", this, other.lb),
ld1("ld1", this, other.ld1),
ld2("ld2", this, other.ld2),
lPs("lPs", this, other.lPs),
Mr("Mr", this, other.Mr),
mu("mu", this, other.mu),
l2g("l2g", this, other.l2g),
V0("V0", this, other.V0),
Vth("Vth", this, other.Vth),
chW(other.chW){
}

Double_t Powder2Pdf::evaluate() const {
    if (t < 0) return 0.;
    Double_t E = TMath::E();
    Double_t Pi = TMath::Pi();

    Double_t DPs = 1E7;
    Double_t lJ=Pi*Pi*DPs/L/L;
    Double_t l = L;

    return
Power(E,chW*(-kd1 - kd2 - lb)*t)*lb*(1 - Pps) +
   ((-Power(E,chW*(-kd1 - kd2 - lb)*t) + Power(E,-(chW*ld1*t)))*kd1*ld1*(1 - Pps))/
	(kd1 + kd2 + lb - ld1) + ((-Power(E,chW*(-kd1 - kd2 - lb)*t) +
		Power(E,-(chW*ld2*t)))*kd2*ld2*(1 - Pps))/(kd1 + kd2 + lb - ld2) +
   (l2g*Pps)/(4.*Power(E,chW*l2g*t)) +
   (3*Power(E,(-((chW*mu*t*Vth)/l) -
		  Mr*mu*Log((1 - (V0 - Vth)/(Power(E,(2*chW*t*Vth)/(l*Mr))*(V0 + Vth)))/
			 (1 - (V0 - Vth)/(V0 + Vth))))/4.)*mu*Pps*Vth*
	  (1 + (V0 - Vth)/(Power(E,(2*chW*t*Vth)/(l*Mr))*(V0 + Vth))))/
	(16.*l*(1 - (V0 - Vth)/(Power(E,(2*chW*t*Vth)/(l*Mr))*(V0 + Vth))))
				;

}

Double_t Powder2Pdf::Power(Double_t a, Double_t b) const{
    return TMath::Power(a, b);
}

Double_t Powder2Pdf::Sqrt(Double_t a) const{
    return TMath::Sqrt(a);
}

Double_t Powder2Pdf::ArcTan(Double_t a) const{
    return TMath::ATan(a);
}

Double_t Powder2Pdf::Coth(Double_t a) const{
    return 1/TMath::TanH(a);
}

Double_t Powder2Pdf::Log(Double_t a) const{
    return TMath::Log(a);
}


Int_t Powder2Pdf::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* rangeName) const {
   if (matchArgs(allVars,analVars,t)) return 1;
   return 0;
}

Double_t Powder2Pdf::indefiniteIntegral(Double_t y) const{
    Double_t l = L;
	Double_t E = TMath::E();
	return
((4*kd1*(-1 + Pps))/(Power(E,chW*ld1*y)*(kd1 + kd2 + lb - ld1)) +
	 (4*kd2*(-1 + Pps))/(Power(E,chW*ld2*y)*(kd1 + kd2 + lb - ld2)) +
	 (4*(kd1*(lb - ld1) + (kd2 + lb - ld1)*(lb - ld2))*(-1 + Pps))/
	  (Power(E,chW*(kd1 + kd2 + lb)*y)*(kd1 + kd2 + lb - ld1)*(kd1 + kd2 + lb - ld2))\
	  - Pps/Power(E,chW*l2g*y) - (3*Power(2,(Mr*mu)/4.)*Pps)/
	  (Power(E,(chW*mu*Vth*y)/(4.*l))*
		Power(((V0 + Vth)*(1 + (-V0 + Vth)/
			   (Power(E,(2*chW*Vth*y)/(l*Mr))*(V0 + Vth))))/Vth,(Mr*mu)/4.)))/(4.*chW)
			;
}

Double_t Powder2Pdf::analyticalIntegral(Int_t code, const char* rangeName) const {
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
        std::cout << "Error in Powder2Pdf::analyticalIntegral" << std::endl;
    }
    return 0;
}
