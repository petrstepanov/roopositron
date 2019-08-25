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
#include "../util/Debug.h"

#include "RooAbsPdf.h"
#include "RooArgSet.h"
#include "RooRealVar.h"
#include "RooCustomizer.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <regex>

ModelCommonizer::ModelCommonizer(RooAbsPdf* pdf, RooRealVar* observable, std::vector<std::string> defaultCommonNames) {
	this->observable = observable;
	// User constructs final array with common parameter names (adds from keyboard)
	commonParameterNames = initCommonParameters(pdf, defaultCommonNames);

	// Save common parameters to a private list
	RooArgSet* params = pdf->getParameters(*observable);
	TIterator* it = params->createIterator();
	while (TObject* object = it->Next()) {
		RooRealVar* rrv = dynamic_cast<RooRealVar*>(object);
		if (rrv && StringUtils::contains(rrv->GetName(), commonParameterNames)) {
			commonParameters->add(*rrv);
		}
	}

	// Output common parameters list
	Debug("ModelCommonizer::ModelCommonizer", "Common parameters extracted from the first spectrum");
	TIterator* it2 = commonParameters->createIterator();
	while (TObject* temp = it2->Next()) {
		if (TNamed* named = dynamic_cast<TNamed*>(temp)) {
			named->Print();
		}
	}
}

ModelCommonizer::~ModelCommonizer() {
}

std::vector<std::string> ModelCommonizer::initCommonParameters(RooAbsPdf* pdf, std::vector<std::string> defaultCommonNames) {
	// Output model parameters and highlight common ones by default (from parameters.txt)
	std::cout << std::endl << "Model has following parameters:" << std::endl;
	RooArgSet* parameters = pdf->getParameters(*observable);
	TIterator* it = parameters->createIterator();
	while (TObject* temp = it->Next()) {
		if (RooRealVar* rrv = dynamic_cast<RooRealVar*>(temp)) {
			Bool_t isCommon = StringUtils::contains(rrv->GetName(), defaultCommonNames);
			std::cout << std::left << std::setw(TAB_WIDTH) << rrv->GetName() << std::left << std::setw(2 * TAB_WIDTH) << rrv->GetTitle() << (isCommon ? "common" : "") << std::endl;
		}
	}

	// User input new names for parameters he wants to make common
	std::cout << std::endl << "Current common parameters are:" << std::endl;
	for (std::vector<std::string>::iterator it = defaultCommonNames.begin(); it != defaultCommonNames.end(); ++it) {
		std::cout << *it << " ";
	}
	std::cout << std::endl << std::endl << "Type parameter names you want to make common (space separated):" << std::endl;
	std::string input;
	getline(std::cin, input);
	std::vector<std::string> newCommonNames = StringUtils::parseString(input, " ");

	// Merge default and new names of common parameters
	defaultCommonNames.insert(defaultCommonNames.end(), newCommonNames.begin(), newCommonNames.end());
	return defaultCommonNames;
}

RooAbsPdf* ModelCommonizer::replaceParametersWithCommon(RooAbsPdf* pdf) {
	Debug("ModelCommonizer::replaceParametersWithCommon");

	// Instantiate Customizer that will replace model parameters
//	RooCustomizer* customizer = new RooCustomizer(*pdf, TString::Format("%s_custom", pdf->GetName()));
	RooCustomizer* customizer = new RooCustomizer(*pdf, "customized");
	Bool_t replacedFlag = kFALSE;

	// Iterate through model local parameters that might need replacement
	RooArgSet* parameters = pdf->getParameters(*observable);
	TIterator* it = parameters->createIterator();
	while (TObject* temp = it->Next()) {
		if (RooRealVar* parameter = dynamic_cast<RooRealVar*>(temp)) {
			// See if parameter name contains any common parameter name substring
			// E.g. 'tau_1' contains 'tau'. Then we replace it.
			RooRealVar* commonParameter = getCommonReplacement(parameter->GetName());
			if (commonParameter) {
				std::cout << "Found replacement for \"" << parameter->GetName() << "\" in common parameters - \"" << commonParameter->GetName() << "\"" << std::endl;
				replacedFlag = kTRUE;
				customizer->replaceArg(*parameter, *commonParameter);
			}
		}
	}
	if (replacedFlag) {
		RooAbsPdf* newPdf = (RooAbsPdf*) customizer->build(kTRUE);
		return newPdf;
	}
	return pdf;
}

RooRealVar* ModelCommonizer::getCommonReplacement(const char* localParameterName) {
	const std::string localParamName = localParameterName;
	const std::regex re("(.*)(_[0-9]+)$");
	std::smatch match;

	if (std::regex_match(localParamName, match, re)){
		// for "#tau1_source_10" returns "#tau1_source"
		std::string commonParamName = match[1].str();
		if (RooAbsArg* arg = commonParameters->find(commonParamName.c_str())){
			if (RooRealVar* commonParameter = dynamic_cast<RooRealVar*>(arg)){
				return commonParameter;
			}
		}
	}
	return NULL;
}
