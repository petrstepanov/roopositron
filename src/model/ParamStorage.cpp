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
#include "TSystem.h"
#include "../util/ObjectNamer.h"
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
    std::string createDirCommand = "mkdir " + postfix;
    gSystem->Exec(createDirCommand.c_str());
    filename = (postfix!="")?("./"+postfix+"/"):"./";
    filename += "parameters";
    filename += (postfix!="")?("-"+postfix):postfix;
    filename += ".txt";
    parameters = _getParametersFromFile(filename.c_str());
    if (parameters->GetEntries()>0){
        std::cout << "ParamStorage::ParamStorage parameters file read successful" << std::endl;            
        parameters->Print();
    }
    else {
        std::cout << "ParamStorage::ParamStorage parameters file empty - created new \"" << filename << "\"" << std::endl;            
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
        if (strcmp(type, "fixed") == 0){
            r->setConstant(kTRUE);
            std::cout << name << " is constant" << std::endl;
        }
        else {
            r->setError(error);
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
    // ObjectNamer makes sure we don't have two variables with same name
    // It generates "name1", "name2" - unique names for a given one
    ObjectNamer* namer = ObjectNamer::getInstance();
    const char* uniqueName = namer->getUniqueName(name);
    
    // Return variable if it was in file
    if (isSet(uniqueName)){
        return get(uniqueName);
    }
    // Or just define a new one
    RooRealVar* var;
    std::string strDescription(description);  
    std::string strName(uniqueName); 
    // If parameter is resolution function related - use default values
    if (strName.find("zero")==0 || strName.find("g1")*strName.find("g2")*strName.find("g3")==0){
        var = new RooRealVar(uniqueName, description, value, minValue, maxValue, unit);
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
            var->setConstant(kTRUE);
            // For correct rounding using autoprecision in legend
//            if(TString(unit).EqualTo("ns")){
//                var->setError(0.001);
//            }
//            if(TString(unit).EqualTo("%")){
//                var->setError(0.1);
//            }            
        }
        else {
            var = new RooRealVar(uniqueName, description, inputValue, inputMinValue, inputMaxValue, unit);        
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
        std::cout << "ParamStorage::save Error writing file " << filename << std::endl;
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