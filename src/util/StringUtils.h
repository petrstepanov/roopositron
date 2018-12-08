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
    static std::vector<std::string> parseString(const char* string, const char* delimeter = ",");
    static std::vector<std::string> parseString(std::string string, const char* delimeter = ",");
    static std::string joinStrings(std::vector<std::string> tokens, const char* delimeter = "-");
    static bool contains(const char* name, std::vector<std::string> names);
    static const char* appendSuffix(const char*, int suffix);
    static const char* appendSuffix(const char*, const char*);
    static bool isSubstring(const char* parent, const char* find);
};

#endif /* STRINGUTILS_H */

