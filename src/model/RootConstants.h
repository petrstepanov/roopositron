/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   AppConstants.h
 * Author: petrstepanov
 *
 * Created on December 6, 2018, 6:04 AM
 */

#ifndef ROOTCONSTANTS_H
#define ROOTCONSTANTS_H

#include "RooConstVar.h"

class RootConstants {
public:
    RooConstVar* fwhm2disp;
    RooConstVar* channelWidth;
    static RootConstants* getInstance();

private: 
    RootConstants();
    static RootConstants* instance;    
};

#endif /* ROOTCONSTANTS_H */

