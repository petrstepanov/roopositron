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

// Private Singleton Members
ObjectNamer::ObjectNamer(){};

ObjectNamer* ObjectNamer::instance = NULL;

ObjectNamer* ObjectNamer::getInstance(){
    if (!instance){
        instance = new ObjectNamer;
    }
    return instance;
}

const char* ObjectNamer::getUniqueName(const char* name) {
    if (findName(name) == kFALSE){
        names.push_back(name);
        return name;
    }
    for (Int_t i = 2; ; i++) {
        std::string temp(name + std::to_string(i));
        const char* name_i = temp.c_str();
        if (findName(name_i) == kFALSE){
            std::cout << name_i << std::endl;
            names.push_back(name_i);
            return name_i;
        }
    }
}

Bool_t ObjectNamer::findName(const char* name) {
    if (std::find(names.begin(), names.end(), name) != names.end()){
        return kTRUE;
    }
    return kFALSE;
}
