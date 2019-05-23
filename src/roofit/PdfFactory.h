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
    kOneGaussPdf,
    kTwoGaussPdf,
    kThreeGaussPdf,
    kExponentPdf,
	kTwoExponentPdf,
	kThreeExponentPdf,
    kTrappingPdf,
    kGrainPdf,
    kPowderPdf,
    kPowder2Pdf,
	kLiquidPdf,
	kLiquidSimplePdf
};

class PdfFactory {
public:   
    PdfFactory();
    virtual ~PdfFactory();
    RooAbsPdf* getPdf(const char* name, RooRealVar* observable, RooRealVar* channelWidth);
//    RooAbsPdf* getPdf(Int_t name, RooRealVar* observable, RooRealVar* channelWidth);

private:
    static PdfHashNames hashPdfName(const char* pdfName);
    int pdfIndexes[32] = {0};
};

#endif /* PDFHASH_H */

