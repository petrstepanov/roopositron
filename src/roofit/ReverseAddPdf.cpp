/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ReverseAddPdf.cpp
 * Author: petrstepanov
 * 
 * Created on December 11, 2018, 4:52 PM
 */

#include <string>

#include "ReverseAddPdf.h"
#include "RooAbsArg.h"
#include "RooArgList.h"
#include "RooRealVar.h"
#include "RooFormulaVar.h"
#include "RooAddPdf.h"
#include "../util/StringUtils.h"

ReverseAddPdf::ReverseAddPdf() {
}

ReverseAddPdf::~ReverseAddPdf() {
}

// Function adds PDFs in following way: I4*F4 + I3*F3+ I2*F2 + (1-I4-I3-I2)*F1
//RooAbsPdf* ReverseAddPdf::addPdf(RooArgList* pdfList, char* prefix) {
//    // Construct inverse list;
//    RooArgList* pdfInverseList = new RooArgList();
//    TIterator* it = pdfList->createIterator(kIterBackward);
//    TObject* temp;
//    while((temp = it->Next())){
//	RooAbsPdf* pdf = dynamic_cast<RooAbsPdf*>(temp);
//	if(pdf){
//	    pdfInverseList->add(*pdf);
//	}
//    }
//    
//    // If one component given just return it
//    unsigned numberOfComponents = pdfList->getSize();
//    if (numberOfComponents == 1){
//	RooAbsArg* arg = pdfList->at(0);
//	RooAbsPdf* pdf = dynamic_cast<RooAbsPdf*>arg;
//	return pdf;
//    }
//    
//    // Construct list of original model coefficients
//    // I_i = [I4, I3, I2]
//    RooArgList* I_i = new RooArgList();
//    for (unsigned i=numberOfComponents; i > 1; i--){
//	RooRealVar* I = new RooRealVar(TString::Format("I%s%d", prefix, i), 
//				       TString::Format("Intensity I%s%d", prefix, i), 5.0, 0.0, 100.0);
//	RooFormulaVar* INorm = new RooFormulaVar(TString::Format("INorm%s%d", prefix, i), "@0/100", *I);
//	I_i->add(*INorm);
//    }
//    
//    // Construct C1, C2, C3
//    RooArgList* C_i = new RooArgList();    
//    std::string formula = "@0";
//    for (unsigned i=1; i < numberOfComponents; i++){  
//	RooRealVar* Ii = dynamic_cast<RooRealVar*>(I_i->at(i-1));
//	RooFormulaVar* Ci;
//        // C1 = I4
//	TString cName = TString::Format("C%s%d", prefix, i);
//	TString cTitle = TString::Format("Intensity C%s%d", prefix, i);
//	if (i==1){
//	    Ci = new RooFormulaVar(cName, cTitle, "@0", RooArgList(*Ii));
//	} else {
//	    // e.g. C1, C2, I3 
//	    RooArgList* dependentsList = new RooArgList();
//	    dependentsList->add(*Ii);
//	    RooArgList* cCoeffList = C_i->clone(TString::Format("cCoeffList%d"));
//	    dependentsList->add(*cCoeffList);
//	    formula+="/(1-@";
//	    formula+=std::to_string(i-1);
//	    
//	    Ci = new RooFormulaVar(cName, cTitle, formula.c_str(), dependentsList);	    
//	}
//	C_i->add(*Ci);
//    }
//    
//    return RooAddPdf(TString::Format("reverseModelI%s", prefix), TString::Format("Reverse Coefficients Model %s", prefix), *pdfInverseList, *C_i, kTRUE);	
//}

//RooAbsPdf* ReverseAddPdf::addPdfRecursive(RooArgList* pdfList, const char* prefix) {
//    // Construct inverse list;
//    RooArgList* pdfInverseList = new RooArgList();
//    TIterator* it = pdfList->createIterator(kIterBackward);
//    TObject* temp;
//    while((temp = it->Next())){
//	RooAbsPdf* pdf = dynamic_cast<RooAbsPdf*>(temp);
//	if(pdf){
//	    pdfInverseList->add(*pdf);
//	}
//    }
//    
//    // https://app.box.com/s/jlqd4mpi26x9pnl1a084cyy9knyk0jy0
//    std::cout << "---------------------------------" << std::endl;
//    pdfList->Print();
//    std::cout << "---------------------------------" << std::endl;
//    pdfInverseList->Print();  
//
//    // If one component given just return it
//    unsigned numberOfComponents = pdfList->getSize();
//    if (numberOfComponents == 1){
//	RooAbsArg* arg = pdfList->at(0);
//	RooAbsPdf* pdf = dynamic_cast<RooAbsPdf*>(arg);
//	return pdf;
//    }

RooAbsPdf* ReverseAddPdf::add(RooArgList* pdfList, const char* pdfName) {
	// Construct inverse list;
	RooArgList* pdfInverseList = new RooArgList();
	TIterator* it = pdfList->createIterator(kIterBackward);
	TObject* temp;
	while ((temp = it->Next())) {
		RooAbsPdf* pdf = dynamic_cast<RooAbsPdf*>(temp);
		if (pdf) {
			pdfInverseList->add(*pdf);
		}
	}

	// If one component given just return it
	unsigned numberOfComponents = pdfList->getSize();
	if (numberOfComponents == 1) {
		RooAbsArg* arg = pdfList->at(0);
		RooAbsPdf* pdf = dynamic_cast<RooAbsPdf*>(arg);
		return pdf;
	}

	RooArgList* I_i = new RooArgList();
	for (unsigned i = 0; i < numberOfComponents - 1; i++) {
		// Construct list of original model coefficients
		// I_i = [I4, I3, I2]
		// 1st "Source" component intensity
		std::string IName = StringUtils::suffix("Int", numberOfComponents - i, pdfName);

		std::string ITitle = pdfName;
		if (ITitle.empty()) {
			ITitle = "component intensity";
		} else {
			ITitle += " component intensity";
		}
		ITitle = StringUtils::ordinal(ITitle.c_str(), numberOfComponents - i);

		RooRealVar* I = new RooRealVar(IName.c_str(), ITitle.c_str(), 5.0, 0.0, 100.0, "%");

		std::string INormName = StringUtils::suffix("INorm", numberOfComponents - i, pdfName);
		RooFormulaVar* INorm = new RooFormulaVar(INormName.c_str(), ITitle.c_str(), "@0/100", *I);
		I_i->add(*INorm);
	}

	return new RooAddPdf(TString::Format("reverseModel%s", pdfName), TString::Format("Reverse Coefficients Model %s", pdfName), *pdfInverseList, *I_i);

}
