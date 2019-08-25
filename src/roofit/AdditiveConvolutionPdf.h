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
#include "RooAbsPdf.h"
#include "RooFormulaVar.h"
#include <vector>
#include "PdfFactory.h"

class AdditiveConvolutionPdf {
public:
    // Constructor accepts two strings: "1exp,1exp,trapping" - coma separated component names and 
    // "2gauss" or "3gauss" - resolution function model
    AdditiveConvolutionPdf(std::vector<std::string> componentIds, const char* resolutionId, int sourceComponents, RooRealVar* observable);
    virtual ~AdditiveConvolutionPdf();

    void constructModel();

    RooAbsPdf* getPdf();

    static const char* VAR_MEAN_GAUSS_NAME;
    static const char* VAR_BINS_NAME;
    static const char* VAR_INTEGRAL_NAME;
    static const char* VAR_INT_SOURCE_NAME;
    static const char* PDF_SOURCE_NAME;
    static const char* PDF_MATERIAL_NAME;
    static const char* VAR_BACKGROUND_COUNT_NAME;
    static const char* PDF_BACKGROUND_NAME;
    static const char* PDF_NON_CONVOLUTED_NAME;
    static const char* PDF_RESOLUTION_FUNCTION_NAME;
    static const char* PDF_CONVOLUTED_NAME;

private:
    void initComponents(std::vector<std::string> componentIds, int sourceComponents);
    void initResolutionModel(const char* resolutionId);

    PdfFactory* pdfFactory;
    RooRealVar* observable;

    RooRealVar* channelWidth;

    RooArgList* componentsList = new RooArgList();
    RooArgList* sourceComponentsList = new RooArgList();
    RooAbsPdf* resolutionFunction;
    
    RooAbsPdf* model;

};

#endif /* ADDITIVECONVOLUTIONPDF_H */

