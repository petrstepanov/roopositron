/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ObjectNamer.cpp
 * Author: petrstepanov
 * 
 * Created on December 5, 2018, 7:57 AM
 */

#include "ObjectNamer.h"
#include <algorithm>
#include <iostream>
#include <sstream>

#include "TCollection.h"
#include "TObjString.h"
#include "TNamed.h"

ObjectNamer::ObjectNamer(const char* prefix) {
    this->prefix = prefix;
}

void ObjectNamer::fixUniquePdfAndParameterNames(RooAbsPdf* pdf, RooRealVar* observable) {
    // Fix pdf name
    fixUniqueName(pdf);

    // Fix pdf's coefficient names
    RooArgSet* params = pdf->getParameters(*observable);
    TIterator* it = params->createIterator();
    TObject* temp;
    while((temp = it->Next())){
	TNamed* named = dynamic_cast<TNamed*>(temp);
	if(named){
	    fixUniqueName(named);
	}
    }
}

void ObjectNamer::fixUniqueName(TNamed* object) {
    std::string name = object->GetName();
    // If name is not unique add prefix to it
    if (findName(name) == kTRUE){
	std::string newName = getPrefixedName(name);
	object->SetName(newName.c_str());
	names.push_back(newName);		
    } else {
    // Remember object's name
	names.push_back(name);	
    }
}

std::string ObjectNamer::getPrefixedName(std::string name) {
    Int_t suffix = 1;
    std::string newName;
    do {
	newName = name + prefix;
	newName += std::to_string(++suffix);
    } while(findName(newName) == kTRUE);
    return newName;
}

Bool_t ObjectNamer::findName(std::string name) {
    if (std::find(names.begin(), names.end(), name) != names.end()){
        return kTRUE;
    }
    return kFALSE;
}
