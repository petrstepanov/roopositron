/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ObjectNamer.h
 * Author: petrstepanov
 *
 * Created on December 5, 2018, 7:57 AM
 */

#ifndef OBJECTNAMER_H
#define OBJECTNAMER_H

#include <TList.h>
#include <RooAbsPdf.h>
#include <RooRealVar.h>

class ObjectNamer {
public:
    ObjectNamer(const char* prefix = "");
    void fixUniquePdfAndParameterNames(RooAbsPdf* pdf, RooRealVar* observable);
    static void suffixAllModelParameters(RooAbsPdf* pdf, RooRealVar* observable, Int_t suffix);
    
private:
    void fixUniqueName(TNamed* object);

    std::string prefix;
    std::vector<std::string> names;   
    std::string getPrefixedName(std::string name);
    Bool_t findName(std::string name);
};

#endif /* OBJECTNAMER_H */

