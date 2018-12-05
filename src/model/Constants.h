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
    
    int getConvolutionBins();
//    int getNumCPU();
    int getNumberOfChannels();
    double getChannelWidth();
    int getSkipLines();
    int getMinChannel();
    int getMaxChannel();
    int getExcludeMinChannel();
    int getExcludeMaxChannel();    
    std::string getResolutionFunctionModel();    
    std::string getDecayModel();
    std::string getSourceContributionModel();
    int getImageWidth();
    int getImageHeight();
    void print();
    bool isNew();
    
private:
    int         convolutionBins; // Convolution bins
//    int         numCPU;          // CPU number
    int         channels;        // entries in maestro file
    double      channelWidth;    // ns
    int         skipLines;       // spectrum header size
    int         minChannel;      // left channel
    int         maxChannel;      // right channel
    int         excludeMinChannel;      // left channel
    int         excludeMaxChannel;      // right channel
    std::string resolutionModel; // resolution function model `2gauss` or `3gauss`
    std::string decayModel;      // decay model `1exp`, `2exp`, `trapping` or `grain`
    std::string sourceModel;     // 1exp, 2exp (annihilation in air?)
    int         imageWidth;
    int         imageHeight;
    void init();
    void writeDefaultConstants();
    const std::string filename = "constants.txt";
    bool        isnew;
};

#endif /* MY_CONSTANTS */

