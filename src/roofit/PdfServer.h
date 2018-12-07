/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PdfHash.h
 * Author: petrstepanov
 *
 * Created on December 6, 2018, 4:53 AM
 */

#ifndef PDFHASH_H
#define PDFHASH_H

#include "RooAbsPdf.h"

enum PdfHashNames {
    kTwoGaussPdf,
    kThreeGaussPdf,
    kOneExponentPdf,
    kTrappingPdf,
    kGrainPdf
};

class PdfServer {
public:
    static RooAbsPdf* getPdf(const char* name, RooRealVar* observable);
private:
    static PdfHashNames hashPdfName(const char* pdfName);
};

#endif /* PDFHASH_H */

