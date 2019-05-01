 /*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   AdditiveConvolutionPdf.cpp
 * Author: petrstepanov
 * 
 * Created on December 5, 2018, 10:20 AM
 */

#include "AdditiveConvolutionPdf.h"
#include "PdfServer.h"
#include "../util/ObjectNamer.h"
#include "ReverseAddPdf.h"
#include "../util/StringUtils.h"
#include "../util/RootHelper.h"
#include "../util/Debug.h"
#include "../model/Constants.h"
#include "../roofit/providers/ExpProvider.h"
#include "../roofit/providers/ExpSourceProvider.h"
#include "../roofit/providers/TwoExpProvider.h"
#include "../roofit/providers/TwoExpSourceProvider.h"
#include "../roofit/providers/ThreeExpProvider.h"
#include "../roofit/providers/ThreeExpSourceProvider.h"
#include <RooFFTConvPdf.h>
#include <RooAddPdf.h>
#include <RooAbsReal.h>
#include <RooNumIntConfig.h>
#include <RooMsgService.h>
#include <RooWorkspace.h>
#include <RooPolynomial.h>
#include "providers/ExpSourceProvider.h"

AdditiveConvolutionPdf::AdditiveConvolutionPdf(std::vector<std::string> componentIds, const char* resolutionId, int sourceComponents, RooRealVar* observable) {
	Debug("AdditiveConvolutionPdf::AdditiveConvolutionPdf");
	this->observable = observable;
	pdfServer = new PdfServer();

	initComponents(componentIds, sourceComponents);
	initResolutionModel(resolutionId);
	constructModel();
}

AdditiveConvolutionPdf::~AdditiveConvolutionPdf() {
}

void AdditiveConvolutionPdf::initComponents(std::vector<std::string> componentIds, int sourceComponents) {
	Debug("AdditiveConvolutionPdf::initComponents")
	// Build component PDFs
	for (std::vector<std::string>::const_iterator it = componentIds.begin(); it != componentIds.end(); ++it) {
		const char* componentId = ((std::string) *it).c_str();
		RooAbsPdf* pdf = pdfServer->getPdf(componentId, observable);
		componentsList->add(*pdf);
	}
	#ifdef USEDEBUG
		Debug("componentsList")
		componentsList->Print();
	#endif

	// Build source contribution PDF from ExpPdf, replace "tau" parameter name with "tauSource"
	if (sourceComponents <= 3){
		RooAbsPdf* sourceComponent = nullptr;
		switch(sourceComponents) {
		    case 1: {
				ExpSourceProvider* esp = new ExpSourceProvider(observable);
				sourceComponent = esp->getPdf();
				// if one component fix it at 385 ps
//				if (RooRealVar* tauSource = (RooRealVar*) (sourceComponent->getParameters(*observable))->find("#tau")) {
//					tauSource->setConstant(kTRUE);
//					tauSource->setVal(0.385);
//				}
				break;
	        }
		    case 2: {
				TwoExpSourceProvider* tesp = new TwoExpSourceProvider(observable);
				sourceComponent = tesp->getPdf();
				break;
	        }
		    case 3: {
				ThreeExpSourceProvider* tesp = new ThreeExpSourceProvider(observable);
				sourceComponent = tesp->getPdf();
				break;
	        }
		}
		// Rename exponential pdf variables and build source pdf.
		sourceComponentsList->add(*sourceComponent);
//		RooAbsPdf* sourceComponentRenamed = RootHelper::suffixPdfAndNodes(sourceComponent, observable, "source");
//		sourceComponentsList->add(*sourceComponentRenamed);
	}
	// Use sum of ExpPdfs each convoluted with resolution (think its slower)
	else {
		for (unsigned i = 1; i <= sourceComponents; i++) {
			ExpSourceProvider* esp = new ExpSourceProvider(observable);
			RooAbsPdf* sourceComponent = esp->getPdf(i);
			sourceComponentsList->add(*sourceComponent);

			//			RooAbsPdf* sourceComponentRenamed = RootHelper::suffixPdfAndNodes(sourceComponent, observable, "source");
//			sourceComponentsList->add(*sourceComponentRenamed);
		}
	}
	#ifdef USEDEBUG
		Debug("sourceComponentsList");
		sourceComponentsList->Print();
	#endif
}

void AdditiveConvolutionPdf::initResolutionModel(const char* resolutionId) {
	resolutionFunction = pdfServer->getPdf(resolutionId, observable);
}

void AdditiveConvolutionPdf::constructModel() {
	Debug("AdditiveConvolutionPdf::constructModel");
	// Sum theoretical components
	RooAbsPdf* sumComponents = ReverseAddPdf::add(componentsList, observable);

	// Source contribution
	RooRealVar* Int_source = new RooRealVar("Int_source", "Source contribution", 11, 5, 20, "%");
	RooFormulaVar* Int_sourceNorm = new RooFormulaVar("sourceContributionNorm", "@0/100", *Int_source);
	RooAbsPdf* sumSourceComponents = ReverseAddPdf::add(sourceComponentsList, observable, "Source");

	// Flat background
	RooRealVar* background = new RooRealVar("background", "Average background counts", 100, "counts");
	RooRealVar* bins = new RooRealVar("bins", "Histogram bins", 1E3);
	RooRealVar* integral = new RooRealVar("integral", "Full histogram integral", 1E6);
	RooFormulaVar* Int_bg = new RooFormulaVar("Int_bg", "@0*@1/@2", RooArgList(*background, *bins, *integral));
	RooPolynomial* bg = new RooPolynomial("bg", "y=1", *observable, RooArgSet());

	// Add source and sample components together
	modelNonConvoluted = new RooAddPdf("modelNonConvoluted", "Components model with source and background", RooArgList(*bg, *sumSourceComponents, *sumComponents), RooArgList(*Int_bg, *Int_sourceNorm), kTRUE);
	modelNonConvoluted->fixAddCoefNormalization(RooArgSet(*observable));

	// Convolute model
	model = new RooFFTConvPdf("model", "Convoluted model with source contribution", *observable, *modelNonConvoluted, *resolutionFunction);
	Double_t bufferFraction = Constants::getInstance()->getBufferFraction();
	((RooFFTConvPdf*)model)->setBufferFraction(bufferFraction);
}

RooAbsPdf* AdditiveConvolutionPdf::getPdf() {
	return model;
}

RooAbsPdf* AdditiveConvolutionPdf::getResolutionFunction() {
	return resolutionFunction;
}
