/*
 * To change this license header, choose License Headers in Project Propertiestring.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   StringUtilstring.cpp
 * Author: petrstepanov
 * 
 * Created on December 5, 2018, 11:33 AM
 */

#include "StringUtils.h"
#include <cstring>
#include <iostream>
#include <vector>

std::vector<const char*> StringUtils::parseString(const char* string, const char* delimeter) {
    std::string s = string;
    return parseString(s, delimeter);
}

std::vector<const char*> StringUtils::parseString(std::string string, const char* delimeter) {
    std::string delimiter = ",";
    std::vector<const char*> tokens;

    size_t pos = 0;
    std::string token;
    while ((pos = string.find(delimiter)) != std::string::npos) {
        token = string.substr(0, pos);
        // std::cout << token << std::endl;
        tokens.push_back(token.c_str());
        string.erase(0, pos + delimiter.length());
    }
    // std::cout << s << std::endl;
    tokens.push_back(string.c_str());
    return tokens;
}

const char* joinStrings(std::vector<const char*> tokens, const char* delimeter){
    if (tokens.size() == 1){
	return tokens.front();
    }
    std::string string = "";
    for(std::vector<const char*>::const_iterator i = tokens.begin(); i != tokens.end(); ++i) {
	string += *i;
	string += delimeter;
    }
    return string .c_str();
}