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

ParametersPool::ParametersPool(std::string ioPath){
    filePathName = (ioPath == "") ? DEFAULT_FILENAME : ioPath + "/" + DEFAULT_FILENAME;
    parametersPool = readPoolParametersFromFile(filePathName.c_str());
    constructExcludedParametersList();
}

void ParametersPool::constructExcludedParametersList(){
    // Don't save mean value (not important for results)
    parametersExcludedFromSave.push_back("gaussMean");
    parametersExcludedFromSave.push_back("bgCount");
    
    // Don't user input gauss FWHM's and Intensities
    // Always use default values. Otherwise it's too much stuff
    parametersExcludedFromInput.push_back("gaussMean");
    parametersExcludedFromInput.push_back("bgCount");
    parametersExcludedFromInput.push_back("gauss1FWHM");
    parametersExcludedFromInput.push_back("gauss2FWHM");
    parametersExcludedFromInput.push_back("gauss2Frac");
    parametersExcludedFromInput.push_back("gauss3FWHM");
    parametersExcludedFromInput.push_back("gauss3Frac");

}

RooArgSet* ParametersPool::readPoolParametersFromFile(const char* filename){
    std::cout << std::endl << "Reading parameters pool from hard drive." << std::endl;
    RooArgSet* parametersPool = new RooArgSet();   
    FILE * pFile;
    pFile = fopen(filename, "r");
    if (pFile == NULL) {
	std::cout << "\"" << filename << "\" file not found." << std::endl; 	
        return parametersPool;
    }
    char* name = new char[64];
    char* description = new char[64];
    char* unit = new char[8];
    char* type = new char[8];
    Double_t val, min, max, error;

    // Skip header
    char buffer[256];
    fgets(buffer, 256, pFile);

    // Read parameters
    while (fscanf(pFile, "%s %s %lf %lf %lf %lf %s %s", name, description, &val, &min, &max, &error, unit, type) == 8){
	RooRealVar* parameter = new RooRealVar(name, description, val, min, max, unit);
	if (strcmp(type, "fixed") == 0){
	    parameter->setConstant(kTRUE);
	}
	else {
	    parameter->setError(error);
	}
	parametersPool->add(*parameter);
    }
    fclose(pFile);  
    
    std::cout << "\"" << filename << "\" found. Successfully read " << parametersPool->getSize() << " values." << std::endl;
    return parametersPool;
}

void ParametersPool::updateModelParametersValuesFromPool(RooArgSet* modelParameters){
    std::cout << std::endl << "Updating model parameters from pool." << std::endl;
    std::cout << "Hit ENTER to use default parameter values." << std::endl;

    // Iterate through model parameters. Either extend values from pool parameters from hard drive.
    TIterator* it = modelParameters->createIterator();
    TObject* temp;
    while((temp = it->Next())){
	RooRealVar* parameter = dynamic_cast<RooRealVar*>(temp);
	if(parameter){
	    const char* name = parameter->GetName();
	    RooRealVar* poolParameter = (RooRealVar*) parametersPool->find(name);
	    // Either set model parameter value, error etc
	    if (poolParameter){
		parameter->setVal(poolParameter->getVal());
		parameter->setMin(poolParameter->getMin());
		parameter->setMax(poolParameter->getMax());
		// TODO: set error if not constant?
		parameter->setConstant(poolParameter->isConstant());
	    } else {
		if (!StringUtils::stringContainsToken(parameter->GetName(), parametersExcludedFromInput)){
		    userInput(parameter);
		}
		if (!StringUtils::stringContainsToken(parameter->GetName(), parametersExcludedFromSave)){
		    parametersPool->add(*parameter);		    
		}
	    }
	}
    }
}

void ParametersPool::userInput(RooRealVar* parameter){
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
    std::cout << "Parameter is " << (parameter->isConstant() ? "fixed":"free") << ". Ok (y/n)? ";  
    std::getline(std::cin, input);
    if (!input.empty()) {
    	char character = input.at(0);
	if (character == 'n' || character == 'N') {
	    parameter->setConstant(!parameter->isConstant());
	    std::cout << "Set to " << (parameter->isConstant()?"fixed":"free") << "." << std::endl;
	}
    }
    if (!parameter->isConstant()){
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
    else {
	parameter->setMin(parameter->getVal());
	parameter->setMax(parameter->getVal());
    }
}

Bool_t ParametersPool::save(){
    std::cout << std::endl << "Saving parameters pool to hard drive." << std::endl;    
    FILE* pFile = fopen(filePathName.c_str(), "w");
    if (pFile == NULL) {
	std::cout << "Error writing to file \"" << filePathName << "\"." << std::endl; 
        return kFALSE;
    }
    
    const unsigned tab = 20;
    fprintf(pFile, "%-*s%-*s%-*s%-*s%-*s%-*s%-*s%-*s\n", tab, "Parameter name", 32, "Description", tab, "Value", tab, "Minimum", tab, "Maximum", tab, "Error", 10, "Unit", tab, "Type");

    TIterator* it = parametersPool->createIterator();
    TObject* temp;
    while((temp = it->Next())){
	RooRealVar* parameter = dynamic_cast<RooRealVar*>(temp);
	if(parameter){
	    std::string type = parameter->isConstant() ? "fixed" : "free";
	    fprintf(pFile, "%-*s%-*s%-*f%-*f%-*f%-*f%-*s%-*s\n", tab, parameter->GetName(), 32, parameter->GetTitle(), tab, parameter->getVal(), tab, parameter->getMin(), tab, parameter->getMax(), tab, parameter->getError(), 10, parameter->getUnit(), tab, type.c_str());
	}
    }
    
    fclose(pFile);
    return kTRUE;
}