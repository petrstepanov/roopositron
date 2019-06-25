/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   RootUtils.cpp
 * Author: petrstepanov
 *
 * Created on December 4, 2018, 2:09 PM
 */

#include "RootHelper.h"
#include "TString.h"
#include "StringUtils.h"
#include "Debug.h"
#include <TUnixSystem.h>
#include <RooWorkspace.h>
#include <iostream>

//void RootHelper::deleteObject(const char* name){
//    TObject* obj = gROOT->FindObject(name);
//    deleteObject(obj);
//}
//
//void RootHelper::deleteObject(TObject* obj){
//    if (obj){
//        obj->Delete();
//    }
//}

Int_t RootHelper::getNumCpu() {
	SysInfo_t sysInfo;
	gSystem->GetSysInfo(&sysInfo);
	std::cout << "RootHelper::getNumCpu: sysInfo.fCpus = " << sysInfo.fCpus << std::endl;
	return (sysInfo.fCpus >= 0) ? sysInfo.fCpus : 1;
}

void RootHelper::printPdfCoefficientNames(RooAbsPdf* pdf, RooRealVar* observable) {
	std::cout << pdf->GetName() << ": ";

	RooArgSet* params = pdf->getParameters(*observable);
	TIterator* it = params->createIterator();
	TObject* temp;
	while ((temp = it->Next())) {
		TNamed* named = dynamic_cast<TNamed*>(temp);
		if (named) {
			std::cout << named->GetName() << " ";
		}
	}
	std::cout << std::endl;
}

RooRealVar* RootHelper::findParameterNameContains(RooAbsPdf* pdf, RooRealVar* observable, const char* substring) {
	RooArgSet* params = pdf->getParameters(*observable);
	TIterator* it = params->createIterator();
	TObject* temp;
	while ((temp = it->Next())) {
		if (RooRealVar* var = dynamic_cast<RooRealVar*>(temp)) {
			if (StringUtils::isSubstring(var->GetName(), substring)){
				return var;
			}
		}
	}
	return NULL;
}

RooAbsPdf* RootHelper::suffixPdfAndNodes(RooAbsPdf* pdf, RooRealVar* observable, const char* suffix){
	Debug("RootHelper::suffixPdfAndNodes");
	RooWorkspace* w = new RooWorkspace("w", "w");
	w->import(*pdf, RooFit::RenameAllVariablesExcept(suffix, observable->GetName()), RooFit::RenameAllNodes(suffix));
	#ifdef USEDEBUG
		w->Print();
	#endif
	TString suffixedName = TString::Format("%s_%s", pdf->GetName(), suffix);
	RooAbsPdf* suffixedPdf =  w->pdf(suffixedName.Data());

	// Rename indirect parameters too!

	return suffixedPdf;
}

void RootHelper::setRooRealVarValueLimits(RooRealVar* var, Double_t value, Double_t min, Double_t max){
	// Tip: roofit can't set min larger than current max and vice versa
	var->setMin(std::numeric_limits<double>::min());
	var->setMax(std::numeric_limits<double>::max());
	var->setMin(min);
	var->setMax(max);
	var->setVal(value);
}

RooRealVar* RootHelper::getParameterByNameCommonOrLocal(RooAbsPdf* pdf, const char* name){
	// for given parameter name, say #tau1 will return either exact match or suffixed:
	// #tau1 or #tau1_13 or #tau1_2

	std::string nameString(name);

	// TODO: check list is not empty otherwise pass observable
	RooArgSet* parameters = pdf->getParameters(RooArgSet()); // pdf->getParameters(*observable);
	TIterator* it = parameters->createIterator();
	while (TObject* temp = it->Next()) {
		if (RooRealVar* parameter = dynamic_cast<RooRealVar*>(temp)) {
			std::string parameterName = parameter->GetName();
			std::string parameterNameWithoutSuffix = StringUtils::getStringWithoutSuffix(parameterName);
			if (nameString == parameterNameWithoutSuffix){
				return parameter;
			}
		}
	}
	return NULL;
}

// Deprecated, use getParameterByNameCommonOrLocal()
RooRealVar* RootHelper::getParameterNameContains(RooArgSet* rooRealVarSet, const char* nameSubstring){
	TIterator* it = rooRealVarSet->createIterator();
	while (TObject* temp = it->Next()) {
		if (RooRealVar* rrv = dynamic_cast<RooRealVar*>(temp)) {
			if (StringUtils::isSubstring(rrv->GetName(), nameSubstring)){
				return rrv;
			}
		}
	}
	return nullptr;
}




//
