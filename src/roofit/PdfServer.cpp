/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PdfHash.cpp
 * Author: petrstepanov
 * 
 * Created on December 6, 2018, 4:53 AM
 */

#include "PdfServer.h"
#include "providers/TwoGaussProvider.h"
#include "providers/ThreeGaussProvider.h"
#include "providers/ExpProvider.h"
#include "providers/GrainProvider.h"
#include "providers/TrappingProvider.h"

PdfHashNames PdfServer::hashPdfName(const char* pdfName) {
    std::string name = pdfName;
    if (name == "2gauss") return kTwoGaussPdf;
    if (name == "3gauss") return kThreeGaussPdf;	
    if (name == "1exp") return kOneExponentPdf;
    if (name == "trapping") return kTrappingPdf;
    if (name == "grain") return kGrainPdf;
//    if (name == "2exp") return kTwoExponentPdf;
    return kOneExponentPdf;
}

RooAbsPdf* PdfServer::getPdf(const char* name, RooRealVar* observable) {
    switch (hashPdfName(name)) {
	case kTwoGaussPdf:
	    {
		TwoGaussProvider* tgp = new TwoGaussProvider(observable);
		return tgp->getPdf();
	    }
	    break;
	case kThreeGaussPdf:
	    {
		ThreeGaussProvider* tgp = new ThreeGaussProvider(observable);
		return tgp->getPdf();
	    }
	    break;
	case kOneExponentPdf:
	    {
		ExpProvider* ep = new ExpProvider(observable);
		return ep->getPdf();
	    }
	case kTrappingPdf:
	    {
		TrappingProvider* tp = new TrappingProvider(observable);
		return tp->getPdf();
	    }	
	    break;
	case kGrainPdf:
	    {
		GrainProvider* gp = new GrainProvider(observable);
		return gp->getPdf();
	    }	
	    break;	    
	default: 
	    return NULL;
    }
}

