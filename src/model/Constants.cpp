/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Constants.cpp
 * Author: petrstepanov
 * 
 * Created on November 9, 2016, 8:07 PM
 */

#include "Constants.h"
#include "../util/StringUtils.h"
#include "RooStringVar.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <list>
#include <string>
#include <cstdlib>

Constants::Constants() {
    // Init default constant values
    constants = initDefaultValues();
    // Replace some default values with values read from file
    extendFromFile(constants, DEFAULT_FILENAME);
    writeToFile(constants, DEFAULT_FILENAME);
    print();
    
    RooStringVar* var = (RooStringVar*) constants->find("channelWidth");
    channelWidth = new RooConstVar("channelWidth", "Width of channel, ns", atof(var->getVal()));    
}

Constants* Constants::instance = NULL;

Constants* Constants::getInstance(){
    if (!instance){
        instance = new Constants;
    }
    return instance;
}

RooConstVar* Constants::fwhm2disp = new RooConstVar("fwhm2disp", "Coefficient to convert fwhm to dispersion", 1./(2.*sqrt(2.*log(2.))));

Constants::~Constants() {
}

RooArgList* Constants::initDefaultValues() {
    RooArgList* constants = new RooArgList();
    constants->add(RooStringVar("channels", "# entries in maestro file", "8192"));
    constants->add(RooStringVar("channelWidth", "# channel width, ns", "0.006186"));
    constants->add(RooStringVar("skipLines", "# spectrum header lines", "12"));
    constants->add(RooStringVar("minChannel", "# left fit channel", "1000"));
    constants->add(RooStringVar("maxChannel", "# right fit channel", "4000"));
    constants->add(RooStringVar("excludeMinChannel", "# left exclude channel", "0"));
    constants->add(RooStringVar("excludeMaxChannel", "# right exclude channel", "0"));
    constants->add(RooStringVar("resolutionFunction", "# \"2gauss\" or \"3gauss\"", "2gauss"));
    constants->add(RooStringVar("decayModel", "# comma-separated combination of \"exp\", \"trapping\", \"grain\", e.g. \"exp,exp\"", "exp"));
    constants->add(RooStringVar("commonParameters", "# comma-separated parameters of simultaneous fit", "gauss1FWHM,gauss2FWHM,gauss3FWHM,tSource"));
    constants->add(RooStringVar("imageWidth", "# output image width", "1280"));
    constants->add(RooStringVar("imageHeight", "# output image height", "700"));
    return constants;
}

bool Constants::extendFromFile(RooArgList* constants, std::string filename){
    std::cout << std::endl << "Reading app constants from file \"" << filename << "\"." << std::endl; 
    RooArgSet* constantsFromFile = new RooArgSet();
    
    std::ifstream constantsFile(filename.c_str());
    if (!constantsFile.is_open()) {
        std::cout << "\"" << filename << "\" not found. Using default values." << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(constantsFile, line)){
	// Read name and value
	std::istringstream streamLine(line);
	std::string name, value;
	streamLine >> name;	
	streamLine >> value;
	
	// Read description
	// std::size_t pos = line.find("#"); 
	// std::string description = line.substr(pos);
	constantsFromFile->add(RooStringVar(name.c_str(), "", value.c_str()));
    }    
    constantsFile.close();

    // Iterate default constants and update their values with ones from file
    TIterator* it = constants->createIterator();
    TObject* temp;
    while((temp = it->Next())){
	RooStringVar* constant = dynamic_cast<RooStringVar*>(temp);
	if(constant){
	    const char* name = constant->GetName();
	    RooStringVar* constantFromFile = (RooStringVar*) constantsFromFile->find(name);
	    if (constantFromFile){
		constant->setVal(constantFromFile->getVal());
	    }
	}
    }
    
    std::cout << "\"" << filename << "\" found. Successfully read " << constantsFromFile->getSize() 
	      << " out of " << constants->getSize() << "values." << std::endl;
    return true;
}

void Constants::print(){
    std::cout << std::endl;
    TIterator* it = constants->createIterator();
    TObject* temp;
    while((temp = it->Next())){
	RooStringVar* constant = dynamic_cast<RooStringVar*>(temp);
	if (constant){
	    std::cout << std::left << std::setw(TAB_SIZE) << constant->GetName()
		      << std::left << std::setw(TAB_SIZE) << constant->getVal()
		      << constant->GetTitle() << std::endl;
	}
    }      
}

void Constants::writeToFile(RooArgList* constants, std::string filename){
    std::cout << std::endl << "Writing app constants to file \"" << filename << "\"." << std::endl; 
    std::ofstream myfile;
    myfile.open(filename.c_str());
    
    TIterator* it = constants->createIterator();
    TObject* temp;
    while((temp = it->Next())){
	RooStringVar* constant = dynamic_cast<RooStringVar*>(temp);
	constant->Print();
	if (constant){
	    myfile << std::left << std::setw(TAB_SIZE) << constant->GetName()
		   << std::left << std::setw(TAB_SIZE) << constant->getVal()
		   << constant->GetTitle() << std::endl;
	}
    }
    myfile.close();  
}

int Constants::getNumberOfChannels(){
    RooStringVar* var = (RooStringVar*) constants->find("channels");
    return atoi(var->getVal());
}

double Constants::getChannelWidth(){
    RooStringVar* var = (RooStringVar*) constants->find("channelWidth");
    return atof(var->getVal());
}

RooConstVar* Constants::getRooChannelWidth(){
    return channelWidth;
}

int Constants::getSkipLines(){
    RooStringVar* var = (RooStringVar*) constants->find("skipLines");
    return atoi(var->getVal());
}

int Constants::getMinChannel(){
    RooStringVar* var = (RooStringVar*) constants->find("minChannel");
    return atoi(var->getVal());
}

int Constants::getMaxChannel(){
    RooStringVar* var = (RooStringVar*) constants->find("maxChannel");
    return atoi(var->getVal());
}

int Constants::getExcludeMinChannel() {
    RooStringVar* var = (RooStringVar*) constants->find("excludeMinChannel");
    return atoi(var->getVal());
}

int Constants::getExcludeMaxChannel() {
    RooStringVar* var = (RooStringVar*) constants->find("excludeMaxChannel");
    return atoi(var->getVal());
}

const char* Constants::getResolutionFunctionModel(){
    RooStringVar* var = (RooStringVar*) constants->find("resolutionFunction");    
    return var->getVal();
}

std::vector<std::string> Constants::getDecayModels() {
    RooStringVar* var = (RooStringVar*) constants->find("decayModel");       
    return StringUtils::parseString(var->getVal());
}

std::vector<std::string> Constants::getCommonParameters() {
    RooStringVar* var = (RooStringVar*) constants->find("commonParameters");       
    return StringUtils::parseString(var->getVal());
}

int Constants::getImageWidth(){
    RooStringVar* var = (RooStringVar*) constants->find("imageWidth");
    return atoi(var->getVal());
}

int Constants::getImageHeight(){
    RooStringVar* var = (RooStringVar*) constants->find("imageHeight");
    return atoi(var->getVal());
}