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
#include <sstream>
#include <iostream>
#include <vector>

std::vector<std::string> StringUtils::parseString(const char* string, const char* delimeter) {
    std::string s = string;
    return parseString(s, delimeter);
}

std::vector<std::string> StringUtils::parseString(std::string s, const char* d) {
    std::string delimiter = d;
    size_t pos = 0;
    std::string token;
    std::vector<std::string> tokens;
//    std::cout << "StringUtils::parseString" << std::endl;     
//    std::cout << "parsing: \"" << s << "\""<< std::endl;
    while ((pos = s.find(delimiter)) != std::string::npos) {
	token = s.substr(0, pos);
//	std::cout << "token: \"" << token << "\"" << std::endl;
	tokens.push_back(token);
	s.erase(0, pos + delimiter.length());
    }
//    std::cout << "token: \"" << s << "\"" << std::endl;
    tokens.push_back(s);
    return tokens;
}

std::string StringUtils::joinStrings(std::vector<std::string> tokens, const char* delimeter) {
//    std::cout << "StringUtils::joinStrings" << std::endl;  
    if (tokens.size() == 1){
//	std::cout << "token: \"" << tokens.front() << "\"" <<std::endl;
//	std::cout << "join: \"" << tokens.front().c_str() << "\"" << std::endl;	
	return tokens.front();
    }
    std::string string = ""; 
    for(std::vector<std::string>::const_iterator it = tokens.begin(); it != tokens.end(); ++it) {
//	std::cout << "token: \"" << *it << "\"" <<std::endl;
	string += *it;
	string += delimeter;
    }
    string = string.substr(0, string.length() - 1);
//    std::cout << "join: \"" << string.c_str() << "\"" << std::endl;    
    return string;
}

bool StringUtils::contains(const char* string, std::vector<std::string> vector) {
    if (std::find(vector.begin(), vector.end(), string) != vector.end()){
	return true;
    }
    return false;
}

const char* StringUtils::appendSuffix(const char* string, int suffix) {
    std::string s = std::to_string(suffix);
    return appendSuffix(string, s.c_str());
}

const char* StringUtils::appendSuffix(const char* string, const char* suffix) {
    std::string temp = string;
    temp += "_";
    temp += suffix;
    return temp.c_str();
}

bool StringUtils::isSubstring(const char* parent, const char* child) {
    std::string parentString (parent);
    std::string childString (child);
    std::size_t found = parentString.find(childString);
    if (found != std::string::npos){
	return true;
    }
    return false;
}

bool StringUtils::stringEnds(std::string& string, std::string& ending) {
    if (string.length() >= ending.length()) {
        return (0 == string.compare(string.length() - ending.length(), ending.length(), ending));
    }
    else {
        return false;
    }
}