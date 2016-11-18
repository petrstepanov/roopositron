/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ParamStorage.cpp
 * Author: petrstepanov
 * 
 * Created on November 9, 2016, 6:08 PM
 */

#include "ParamStorage.h"
#include "Constants.h"

//ParamStorage::ParamStorage() {
//}
//
//ParamStorage::ParamStorage(const ParamStorage& orig) {
//}
//
//ParamStorage::~ParamStorage() {
//}


ParamStorage::ParamStorage(){
    ParamStorage("");
}

ParamStorage::ParamStorage(std::string postfix){
    filename = "parameters";
    filename += (postfix!="")?("_"+postfix):postfix;
    filename += ".txt";
    parameters = _getParametersFromFile(filename.c_str());
    if (parameters->GetEntries()>0){
        std::cout << "ParamStorage: parameters file read successful" << std::endl;            
        parameters->Print();
    }
    else {
        std::cout << "ParamStorage: parameters file empty - created new \"" << filename << "\"" << std::endl;            
    }        
}

TList* ParamStorage::_getParametersFromFile(const char* filename){
    FILE * pFile;
    pFile = fopen(filename, "r");
    if (pFile == NULL) {
        return new TList();
    }
    char* name = new char[64];
    char* description = new char[64];
    char* unit = new char[8];
    char* type = new char[8];
    Double_t val, min, max, error;
    TList* params = new TList();
    // Skip header
    char buffer[256];
    fgets(buffer, 256, pFile);
    while (fscanf(pFile, "%s %s %lf %lf %lf %lf %s %s", name, description, &val, &min, &max, &error, unit, type) == 8){
        RooRealVar* r = new RooRealVar(name, description, val, min, max, unit);
        r->setError(error);
        if (strcmp(type, "fixed") == 0){
            r->setConstant(kTRUE);
            std::cout << name << " is constant" << std::endl;
        }
        params->Add(r);
    }
    fclose(pFile);        
    return params;
}

Bool_t ParamStorage::hasParameters(){
    return parameters!=NULL;
}

RooRealVar* ParamStorage::get(const char* name){
    TObject* obj = parameters->FindObject(name);
    if (obj->InheritsFrom(RooRealVar::Class())){
        RooRealVar* var = (RooRealVar*)obj;
        return var;
    }
    else return NULL;
}

Bool_t ParamStorage::isSet(const char* name){
    TObject* obj = parameters->FindObject(name);
    return obj != NULL;
}

RooRealVar* ParamStorage::getOrMakeNew(const char* name, const char* description, Double_t value, Double_t minValue, Double_t maxValue, const char *unit){
    // Return variable if it was in file
    if (isSet(name)){
        return get(name);
    }
    // Or just define a new one
    RooRealVar* var;
    std::string strDescription(description);  
    std::string strName(name); 
    // If parameter is resolution function related - use default values
    if (strName.find("zero")*strName.find("g1")*strName.find("g2")==0){
        var = new RooRealVar(name, description, value, minValue, maxValue, unit);
    }
    // For model parameters - ask to input from keyboard 
    else {
        Double_t inputValue;
        Double_t inputMinValue;
        Double_t inputMaxValue;
        Bool_t isFixed;
        std::replace(strDescription.begin(), strDescription.end(), '_', ' ');
        std::cout << std::endl << "Input \"" << name << "\" - " << strDescription << ", " << unit << "." << std::endl;
        std::cout << "Value: ";
        std::cin >> inputValue;
        std::cout << "Free \"0\" or fixed \"1\": ";
        std::cin >> isFixed;
        if (!isFixed){
            std::cout << "Min value: ";
            std::cin >> inputMinValue;        
            std::cout << "Max value: ";
            std::cin >> inputMaxValue;
        }
        if (isFixed){
            var = new RooRealVar(name, description, inputValue, inputValue, inputValue, unit);      
        }
        else {
            var = new RooRealVar(name, description, inputValue, inputMinValue, inputMaxValue, unit);        
        }
    }
    parameters->Add(var);
    return var;
}

Bool_t ParamStorage::fileExists(std::string filename){
    FILE* pFile = fopen(filename.c_str(), "r");
    if (pFile == NULL) return kFALSE;
    fclose(pFile);
    return kTRUE;
}

Bool_t ParamStorage::save(){
    FILE* pFile = fopen(filename.c_str(), "w");
    if (pFile == NULL) {
        std::cout << "Error writing file" << std::endl;
        return kFALSE;
    }
    TIter next(parameters);
    const unsigned tab = 20;
    fprintf(pFile, "%-*s%-*s%-*s%-*s%-*s%-*s%-*s%-*s\n", tab, "Variable name", 32, "Description", tab, "Value", tab, "Minimum", tab, "Maximum", tab, "Error", 10, "Unit", tab, "Type");
    while (TObject *to = next()){
        if (to->InheritsFrom(RooRealVar::Class())){
            RooRealVar* var = (RooRealVar*)to;
            const char* type = var->isConstant() ? "fixed" : "free";
            fprintf(pFile, "%-*s%-*s%-*f%-*f%-*f%-*f%-*s%-*s\n", tab, var->GetName(), 32, var->GetTitle(), tab, var->getVal(), tab, var->getMin(), tab, var->getMax(), tab, var->getError(), 10, var->getUnit(), tab, type);
        }
    }
    fclose(pFile);
    return kTRUE;
}