/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   AdditiveConvolutionPdf.h
 * Author: petrstepanov
 *
 * Created on December 5, 2018, 10:20 AM
 */

#ifndef ADDITIVECONVOLUTIONPDF_H
#define ADDITIVECONVOLUTIONPDF_H

#include "../model/ParametersPool.h"
#include "../roofit/PdfServer.h"
#include "RooAbsPdf.h"
#include "RooFormulaVar.h"
#include <vector>

class AdditiveConvolutionPdf {
public:
    // Constructor accepts two strings: "1exp,1exp,trapping" - coma separated component names and 
    // "2gauss" or "3gauss" - resolution function model
    AdditiveConvolutionPdf(std::vector<std::string> componentIds, const char* resolutionId, int sourceComponents, RooRealVar* observable);
    virtual ~AdditiveConvolutionPdf();

    void constructModel();

    RooAbsPdf* getPdf();
    RooAbsPdf* getResolutionFunction();
    
private:
    void initComponents(std::vector<std::string> componentIds, int sourceComponents);
    void initResolutionModel(const char* resolutionId);

    PdfServer* pdfServer;
    RooRealVar* observable;

    RooArgList* componentsList = new RooArgList();
    RooArgList* sourceComponentsList = new RooArgList();
    RooAbsPdf* resolutionFunction; 
    
    RooAbsPdf* modelNonConvoluted;
    RooAbsPdf* model;

};

#endif /* ADDITIVECONVOLUTIONPDF_H */

