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
    imageWidth    = 1024;
    imageHeight   = 500;
    // Read from file
    std::string line;
    std::ifstream constantsFile("constants.txt");
    if (!constantsFile.is_open()) {
        std::cout << "Constants: \"constants.txt\" not found. Using default values." << std::endl;        
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