/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ParametersPool.cpp
 * Author: petrstepanov
 * 
 * Created on November 9, 2016, 6:08 PM
 */

#include "ParametersPool.h"
#include "Constants.h"
#include "TSystem.h"
#include "../roofit/AdditiveConvolutionPdf.h"
#include "../util/ObjectNamer.h"
#include "../util/RootHelper.h"
#include "../util/StringUtils.h"
#include "../util/Debug.h"
#include <sstream>
#include <iostream>

ParametersPool::ParametersPool(std::string ioPath) {
	// Calculated parameters - aren't saved to file to avoid confusion
	parametersExcludedFromImport.push_back(AdditiveConvolutionPdf::VAR_MEAN_GAUSS_NAME);
	parametersExcludedFromImport.push_back(AdditiveConvolutionPdf::VAR_BINS_NAME);
	parametersExcludedFromImport.push_back(AdditiveConvolutionPdf::VAR_INTEGRAL_NAME);
	parametersExcludedFromImport.push_back(AdditiveConvolutionPdf::VAR_BACKGROUND_COUNT_NAME);

	// Don't user input gauss FWHM's and Intensities
	// Always use default values. Otherwise it's too much input going on
	parametersExcludedFromInput.push_back(AdditiveConvolutionPdf::VAR_MEAN_GAUSS_NAME);
	parametersExcludedFromInput.push_back(AdditiveConvolutionPdf::VAR_BACKGROUND_COUNT_NAME);
	parametersExcludedFromInput.push_back(AdditiveConvolutionPdf::VAR_BINS_NAME);
	parametersExcludedFromInput.push_back(AdditiveConvolutionPdf::VAR_INTEGRAL_NAME);
	parametersExcludedFromInput.push_back("FWHM_gauss1");
	parametersExcludedFromInput.push_back("FWHM_gauss2");
	parametersExcludedFromInput.push_back("FWHM_gauss3");
	parametersExcludedFromInput.push_back("Int_gauss2");
	parametersExcludedFromInput.push_back("Int_gauss3");

	filePathName = (ioPath == "") ? DEFAULT_FILENAME : ioPath + "/" + DEFAULT_FILENAME;
	parametersPool = readPoolParametersFromFile();
}

RooArgSet* ParametersPool::readPoolParametersFromFile() {
	#ifdef USEDEBUG
		std::cout << "ParametersPool::readPoolParametersFromFile" << std::endl << "Reading parameters pool from hard drive (" << filePathName.c_str() << ")." << std::endl;
	#endif
	RooArgSet* params = new RooArgSet();
	FILE * pFile;
	pFile = fopen(filePathName.c_str(), "r");
	if (pFile == NULL) {
		std::cout << "\"" << filePathName.c_str() << "\" file not found." << std::endl;
		return params;
	}
	char* name = new char[tab + 1];
	char* description = new char[tab * 2 + 1];
	char* unit = new char[tab + 1];
	char* type = new char[tab + 1];
	Double_t val, min, max, error;

	// Skip header
	char buffer[256];
	if (fgets(buffer, 256, pFile) == NULL) return params;

	// Read parameters
	// Scanf with spaces. https://stackoverflow.com/questions/2854488/reading-a-string-with-spaces-with-sscanf
	while (fscanf(pFile, "%s %lf %lf %lf %lf %s %s %[^\n]s", name, &val, &min, &max, &error, unit, type, description) != EOF) {
		RooRealVar* parameter = new RooRealVar(name, description, val, min, max, unit);
		if (strcmp(type, "fixed") == 0) {
			parameter->setConstant(kTRUE);
		} else {
			// parameter->setError(error);
		}
		if (!StringUtils::stringContainsToken(parameter->GetName(), parametersExcludedFromImport)) {
			#ifdef USEDEBUG
				std::cout << "Parameter imported:" << std::endl;
			#endif
			params->add(*parameter);
		}
		else {
			#ifdef USEDEBUG
				std::cout << "Parameter not imported (in excluded list):" << std::endl;
			#endif
		}
		#ifdef USEDEBUG
			parameter->Print();
		#endif
	}
	fclose(pFile);
	#ifdef USEDEBUG
		std::cout << "\"" << filePathName.c_str() << "\" found. Successfully read " << params->getSize() << " values." << std::endl;
	#endif
	return params;
}

Bool_t ParametersPool::containsAllParameters(RooArgSet* modelParameters){
	// Iterate through model parametersdeclared. Either extend values from pool parameters from hard drive.
	TIterator* it = modelParameters->createIterator();
	while (TObject* temp = it->Next()) {
		if (RooRealVar* parameter = dynamic_cast<RooRealVar*>(temp)) {
			const char* name = parameter->GetName();
			if (!StringUtils::stringContainsToken(parameter->GetName(), parametersExcludedFromImport)) {
				RooRealVar* poolParameter = (RooRealVar*) parametersPool->find(name);
				if (!poolParameter) {
					return kFALSE;
				}
			}
		}
	}
	return kTRUE;
}

// Tips: RooArgSet works like a set of pointers to objects
//       originally being read from a file it stores some RooRealVar variables.

//       When updating model parameters from pool we have to:
//		 1. set model parameter value and limits from pool parameter
//       2. remove existing pool parameter from set.
//		 3. add model parameter to set.

//void ParametersPool::addUpdateModelParameters(RooArgSet* modelParameters) {
//	#ifdef USEDEBUG
//		std::cout << "ParametersPool::addUpdateModelParameters" << std::endl;
//	#endif

//	
//
//	// Iterate through model parameters. Either extend values from pool parameters from hard drive.
//	TIterator* it = modelParameters->createIterator();
//	while (TObject* temp = it->Next()) {
//		if (RooRealVar* modelParameter = dynamic_cast<RooRealVar*>(temp)) {
//			// Either set model parameter value, error etc
//			if (RooRealVar* poolParameter = (RooRealVar*) parametersPool->find(modelParameter->GetName())) {
//				if (poolParameter != modelParameter){
//					RootHelper::setRooRealVarValueLimits(modelParameter, poolParameter->getVal(), poolParameter->getMin(), poolParameter->getMax());
//					// TODO: set error if not constant?
//					modelParameter->setConstant(poolParameter->isConstant());
//					parametersPool->remove(*poolParameter);
//					parametersPool->add(*modelParameter);
//				}
//			} else {
//				parametersPool->add(*modelParameter);
//			}
//		}
//	}
//}

void ParametersPool::updateModelParameters(RooArgSet* modelParameters) {
	#ifdef USEDEBUG
		std::cout << "ParametersPool::addUpdateModelParameters" << std::endl;
	#endif

	// Iterate through model parameters. Either extend values from pool parameters from hard drive.
	TIterator* it = modelParameters->createIterator();
	while (TObject* temp = it->Next()) {
		if (RooRealVar* modelParameter = dynamic_cast<RooRealVar*>(temp)) {
			// Either set model parameter value, error etc
			if (RooRealVar* poolParameter = (RooRealVar*) parametersPool->find(modelParameter->GetName())) {
				if (poolParameter != modelParameter){
					RootHelper::setRooRealVarValueLimits(modelParameter, poolParameter->getVal(), poolParameter->getMin(), poolParameter->getMax());
					// TODO: set error if not constant?
					modelParameter->setConstant(poolParameter->isConstant());
				}
			}
		}
	}
}

void ParametersPool::addInputModelParameters(RooArgSet* modelParameters) {
	#ifdef USEDEBUG
		std::cout << "ParametersPool::inputMissingPoolParameters" << std::endl << "Hit ENTER to use default parameter values." << std::endl;
	#endif

	// Iterate through model parameters. Either extend values from existing pool or user input and add to pool.
	TIterator* it = modelParameters->createIterator();
	while (TObject* temp = it->Next()) {
		if (RooRealVar* modelParameter = dynamic_cast<RooRealVar*>(temp)) {
			// Either set model parameter value, error etc
			if (RooRealVar* poolParameter = (RooRealVar*) parametersPool->find(modelParameter->GetName())) {
				if (poolParameter != modelParameter){
					RootHelper::setRooRealVarValueLimits(modelParameter, poolParameter->getVal(), poolParameter->getMin(), poolParameter->getMax());
					// TODO: set error if not constant?
					modelParameter->setConstant(poolParameter->isConstant());
					parametersPool->remove(*poolParameter);
					parametersPool->add(*modelParameter);
					poolParameter->Delete();
				}
			} else {
				if (!StringUtils::stringContainsToken(modelParameter->GetName(), parametersExcludedFromInput)) {
					userInput(modelParameter);
				}
				parametersPool->add(*modelParameter);
			}
		}
	}
}

void ParametersPool::userInput(RooRealVar* parameter) {
//	std::cout << "ParametersPool::userInput" << std::endl;
	std::string input;
	std::cout << std::endl << "Input \"" << parameter->GetName() << "\" - " << parameter->GetTitle() << "." << std::endl;

	std::cout << "Value (default is " << parameter->getVal() << " " << parameter->getUnit() << "): ";
	std::getline(std::cin, input);
	if (!input.empty()) {
		Double_t value;
		std::istringstream stream(input);
		stream >> value;
		parameter->setVal(value);
	}
//	parameter->Print();
	std::cout << "Parameter is " << (parameter->isConstant() ? "fixed" : "free") << ". Ok (y/n)? ";
	std::getline(std::cin, input);
	if (!input.empty()) {
		char character = input.at(0);
		if (character == 'n' || character == 'N') {
			parameter->setConstant(!parameter->isConstant());
			std::cout << "Set to " << (parameter->isConstant() ? "fixed" : "free") << "." << std::endl;
		}
	}
	if (!parameter->isConstant()) {
		std::cout << "Min value (default is " << parameter->getMin() << " " << parameter->getUnit() << "): ";
		std::getline(std::cin, input);
		if (!input.empty()) {
			Double_t value;
			std::istringstream stream(input);
			stream >> value;
			parameter->setMin(value);
		}

		std::cout << "Max value (default is " << parameter->getMax() << " " << parameter->getUnit() << "): ";
		std::getline(std::cin, input);
		if (!input.empty()) {
			Double_t value;
			std::istringstream stream(input);
			stream >> value;
			parameter->setMax(value);
		}
	}
//	else {
//		parameter->setMin(parameter->getVal());
//		parameter->setMax(parameter->getVal());
//	}
}

// TODO: separate add and save()
Bool_t ParametersPool::saveToFile() {
	#ifdef USEDEBUG
		std::cout << "ParametersPool::saveToFile" std::endl << "Saving parameters pool to hard drive (" << filePathName.c_str() << ")." << std::endl;
	#endif

	FILE* pFile = fopen(filePathName.c_str(), "w");
	if (pFile == NULL) {
		std::cout << "Error writing to file \"" << filePathName << "\"." << std::endl;
		return kFALSE;
	}

	fprintf(pFile, "%-*s%-*s%-*s%-*s%-*s%-*s%-*s%s\n", tab, "Parameter name", tab, "Value", tab, "Minimum", tab, "Maximum", tab, "Error", tab, "Unit", tab, "Type", "Description");

	// Save parameters
	TIterator* it = parametersPool->createIterator();
	while (TObject* temp = it->Next()) {
		if (RooRealVar* parameter = dynamic_cast<RooRealVar*>(temp)) {
			if (!StringUtils::stringContainsToken(parameter->GetName(), parametersExcludedFromImport)) {
				std::string freeOrFixed = parameter->isConstant() ? "fixed" : "free";

				// To prevent reading error from parameters file - we replace empty parameter units with "-"
				std::string safeUnit = StringUtils::isEmpty(parameter->getUnit()) ? "-" : parameter->getUnit();

	//			Double_t parameterMin = parameter->isConstant() ? parameter->getVal() : parameter->getMin();
	//			Double_t parameterMax = parameter->isConstant() ? parameter->getVal() : parameter->getMax();
				Double_t parameterErr = parameter->isConstant() ? 0 : parameter->getError();

				// https://stackoverflow.com/questions/23776824/what-is-the-meaning-of-s-in-a-printf-format-string/23777065
				fprintf(pFile, "%-*s%-*f%-*f%-*f%-*f%-*s%-*s%s\n", tab, parameter->GetName(), tab, parameter->getVal(), tab, parameter->getMin(), tab, parameter->getMax(), tab, parameterErr, tab,
						safeUnit.c_str(), tab, freeOrFixed.c_str(), parameter->GetTitle());
				#ifdef USEDEBUG
						parameter->Print();
				#endif
			}
		}
	}

	fclose(pFile);
	return kTRUE;
}
