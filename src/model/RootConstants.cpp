/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   AppConstants.cpp
 * Author: petrstepanov
 * 
 * Created on December 6, 2018, 6:04 AM
 */

#include "RootConstants.h"

RootConstants::RootConstants(){
    fwhm2disp = new RooConstVar("fwhm2disp", "Coefficient to convert fwhm to dispersion", 1./(2.*sqrt(2.*log(2.))));
};

RootConstants* RootConstants::instance = NULL;

RootConstants* RootConstants::getInstance(){
    if (!instance){
        instance = new RootConstants;
    }
    return instance;
}