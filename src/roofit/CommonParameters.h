/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CommonParameters.h
 * Author: petrstepanov
 *
 * Created on December 6, 2018, 2:42 PM
 */

#ifndef COMMONPARAMETERS_H
#define COMMONPARAMETERS_H

#include "RooArgSet.h"

class CommonParameters {
public:
    CommonParameters();
    CommonParameters(const CommonParameters& orig);
    virtual ~CommonParameters();
private:
    RooArgSet* commonParameters;
};

#endif /* COMMONPARAMETERS_H */

