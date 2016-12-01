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
    numCPU          = 1;
    channels        = 8192;        // entries in maestro file
    channelWidth    = 0.0061626;   // ns
    skipLines       = 12;          // spectrum header size
    minChannel      = 2100;        // left channel
    maxChannel      = 5500;        // right channel   
    resolutionModel = "2gauss";
    decayModel      = "1exp";
    sourceModel     = "1exp";
    imageWidth      = 1024;
    imageHeight     = 500;
    // Read from file
    std::string line;
    std::ifstream constantsFile(filename.c_str());
    if (!constantsFile.is_open()) {
        std::cout << "Constants: \"" << filename << "\" not found. Using default values." << std::endl;        
        writeDefaultConstants();
        return;
    }

    constantsFile >> numCPU;
    std::getline(constantsFile, line);
    
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

    constantsFile >> resolutionModel;
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
    std::cout << "numCPU:             " << numCPU << std::endl;
    std::cout << "channels:           " << channels << std::endl;
    std::cout << "channel width:      " << channelWidth << std::endl;
    std::cout << "skip lines:         " << skipLines << std::endl;
    std::cout << "minChannel:         " << minChannel << std::endl;
    std::cout << "maxChannel:         " << maxChannel << std::endl;
    std::cout << "resolutionModel:    " << resolutionModel << std::endl;
    std::cout << "decayModel:         " << decayModel << std::endl;
    std::cout << "sourceModel:        " << sourceModel << std::endl;    
    std::cout << "imageWidth:         " << imageWidth << std::endl;   
    std::cout << "imageHeight:        " << imageHeight << std::endl;       
}

void Constants::writeDefaultConstants(){
    std::ofstream myfile;
    myfile.open (filename.c_str());
    myfile << std::left << std::setw(12) << numCPU            << "# number of CPU (careful with \"grain\" model)" << std::endl;
    myfile << std::left << std::setw(12) << channels          << "# number of channels in Maestro .Spe file" << std::endl;
    myfile << std::left << std::setw(12) << channelWidth      << "# channel width, ns" << std::endl;
    myfile << std::left << std::setw(12) << skipLines         << "# Maestro header size" << std::endl;
    myfile << std::left << std::setw(12) << minChannel        << "# minimum channel (>= 1), included in plot" << std::endl;
    myfile << std::left << std::setw(12) << maxChannel        << "# maximum channel, included in plot" << std::endl;
    myfile << std::left << std::setw(12) << resolutionModel   << "# resolution model - \"2gauss\" or \"3gauss\"" << std::endl;
    myfile << std::left << std::setw(12) << decayModel        << "# decay model - \"1exp\", \"2exp\", \"trapping\", \"grain\"" << std::endl;
    myfile << std::left << std::setw(12) << sourceModel       << "# source contribution - \"1exp\", \"2exp\" (annihilation in air?)" << std::endl;    
    myfile << std::left << std::setw(12) << imageWidth        << "# image width" << std::endl;   
    myfile << std::left << std::setw(12) << imageHeight       << "# image height" << std::endl;       
    myfile.close();  
}

int Constants::getNumCPU(){
    return numCPU;
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

std::string Constants::getResolutionFunctionModel(){
    return resolutionModel;
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