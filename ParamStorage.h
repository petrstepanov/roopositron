/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ParamStorage.h
 * Author: petrstepanov
 *
 * Created on November 9, 2016, 6:08 PM
 *
 *  * Class that Stores RooRealVars in file
 */

#ifndef MY_PARAMSTORAGE
#define MY_PARAMSTORAGE

#include "TList.h"
#include "RooRealVar.h"
#include <iostream>

class ParamStorage {
public:
//    ParamStorage();
//    ParamStorage(const ParamStorage& orig);
//    virtual ~ParamStorage();    
    ParamStorage();
    ParamStorage(std::string filePostfix);
    Bool_t isSet(const char*);
    RooRealVar* get(const char*);
    Bool_t fileExists(std::string);
    RooRealVar* getOrMakeNew(const char* name, const char* description, Double_t value, Double_t minValue, Double_t maxValue, const char *unit = "");
    Bool_t save();
    Bool_t hasParameters();
private:
    std::string filename;
    TList* parameters;
    TList* _getParametersFromFile(const char*);    
};

#endif /* PARAMSTORAGE_H */