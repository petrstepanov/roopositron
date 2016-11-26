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

Constants::Constants() {
    this->init();
    this->print();
}

Constants::Constants(const Constants& orig) {
}

Constants::~Constants() {
}

void Constants::init(){
    // Default values
    channels      = 8192;        // entries in maestro file
    channelWidth  = 0.0061626;   // ns
    skipLines     = 12;          // spectrum header size
    minChannel    = 2200;        // left channel
    maxChannel    = 3500;        // right channel   
    decayModel    = "1exp";
    sourceModel   = "1exp";
    imageWidth    = 1024;
    imageHeight   = 500;
    // Read from file
    std::string line;
    std::ifstream constantsFile(filename.c_str());
    if (!constantsFile.is_open()) {
        std::cout << "Constants: \"" << filename << "\" not found. Using default values." << std::endl;        
        writeDefaultConstants();
        return;
    }

    constantsFile >> channels;
    std::getline(constantsFile, line);

    constantsFile >> channelWidth;
    std::getline(constantsFile, line);

    constantsFile >> skipLines;
    std::getline(constantsFile, line);

    constantsFile >> minChannel;
    std::getline(constantsFile, line);

    constantsFile >> maxChannel;
    std::getline(constantsFile, line);

    constantsFile >> decayModel;
    std::getline(constantsFile, line);

    constantsFile >> sourceModel;
    std::getline(constantsFile, line);

    constantsFile >> imageWidth;
    std::getline(constantsFile, line);
    
    constantsFile >> imageHeight;
    std::getline(constantsFile, line);
    
    constantsFile.close();
    
    std::cout << "Constants: constants read successfully." << std::endl;
}

void Constants::print(){
    std::cout << "channels:      " << channels << std::endl;
    std::cout << "channel width: " << channelWidth << std::endl;
    std::cout << "skip lines:    " << skipLines << std::endl;
    std::cout << "minChannel:    " << minChannel << std::endl;
    std::cout << "maxChannel:    " << maxChannel << std::endl;
    std::cout << "decayModel:    " << decayModel << std::endl;
    std::cout << "sourceModel:   " << sourceModel << std::endl;    
    std::cout << "imageWidth:    " << imageWidth << std::endl;   
    std::cout << "imageHeight:   " << imageHeight << std::endl;       
}

void Constants::writeDefaultConstants(){
    std::ofstream myfile;
    myfile.open (filename.c_str());
    myfile << "Writing default constants file.\n";
    myfile.close();    
    std::cout << "channels:      " << std::setw(12) << channels     << "# number of channels in Maestro .Spe file" << std::endl;
    std::cout << "channel width: " << std::setw(12) << channelWidth << "# channel width, ns" << std::endl;
    std::cout << "skip lines:    " << std::setw(12) << skipLines    << "# Maestro header size" << std::endl;
    std::cout << "minChannel:    " << std::setw(12) << minChannel   << "# minimum channel (>= 1), included in plot" << std::endl;
    std::cout << "maxChannel:    " << std::setw(12) << maxChannel   << "# maximum channel, included in plot" << std::endl;
    std::cout << "decayModel:    " << std::setw(12) << decayModel   << "# decay model - \"1exp\", \"2exp\", \"trapping\", \"grain\"" << std::endl;
    std::cout << "sourceModel:   " << std::setw(12) << sourceModel  << "# source contribution - \"1exp\", \"2exp\" (annihilation in air?)" << std::endl;    
    std::cout << "imageWidth:    " << std::setw(12) << imageWidth   << "# image width" << std::endl;   
    std::cout << "imageHeight:   " << std::setw(12) << imageHeight  << "# image height" << std::endl;       
}

int Constants::getNumberOfChannels(){
    return channels;
}

double Constants::getChannelWidth(){
    return channelWidth;
}

int Constants::getSkipLines(){
    return skipLines;
}

int Constants::getMinChannel(){
    return minChannel;
}

int Constants::getMaxChannel(){
    return maxChannel;
}

std::string Constants::getDecayModel(){
    return decayModel;
}

std::string Constants::getSourceContributionModel(){
    return sourceModel;
}

int Constants::getImageWidth(){
    return imageWidth;
}

int Constants::getImageHeight(){
    return imageHeight;
}