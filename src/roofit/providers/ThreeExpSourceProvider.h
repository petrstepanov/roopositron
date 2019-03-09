/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ThreeExpProvider.h
 * Author: petrstepanov
 *
 * Created on December 5, 2018, 8:43 AM
 */

#ifndef THREEEXPSOURCEPROVIDER_H
#define THREEEXPSOURCEPROVIDER_H

#include "AbstractProvider.h"
#include "RooRealVar.h"
#include "RooAbsPdf.h"

class ThreeExpSourceProvider : public AbstractProvider {
public:
	ThreeExpSourceProvider(RooRealVar* observable);
    virtual ~ThreeExpSourceProvider();

    RooAbsPdf* initPdf(int i);
};

#endif /* THREEEXPSOURCEPROVIDER_H */

