/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ModelCommonizer.h
 * Author: petrstepanov
 *
 * Created on December 6, 2018, 2:42 PM
 */

#ifndef COMMONPARAMETERS_H
#define COMMONPARAMETERS_H

#include "RooArgSet.h"
#include "RooAbsPdf.h"

class ModelCommonizer {
public:
    ModelCommonizer(RooAbsPdf* pdf, RooRealVar *observable, std::vector<std::string> defaultCommonNames);
    virtual ~ModelCommonizer();
    RooAbsPdf* replaceParametersWithCommon(RooAbsPdf* pdf);
    
private:
    RooRealVar* observable;
    RooArgSet* commonParameters = new RooArgSet();
    std::vector<std::string> commonParameterNames;
    
    std::vector<std::string> initCommonParameters(RooAbsPdf* pdf, std::vector<std::string> defaultCommonNames);
    RooRealVar* getCommonReplacement(const char* localParameterName);
    const int TAB_WIDTH = 20;
};

#endif /* COMMONPARAMETERS_H */

