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
    static bool containsSubstring(const char* string, std::vector<std::string> vector);
    static bool stringContainsToken(const char* string, std::vector<std::string> vector);
//    static const char* appendSuffix(const char*, int suffix);
    static bool isSubstring(const char* parent, const char* find);
    static bool isSubstring(std::string parent, std::string find);
    static bool stringEnds(std::string& string, std::string& ending);
    static std::string underscoreSuffix(const char*, const char*);
    static std::string suffix(const char* name, int index, const char* nameSuffix = "");
    static std::string ordinal(const char*, int index);
    static bool isEmpty(const char*);
};

#endif /* STRINGUTILS_H */

