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

class ObjectNamer {
public:
    static ObjectNamer* getInstance();
    const char* getUniqueName(const char* name);
    
private:
    ObjectNamer();
    static ObjectNamer* instance;
    std::vector<std::string> names;
    Bool_t findName(const char* name);
};

#endif /* OBJECTNAMER_H */

