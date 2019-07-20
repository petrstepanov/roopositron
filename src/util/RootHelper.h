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
#include <RooArgSet.h>
#include <TMatrixD.h>
#include <RooPlot.h>
#include <TString.h>
#include <TList.h>
#include <utility>

class RootHelper {
public:
//    static void deleteObject(const char* name);
//    static void deleteObject(TObject* obj);
    static Int_t getNumCpu();
    static void printPdfCoefficientNames(RooAbsPdf* pdf, RooRealVar* observable);
//    static RooRealVar* findParameterNameContains(RooAbsPdf* pdf, RooRealVar* observable, const char* substring);
    static RooAbsPdf* suffixPdfAndNodes(RooAbsPdf* pdf, RooRealVar* observable, const char* suffix);
    static void setRooRealVarValueLimits(RooRealVar* var, Double_t value, Double_t min, Double_t max);
    static RooRealVar* getParameterNameContains(RooAbsPdf* pdf, const char* nameSubstring);
    static RooAbsPdf* getComponentNameContains(RooAbsPdf* pdf, const char* nameSubstring);
    static RooRealVar* getParameterByNameCommonOrLocal(RooAbsPdf* pdf, const char* name);
    static RooAbsPdf* getComponentByNameCommonOrLocal(RooAbsPdf* pdf, const char* name);
    static RooAbsArg* findArgNameSubstring(RooAbsCollection* list, const char* nameSubstring);
    static std::pair<TMatrixD,TList*> rooPlotToMatrix(RooRealVar* axis, RooPlot* plot);

private:
    static TStopwatch* watch;
};

#endif /* ROOTUTILS_H */
