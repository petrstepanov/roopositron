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
#include <fstream>
#include <string>

class Constants {
public:
    Constants();
    Constants(const Constants& orig);
    virtual ~Constants();
    
    int getNumberOfChannels();
    double getChannelWidth();
    int getSkipLines();
    int getMinChannel();
    int getMaxChannel();
    std::string getDecayModel();
    std::string getSourceContributionModel();
    int getImageWidth();
    int getImageHeight();
    void print();
    
private:
    int         channels;       // entries in maestro file
    double      channelWidth;   // ns
    int         skipLines;      // spectrum header size
    int         minChannel;     // left channel
    int         maxChannel;     // right channel
    std::string decayModel;     // decay model 1exp, 2exp, 3exp, grain
    std::string sourceModel;    // 1exp, 2exp (annihilation in air?)
    int         imageWidth;
    int         imageHeight;
    void init();
};

#endif /* MY_CONSTANTS */

