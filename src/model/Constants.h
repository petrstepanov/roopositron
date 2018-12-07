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

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

class Constants {
public:
    Constants();
    Constants(const Constants& orig);
    virtual ~Constants();
    
//    int getConvolutionBins();
    int getNumberOfChannels();
    double getChannelWidth();
    int getSkipLines();
    int getMinChannel();
    int getMaxChannel();
    int getExcludeMinChannel();
    int getExcludeMaxChannel();    
    const char* getResolutionFunctionModel();    
    std::vector<const char*> getDecayModels();
    std::vector<const char*> getCommonParameters();
//    std::string getSourceContributionModel();
    int getImageWidth();
    int getImageHeight();
    void print();
    bool isReadSuccess();
    
private:
    int         convolutionBins;        // Convolution bins
    int         channels;               // entries in maestro file
    double      channelWidth;           // ns
    int         skipLines;              // Maestro spectrum header size
    int         minChannel;             // fit min channel
    int         maxChannel;             // fit max channel
    int         excludeMinChannel;      // exclude min channel
    int         excludeMaxChannel;      // exclude max channel
    std::string resolutionModel;        // resolution function model `2gauss` or `3gauss`
    std::string decayModel;             // decay model `1exp`, `2exp`, `trapping` or `grain`
    std::string sourceModel;            // 1exp, 2exp (annihilation in air?)
    std::string commonParameters;       // gaussFWHM
    int         imageWidth;
    int         imageHeight;

    bool readConstants();
    void writeConstants();
    
    const std::string filename = "constants.txt";
    bool readSuccess;
};

#endif /* MY_CONSTANTS */

