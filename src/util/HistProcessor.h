/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   HistProcessor.h
 * Author: petrstepanov
 *
 * Created on December 8, 2018, 11:35 AM
 */

#ifndef HISTPROCESSOR_H
#define HISTPROCESSOR_H

#include "RtypesCore.h"
#include "TH1F.h"
#include "RooHist.h"

class HistProcessor {
public:
    HistProcessor();
    HistProcessor(const HistProcessor& orig);
    virtual ~HistProcessor();
    
    static Double_t getConstBackgroundFraction(TH1F* hist);
    static Double_t getAverageBackground(TH1F* hist, Int_t lwftWingBins = 150);
    static void setZeroErrors(RooHist* hist);
private:

};

#endif /* HISTPROCESSOR_H */

