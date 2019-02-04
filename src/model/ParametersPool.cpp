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
#include "../util/ObjectNamer.h"
#include "../util/StringUtils.h"
#include <sstream>
#include <iostream>

ParametersPool::ParametersPool(std::string ioPath) {
	filePathName = (ioPath == "") ? DEFAULT_FILENAME : ioPath + "/" + DEFAULT_FILENAME;
	parametersPool = readPoolParametersFromFile();
	constructExcludedParametersList();
}

void ParametersPool::constructExcludedParametersList() {
	// Don't save mean value (not important for results)
	parametersExcludedFromSave.push_back("gaussMean");
	parametersExcludedFromSave.push_back("bins");
	parametersExcludedFromSave.push_back("integral");
	parametersExcludedFromSave.push_back("background");

// Don't user input gauss FWHM's and Intensities
// Always use default values. Otherwise it's too much stuff
	parametersExcludedFromInput.push_back("gaussMean");
	parametersExcludedFromInput.push_back("background");
	parametersExcludedFromInput.push_back("gauss1FWHM");
	parametersExcludedFromInput.push_back("gauss2FWHM");
	parametersExcludedFromInput.push_back("gauss2Frac");
	parametersExcludedFromInput.push_back("gauss3FWHM");
	parametersExcludedFromInput.push_back("gauss3Frac");
	parametersExcludedFromInput.push_back("bins");
	parametersExcludedFromInput.push_back("integral");
}

RooArgSet* ParametersPool::readPoolParametersFromFile() {
	std::cout << "ParametersPool::readPoolParametersFromFile" << std::endl;
	std::cout << std::endl << "Reading parameters pool from hard drive (" << filePathName.c_str() << ")." << std::endl;
	RooArgSet* parametersPool = new RooArgSet();
	FILE * pFile;
	pFile = fopen(filePathName.c_str(), "r");
	if (pFile == NULL) {
		std::cout << "\"" << filePathName.c_str() << "\" file not found." << std::endl;
		return parametersPool;
	}
	char* name = new char[tab + 1];
	char* description = new char[tab * 2 + 1];
	char* unit = new char[tab + 1];
	char* type = new char[tab + 1];
	Double_t val, min, max, error;

	// Skip header
	char buffer[256];
	fgets(buffer, 256, pFile);

	// Read parameters
	// Scanf with spaces. https://stackoverflow.com/questions/2854488/reading-a-string-with-spaces-with-sscanf
	while (fscanf(pFile, "%s %lf %lf %lf %lf %s %s %[^\n]s", name, &val, &min, &max, &error, unit, type, description) != EOF) {
		RooRealVar* parameter = new RooRealVar(name, description, val, min, max, unit);
		if (strcmp(type, "fixed") == 0) {
			parameter->setConstant(kTRUE);
		} else {
			parameter->setError(error);
		}
		parameter->Print();
		parametersPool->add(*parameter);
	}
	fclose(pFile);

	std::cout << "\"" << filePathName.c_str() << "\" found. Successfully read " << parametersPool->getSize() << " values." << std::endl;
	return parametersPool;
}

void ParametersPool::updateModelParametersValuesFromPool(RooArgSet* modelParameters) {
	std::cout << std::endl << "ParametersPool::updateModelParametersValuesFromPool" << std::endl;
	std::cout << "Hit ENTER to use default parameter values." << std::endl;

	// Iterate through model parameters. Either extend values from pool parameters from hard drive.
	TIterator* it = modelParameters->createIterator();
	TObject* temp;
	while ((temp = it->Next())) {
		RooRealVar* parameter = dynamic_cast<RooRealVar*>(temp);
		if (parameter) {
			const char* name = parameter->GetName();
			RooRealVar* poolParameter = (RooRealVar*) parametersPool->find(name);
			// Either set model parameter value, error etc
			if (poolParameter) {
				std::cout << "Parameter " << poolParameter->GetName() << " found" << std::endl;
				// Fix: roofit can't set min larger than current max and vice versa
				parameter->setMin(std::numeric_limits<double>::min());
				parameter->setMax(std::numeric_limits<double>::max());
				parameter->setMin(poolParameter->getMin());
				parameter->setMax(poolParameter->getMax());
				parameter->setVal(poolParameter->getVal());
				// TODO: set error if not constant?
				parameter->setConstant(poolParameter->isConstant());
//		int a; std::cin >> a;
			} else {
				if (!StringUtils::stringContainsToken(parameter->GetName(), parametersExcludedFromInput)) {
					userInput(parameter);
				}
				if (!StringUtils::stringContainsToken(parameter->GetName(), parametersExcludedFromSave)) {
					parametersPool->add(*parameter);
				}
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
	} else {
		parameter->setMin(parameter->getVal());
		parameter->setMax(parameter->getVal());
	}
}

Bool_t ParametersPool::save(RooArgSet* modelParameters) {
	// Iterate through model parameters. Either extend values from pool parameters from hard drive.
	TIterator* it = modelParameters->createIterator();
	TObject* temp;
	while ((temp = it->Next())) {
		RooRealVar* modelParameter = dynamic_cast<RooRealVar*>(temp);
		if (modelParameter) {
			const char* name = modelParameter->GetName();
			RooRealVar* poolParameter = (RooRealVar*) parametersPool->find(name);
			// Either set model parameter value, error etc
			if (poolParameter) {
				parametersPool->replace(*poolParameter, *modelParameter);
			}
		}
	}

	std::cout << "ParametersPool::save" << std::endl;
	std::cout << std::endl << "Saving parameters pool to hard drive (" << filePathName.c_str() << ")." << std::endl;
	FILE* pFile = fopen(filePathName.c_str(), "w");
	if (pFile == NULL) {
		std::cout << "Error writing to file \"" << filePathName << "\"." << std::endl;
		return kFALSE;
	}

	fprintf(pFile, "%-*s%-*s%-*s%-*s%-*s%-*s%-*s%s\n", tab, "Parameter name", tab, "Value", tab, "Minimum", tab, "Maximum", tab, "Error", tab, "Unit", tab, "Type", "Description");

	// Save parameters
	it = parametersPool->createIterator();
	while ((temp = it->Next())) {
		RooRealVar* parameter = dynamic_cast<RooRealVar*>(temp);
		if (parameter) {
			std::string freeOrFixed = parameter->isConstant() ? "fixed" : "free";

			// To prevent reading error from parameters file - we replace empty parameter units with "-"
			std::string safeUnit = StringUtils::isEmpty(parameter->getUnit()) ? "-" : parameter->getUnit();

//			Double_t parameterMin = parameter->isConstant() ? parameter->getVal() : parameter->getMin();
//			Double_t parameterMax = parameter->isConstant() ? parameter->getVal() : parameter->getMax();
			Double_t parameterErr = parameter->isConstant() ? 0 : parameter->getMax();

			// https://stackoverflow.com/questions/23776824/what-is-the-meaning-of-s-in-a-printf-format-string/23777065
			fprintf(pFile, "%-*s%-*f%-*f%-*f%-*f%-*s%-*s%s\n", tab, parameter->GetName(), tab, parameter->getVal(), tab, parameter->getMin(), tab, parameter->getMax(), tab, parameterErr, tab,
					safeUnit.c_str(), tab, freeOrFixed.c_str(), parameter->GetTitle());
		}
	}

	fclose(pFile);
	return kTRUE;
}
