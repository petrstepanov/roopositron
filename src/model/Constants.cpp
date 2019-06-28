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
#include "../util/Debug.h"
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

//	RooStringVar* var = (RooStringVar*) constants->find("channelWidth");
//	channelWidth = new RooConstVar("channelWidth", "Width of channel, ns", atof(var->getVal()));
}

Constants* Constants::instance = NULL;

Constants* Constants::getInstance() {
	if (!instance) {
		instance = new Constants;
	}
	return instance;
}

RooConstVar* Constants::fwhm2disp = new RooConstVar("fwhm2disp", "Coefficient to convert fwhm to dispersion", 1. / (2. * sqrt(2. * log(2.))));

Constants::~Constants() {
}

RooArgList* Constants::initDefaultValues() {
	RooArgList* constants = new RooArgList();
	RooStringVar* constant;
    constant = new RooStringVar("channels", " # entries in maestro file", "8192");
    constants->add(*constant);
	constant = new RooStringVar("defaultChannelWidth", " # channel width, ns", "0.006186");
	constants->add(*constant);
	constant = new RooStringVar("skipLines", " # spectrum header lines", "12");
	constants->add(*constant);
	constant = new RooStringVar("minChannel", " # left fit channel; allow 200 left bins for background calculation", "600");
	constants->add(*constant);
	constant = new RooStringVar("maxChannel", " # right fit channel", "4000");
	constants->add(*constant);
	constant = new RooStringVar("excludeMinChannel", " # left exclude channel", "0");
	constants->add(*constant);
	constant = new RooStringVar("excludeMaxChannel", " # right exclude channel", "0");
	constants->add(*constant);
	constant = new RooStringVar("minimizerType", " # \"Minuit\", \"Minuit2\", \"Fumili\", \"GSLMultiMin\"", "Minuit");
	constants->add(*constant);
	constant = new RooStringVar("decayModel", " # comma-separated names \"exp\", \"2exp\", \"3exp\", \"trapping\", \"grain\", \"powder\"", "exp");
	constants->add(*constant);
	constant = new RooStringVar("resolutionFunction", " # \"1gauss\", \"2gauss\" or \"3gauss\"", "2gauss");
	constants->add(*constant);
	constant = new RooStringVar("sourceComponents", " # number of components in source: 1, 2, 3, etc", "1");
	constants->add(*constant);
	constant =
			new RooStringVar("commonParameters", " # comma-separated parameters of simultaneous fit",
					"channelWidth,FWHM_gauss1,FWHM_gauss2,FWHM_gauss3,Int_gauss2,Int_gauss3,#tau_source,Int_source,#tau1_source,#tau2_source,#tau3_source,Int_expSource2,Int_expSource3");
	constants->add(*constant);
	constant = new RooStringVar("bufferFraction", " # set to about 1.5-2 when dealing with larger lifetime components ~5ns", "0.2");
	constants->add(*constant);
	constant = new RooStringVar("convolutionBins", " # number of bins for convolution (reduce for speed, increase for fit quality)", "1024");
	constants->add(*constant);
	constant = new RooStringVar("backgroundBins", " # number of left bins for fixed background calculation; set 0 for free background", "150");
	constants->add(*constant);
	constant = new RooStringVar("asciiDelimeter", " # column delimeter in plot ascii files", "\\t");
	constants->add(*constant);
	constant = new RooStringVar("imageWidth", " # output image width", "1200");
	constants->add(*constant);
	constant = new RooStringVar("imageHeight", " # output image height", "800");
	constants->add(*constant);
	return constants;
}

bool Constants::extendFromFile(RooArgList* constants, std::string filename) {
	std::cout << std::endl << "Reading app constants from file \"" << filename << "\"." << std::endl;
	RooArgSet* constantsFromFile = new RooArgSet();

	std::ifstream constantsFile(filename.c_str());
	if (!constantsFile.is_open()) {
		std::cout << "\"" << filename << "\" not found. Using default values." << std::endl;
		return false;
	}

	std::string line;
	while (std::getline(constantsFile, line)) {
		// Read name and value
		std::istringstream streamLine(line);
		std::string name, value;
		streamLine >> name;
		streamLine >> value;

		// Read description
		// std::size_t pos = line.find("#");
		// std::string description = line.substr(pos);
		RooStringVar* constant = new RooStringVar(name.c_str(), "", value.c_str());
		constantsFromFile->add(*constant);
	}
	constantsFile.close();

	// Iterate default constants and update their values with ones from file
	TIterator* it = constants->createIterator();
	TObject* temp;
	while ((temp = it->Next())) {
		RooStringVar* constant = dynamic_cast<RooStringVar*>(temp);
		if (constant) {
			const char* name = constant->GetName();
			RooStringVar* constantFromFile = (RooStringVar*) constantsFromFile->find(name);
			if (constantFromFile) {
				constant->setVal(constantFromFile->getVal());
			}
		}
	}

	Debug("\"" << filename << "\" found. Successfully read " << constantsFromFile->getSize() << " out of " << constants->getSize() << " values.");
#ifdef USEDEBUG
	constantsFromFile->Print();
#endif
	return true;
}

void Constants::print() {
	std::cout << std::endl << "Application constants in memory" << std::endl;

	std::cout << std::endl;
	TIterator* it = constants->createIterator();
	TObject* temp;
	while ((temp = it->Next())) {
		RooStringVar* constant = dynamic_cast<RooStringVar*>(temp);
		if (constant) {
			std::cout << std::left << std::setw(TAB_SIZE) << constant->GetName() << constant->GetTitle() << std::endl << constant->getVal() << std::endl
					<< std::endl;
		}
	}
}

void Constants::writeToFile(RooArgList* constants, std::string filename) {
	std::cout << std::endl << "Writing app constants to file \"" << filename << "\"." << std::endl;
	std::ofstream myfile;
	myfile.open(filename.c_str());

	TIterator* it = constants->createIterator();
	TObject* temp;
	while ((temp = it->Next())) {
		RooStringVar* constant = dynamic_cast<RooStringVar*>(temp);
		constant->Print();
		if (constant) {
			TString value(constant->getVal());
//			value.ReplaceAll("\\", "\\\\");
			myfile << std::left << std::setw(TAB_SIZE) << constant->GetName() << std::left << std::setw(TAB_SIZE * 2) << value.Data()
					<< constant->GetTitle() << std::endl;
		}
	}
	myfile.close();
}

//int Constants::getNumberOfChannels(){
//    RooStringVar* var = (RooStringVar*) constants->find("channels");
//    return atoi(var->getVal());
//}

double Constants::getDefaultChannelWidth() {
	RooStringVar* var = (RooStringVar*) constants->find("defaultChannelWidth");
	return atof(var->getVal());
}

//RooConstVar* Constants::getRooChannelWidth() {
//	return channelWidth;
//}

int Constants::getSkipLines() {
	RooStringVar* var = (RooStringVar*) constants->find("skipLines");
	return atoi(var->getVal());
}

int Constants::getMinChannel() {
	RooStringVar* var = (RooStringVar*) constants->find("minChannel");
	return atoi(var->getVal());
}

int Constants::getMaxChannel() {
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

const char* Constants::getResolutionFunctionModel() {
	RooStringVar* var = (RooStringVar*) constants->find("resolutionFunction");
	return var->getVal();
}

const char* Constants::getMinimizerType() {
	RooStringVar* var = (RooStringVar*) constants->find("minimizerType");
	return var->getVal();
}

std::vector<std::string> Constants::getDecayModels() {
	RooStringVar* var = (RooStringVar*) constants->find("decayModel");
	return StringUtils::parseString(var->getVal());
}

int Constants::getSourceComponentsNumber() {
	RooStringVar* var = (RooStringVar*) constants->find("sourceComponents");
	return atoi(var->getVal());
}

std::vector<std::string> Constants::getCommonParameters() {
	RooStringVar* var = (RooStringVar*) constants->find("commonParameters");
	return StringUtils::parseString(var->getVal());
}

double Constants::getBufferFraction() {
	RooStringVar* var = (RooStringVar*) constants->find("bufferFraction");
	return atof(var->getVal());
}

int Constants::getConvolutionBins() {
	RooStringVar* var = (RooStringVar*) constants->find("convolutionBins");
	return atoi(var->getVal());
}

int Constants::getBackgroundBins() {
	RooStringVar* var = (RooStringVar*) constants->find("backgroundBins");
	return atoi(var->getVal());
}

std::string Constants::getDelimeter() {
	RooStringVar* var = (RooStringVar*) constants->find("asciiDelimeter");
	std::string delimeter(var->getVal());
	return delimeter;
}

int Constants::getImageWidth() {
	RooStringVar* var = (RooStringVar*) constants->find("imageWidth");
	return atoi(var->getVal());
}

int Constants::getImageHeight() {
	RooStringVar* var = (RooStringVar*) constants->find("imageHeight");
	return atoi(var->getVal());
}
