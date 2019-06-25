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

#include "PdfFactory.h"

#include "providers/OneGaussProvider.h"
#include "providers/TwoGaussProvider.h"
#include "providers/ThreeGaussProvider.h"
#include "providers/ExpProvider.h"
#include "providers/TwoExpProvider.h"
#include "providers/ThreeExpProvider.h"
#include "providers/GrainProvider.h"
#include "providers/LiquidProvider.h"
#include "providers/LiquidSimpleProvider.h"
#include "providers/PowderProvider.h"
#include "providers/Powder2Provider.h"
#include "providers/TrappingProvider.h"

PdfFactory::PdfFactory(){};
PdfFactory::~PdfFactory(){};

PdfHashNames PdfFactory::hashPdfName(const char* pdfName) {
	std::string name = pdfName;
	if (name == "1gauss")
		return kOneGaussPdf;
	if (name == "2gauss")
		return kTwoGaussPdf;
	if (name == "3gauss")
		return kThreeGaussPdf;
	if (name == "exp")
		return kExponentPdf;
	if (name == "2exp")
		return kTwoExponentPdf;
	if (name == "3exp")
		return kThreeExponentPdf;
	if (name == "trapping")
		return kTrappingPdf;
	if (name == "grain")
		return kGrainPdf;
	if (name == "powder")
		return kPowderPdf;
	if (name == "powder2")
		return kPowder2Pdf;
	if (name == "liquid")
		return kLiquidPdf;
	if (name == "liquidsim")
		return kLiquidSimplePdf;
	return kExponentPdf;
}

RooAbsPdf* PdfFactory::getPdf(const char* modelName, RooRealVar* observable, RooRealVar* channelWidth) {
	int hashName = hashPdfName(modelName);
	pdfIndexes[hashName]++;

	switch (hashName) {
		case kOneGaussPdf: {
			OneGaussProvider* ogp = new OneGaussProvider(observable, channelWidth);
			return ogp->getPdf(pdfIndexes[kOneGaussPdf]);
			break;
		}
		case kTwoGaussPdf: {
			TwoGaussProvider* tgp = new TwoGaussProvider(observable, channelWidth);
			return tgp->getPdf(pdfIndexes[kTwoGaussPdf]);
			break;
		}
		case kThreeGaussPdf: {
			ThreeGaussProvider* tgp = new ThreeGaussProvider(observable, channelWidth);
			return tgp->getPdf(pdfIndexes[kThreeGaussPdf]);
			break;
		}
		case kExponentPdf: {
			ExpProvider* ep = new ExpProvider(observable, channelWidth);
			return ep->getPdf(pdfIndexes[kExponentPdf]);
			break;
		}
		case kTwoExponentPdf: {
			TwoExpProvider* tep = new TwoExpProvider(observable, channelWidth);
			return tep->getPdf(pdfIndexes[kTwoExponentPdf]);
			break;
		}
		case kThreeExponentPdf: {
			ThreeExpProvider* thep = new ThreeExpProvider(observable, channelWidth);
			return thep->getPdf(pdfIndexes[kThreeExponentPdf]);
			break;
		}
		case kTrappingPdf: {
			TrappingProvider* tp = new TrappingProvider(observable, channelWidth);
			return tp->getPdf(pdfIndexes[kTrappingPdf]);
			break;
		}
		case kGrainPdf: {
			GrainProvider* gp = new GrainProvider(observable, channelWidth);
			return gp->getPdf(pdfIndexes[kGrainPdf]);
			break;
		}
		case kPowderPdf: {
			PowderProvider* pp = new PowderProvider(observable, channelWidth);
			return pp->getPdf(pdfIndexes[kPowderPdf]);
			break;
		}
		case kPowder2Pdf: {
			Powder2Provider* pp = new Powder2Provider(observable, channelWidth);
			return pp->getPdf(pdfIndexes[kPowder2Pdf]);
			break;
		}
		case kLiquidPdf: {
			LiquidProvider* lp = new LiquidProvider(observable, channelWidth);
			return lp->getPdf(pdfIndexes[kLiquidPdf]);
			break;
		}
		case kLiquidSimplePdf: {
			LiquidSimpleProvider* pp = new LiquidSimpleProvider(observable, channelWidth);
			return pp->getPdf(pdfIndexes[kLiquidSimplePdf]);
			break;
		}
		default: {
			return NULL;
		}
	}
}

// TODO: make methods in Provider classes static, remove constructors?
RooArgSet* PdfFactory::getIndirectParameters(const char* modelName, RooAbsPdf* pdf) {
	int hashName = hashPdfName(modelName);

	switch (hashName) {
		case kOneGaussPdf: {
			break;
		}
		case kTwoGaussPdf: {
			RooArgSet* indirectParameters = TwoGaussProvider::getIndirectParameters(pdf);
			return indirectParameters;
			break;
		}
		case kThreeGaussPdf: {
			RooArgSet* indirectParameters = ThreeGaussProvider::getIndirectParameters(pdf);
			return indirectParameters;
			break;
		}
		case kExponentPdf: {
			break;
		}
		case kTwoExponentPdf: {
			RooArgSet* indirectParameters = TwoExpProvider::getIndirectParameters(pdf);
			return indirectParameters;
			break;
		}
		case kThreeExponentPdf: {
			RooArgSet* indirectParameters = ThreeExpProvider::getIndirectParameters(pdf);
			return indirectParameters;
			break;
		}
		case kTrappingPdf: {
			break;
		}
		case kGrainPdf: {
			break;
		}
		case kPowderPdf: {
			break;
		}
		case kPowder2Pdf: {
			break;
		}
		case kLiquidPdf: {
			RooArgSet* indirectParameters = LiquidProvider::getIndirectParameters(pdf);
			return indirectParameters;
			break;
		}
		case kLiquidSimplePdf: {
			break;
		}
//		default: {
//			return NULL;
//		}
	}
	return new RooArgSet();
}

