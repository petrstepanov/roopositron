/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   RootUtils.h
 * Author: petrstepanov
 *
 * Created on December 4, 2018, 2:09 PM
 */

#ifndef ROOTUTILS_H
#define ROOTUTILS_H

#include <TROOT.h>
#include <TStopwatch.h>
#include <RooAbsPdf.h>
#include <RooRealVar.h>

class RootHelper {
public:
//    static void deleteObject(const char* name);
//    static void deleteObject(TObject* obj);
    static Int_t getNumCpu();
    static void startTimer(void);
    static void stopAndPrintTimer();
    static void printPdfCoefficientNames(RooAbsPdf* pdf, RooRealVar* observable);
    static RooRealVar* findParameterNameContains(RooAbsPdf* pdf, RooRealVar* observable, const char* substring);
    
private:
    static TStopwatch* watch;
};

#endif /* ROOTUTILS_H */
