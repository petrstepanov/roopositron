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
    AdditiveConvolutionPdf(std::vector<std::string> componentIds, const char* resolutionId, RooRealVar* observable);
    virtual ~AdditiveConvolutionPdf();

    void constructModel();
    void convoluteComponents(RooRealVar* observable);
    void initCoefficients();

    RooAbsPdf* getPdf();
    RooAbsPdf* getResolutionFunction();
    RooArgList* getAllComponents();
    RooArgList* getConvolutedComponents();
    RooAbsPdf* getSourceCompoment();
    RooAbsPdf* getConvolutedSourceComponent();
    
private:
    void initComponents(std::vector<std::string> componentIds, RooRealVar* observable);
    void initResolutionModel(const char* resolutionId, RooRealVar* observable);

    PdfServer* pdfServer;
    
    RooArgList* componentsList = new RooArgList();
    RooAbsPdf* resolutionFunction; 
    
    RooArgList* coefficientsList = new RooArgList(); // in percent
    RooArgList* normalizedCoefficients = new RooArgList(); // in [0, 1]
    RooArgList* recursiveCoefficients = new RooArgList();
    
    RooArgList* convolutedComponentsList = new RooArgList();
    RooAbsPdf* model;

    RooAbsPdf* sourcePdf;
    RooAbsPdf* convolutedSourcePdf;
    
    // Source contribution coefficients
    RooRealVar* ISource;
    RooFormulaVar* ISourceNorm;
    
    Int_t componentsNumber;
};

#endif /* ADDITIVECONVOLUTIONPDF_H */

