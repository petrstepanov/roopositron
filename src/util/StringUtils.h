/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   StringUtils.h
 * Author: petrstepanov
 *
 * Created on December 5, 2018, 11:33 AM
 */

#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <iostream>

class StringUtils {
public:
    static std::vector<const char*> parseString(const char* string, const char* delimeter = ",");
    static std::vector<const char*> parseString(std::string string, const char* delimeter = ",");
    static const char* joinStrings(std::vector<const char*> tokens, const char* delimeter = "-");
};

#endif /* STRINGUTILS_H */

