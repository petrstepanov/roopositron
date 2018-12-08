/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ModelCommonizer.cpp
 * Author: petrstepanov
 * 
 * Created on December 6, 2018, 2:42 PM
 */

#include "ModelCommonizer.h"
#include "../util/StringUtils.h"

#include "RooAbsPdf.h"
#include "RooArgSet.h"
#include "RooRealVar.h"
#include "RooCustomizer.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

ModelCommonizer::ModelCommonizer(RooAbsPdf* pdf, RooRealVar* observable, std::vector<std::string> defaultCommonNames) {
    this->observable = observable;
    // User constructs final array with common parameter names
    commonParameterNames = initCommonParameters(pdf, defaultCommonNames);

    // Save common parameter pointers to a private list
    RooArgSet* params = pdf->getParameters(*observable);
    TIterator* it = params->createIterator();
    TObject* object;
    while((object = it->Next())){
	RooRealVar* rrv = dynamic_cast<RooRealVar*>(object);
	if (rrv && StringUtils::contains(rrv->GetName(), commonParameterNames)){
	    commonParameters->add(*rrv);
	}
    }
}

ModelCommonizer::~ModelCommonizer() {
}

std::vector<std::string> ModelCommonizer::initCommonParameters(RooAbsPdf* pdf, std::vector<std::string> defaultCommonNames) {
    // List model parameters and highlight common ones by default (from parameters.txt)
    std::cout << std::endl << "Model has following parameters:" << std::endl;
    RooArgSet* parameters = pdf->getParameters(*observable);
    TIterator* it = parameters->createIterator();
    TObject* temp;
    while((temp = it->Next())){
	RooRealVar* rrv = dynamic_cast<RooRealVar*>(temp);
	if(rrv){
	    Bool_t isCommon = StringUtils::contains(rrv->GetName(), defaultCommonNames);
	    std::cout << std::left << std::setw(TAB_WIDTH) << rrv->GetName() 
		      << std::left << std::setw(2*TAB_WIDTH) << rrv->GetTitle()
		      << (isCommon?"common":"") << std::endl;
	}
    }

    // User input new names for parameters he wants to make common
    std::cout << std::endl << "Current common parameters are:" << std::endl;
    for (std::vector<std::string>::iterator it = defaultCommonNames.begin(); it != defaultCommonNames.end(); ++it){
	std::cout << *it << " ";
    }
    std::cout << std::endl << std::endl << "Type parameters name you want to add to common list:" << std::endl;
    std::string input;
    getline(std::cin, input);
    std::vector<std::string> newCommonNames = StringUtils::parseString(input, " ");

    // Merge default and new names of common parameters
    defaultCommonNames.insert(defaultCommonNames.end(), newCommonNames.begin(), newCommonNames.end() );
    return defaultCommonNames;
}

RooAbsPdf* ModelCommonizer::replaceParametersWithCommon(RooAbsPdf* pdf) {
    // Instantiate Customizer that will replace model parameters
    RooCustomizer* customizer = new RooCustomizer(*pdf, StringUtils::appendSuffix(pdf->GetName(), "custom"));
    Bool_t replacedFlag = kFALSE;
    
    // Iterate through model local parameters that might need replacement
    RooArgSet* parameters = pdf->getParameters(*observable);
    TIterator* it = parameters->createIterator();
    TObject* temp;
       while((temp = it->Next())){
	RooRealVar* parameter = dynamic_cast<RooRealVar*>(temp);
	if(parameter){
	    // See if parameter name contains any common parameter name substring
	    // E.g. 'tau_1' contains 'tau'. Then we replace it.
	    RooRealVar* commonParameter = getCommonReplacement(parameter->GetName());
	    if (commonParameter){
		replacedFlag = kTRUE;
		customizer->replaceArg(*parameter,*commonParameter);
	    }
	}
    }    
    if (replacedFlag){
	RooAbsPdf* newPdf = (RooAbsPdf*) customizer->build(kTRUE);
	return newPdf;
    }
    return pdf;
}

RooRealVar* ModelCommonizer::getCommonReplacement(const char* localParameterName) {
    TIterator* it = commonParameters->createIterator();
    TObject* temp;
    while((temp = it->Next())){
	RooRealVar* commonParameter = dynamic_cast<RooRealVar*>(temp);
	if(commonParameter){
	    // Replace all tau_# with tau (adding underscore to not interfere with tau_2)
	    std::string commonParameterName_ = commonParameter->GetName();
	    commonParameterName_ += "_";
	    Bool_t match = StringUtils::isSubstring(localParameterName, commonParameterName_.c_str());
	    if (match){
		return commonParameter;
	    }
	}
    }
    return NULL;
}