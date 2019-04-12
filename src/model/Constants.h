/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Constants.h
 * Author: petrstepanov
 *
 * Created on November 9, 2016, 8:07 PM
 */

#ifndef MY_CONSTANTS
#define MY_CONSTANTS

#include "RooConstVar.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

class Constants {
public:
    virtual ~Constants();
    
//    int getNumberOfChannels();
    double getChannelWidth();
    RooConstVar* getRooChannelWidth();
    int getSkipLines();
    int getMinChannel();
    int getMaxChannel();
    int getExcludeMinChannel();
    int getExcludeMaxChannel();    
    const char* getResolutionFunctionModel();
    const char* getMinimizerType();
    int getSourceComponentsNumber();
    std::vector<std::string> getDecayModels();
    std::vector<std::string> getCommonParameters();
    double getBufferFraction();
    int getConvolutionBins();
    int getImageWidth();
    int getImageHeight();
    void print();

    static RooConstVar* fwhm2disp;
    static Constants* getInstance();
    
private:
    Constants();
    static Constants* instance;

    RooArgList* constants;
    RooArgList* initDefaultValues();
    bool extendFromFile(RooArgList* constants, std::string filename);
    void writeToFile(RooArgList* constants, std::string filename);
    
    RooConstVar* channelWidth;
    const std::string DEFAULT_FILENAME = "constants.txt";
    const int TAB_SIZE = 20; 
};

#endif /* MY_CONSTANTS */

