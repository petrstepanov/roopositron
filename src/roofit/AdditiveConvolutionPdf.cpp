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
#include "../roofit/providers/TwoExpProvider.h"
#include "../roofit/providers/ThreeExpProvider.h"
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
	initCoefficients();
//	convoluteComponents();
	constructModel();
	addBackground();
}

AdditiveConvolutionPdf::~AdditiveConvolutionPdf() {
}

void AdditiveConvolutionPdf::initComponents(std::vector<std::string> componentIds, int sourceComponents) {
	// Build component PDFs
	for (std::vector<std::string>::const_iterator it = componentIds.begin(); it != componentIds.end(); ++it) {
		const char* componentId = ((std::string) *it).c_str();
		RooAbsPdf* pdf = pdfServer->getPdf(componentId, observable);
		componentsList->add(*pdf);
	}
	#ifdef USEDEBUG
		Debug("AdditiveConvolutionPdf::initComponents", "componentsList")
		componentsList->Print();
	#endif

	// Build source contribution PDF from ExpPdf, replace "tau" parameter name with "tauSource"
	if (sourceComponents <= 3){
		RooAbsPdf* sourceComponent = nullptr;
		switch(sourceComponents) {
		    case 1: {
				ExpProvider* ep = new ExpProvider(observable);
				sourceComponent = ep->getPdf();
				// if one component fix it at 385 ps
				if (RooRealVar* tauSource = (RooRealVar*) (sourceComponent->getParameters(*observable))->find("#tau")) {
					tauSource->setConstant(kTRUE);
					tauSource->setVal(0.385);
				}
				break;
	        }
		    case 2: {
				TwoExpProvider* tep = new TwoExpProvider(observable);
				sourceComponent = tep->getPdf();
				break;
	        }
		    case 3: {
				ThreeExpProvider* tep = new ThreeExpProvider(observable);
				sourceComponent = tep->getPdf();
				break;
	        }
		}
		// Rename exponential pdf variables and build source pdf.
		RooAbsPdf* sourceComponentRenamed = RootHelper::suffixPdfAndNodes(sourceComponent, observable, "source");
		sourceComponentsList->add(*sourceComponentRenamed);
	}
	// Use sum of ExpPdfs each convoluted with resolution (think its slower)
	else {
		for (unsigned i = 1; i <= sourceComponents; i++) {
			ExpProvider* sp = new ExpProvider(observable);
			RooAbsPdf* sourceComponent = sp->getPdf(i);
			RooAbsPdf* sourceComponentRenamed = RootHelper::suffixPdfAndNodes(sourceComponent, observable, "source");
			sourceComponentsList->add(*sourceComponentRenamed);
		}
	}
	#ifdef USEDEBUG
		Debug("AdditiveConvolutionPdf::initComponents", "sourceComponentsList");
		sourceComponentsList->Print();
	#endif
}

void AdditiveConvolutionPdf::initResolutionModel(const char* resolutionId) {
	// std::cout << std::endl << "AdditiveConvolutionPdf::initResolutionModel" << std::endl;
	resolutionFunction = pdfServer->getPdf(resolutionId, observable);
	// resolutionFunction->Print();
}

void AdditiveConvolutionPdf::initCoefficients() {
	// Initialize source contribution coefficient
	sourceContribution = new RooRealVar("Int_source", "Source contribution", 11, 5, 20, "%");
	sourceContributionNorm = new RooFormulaVar("sourceContributionNorm", "@0/100", *sourceContribution);
}

void AdditiveConvolutionPdf::convoluteComponents() {
//
//	// Convolute components
//	double bufferFraction = Constants::getInstance()->getBufferFraction();
//	for (unsigned i = 0; i < componentsList->getSize(); i++) {
//		RooAbsPdf* component = dynamic_cast<RooAbsPdf*>(componentsList->at(i));
//		const char* componentName = component->GetName();
//		const char* componentTitle = component->GetTitle();
//		RooFFTConvPdf* pdf = new RooFFTConvPdf(Form("%sConv", componentName), Form("%s convoluted", componentTitle), *observable, *component, *resolutionFunction);
//		pdf->setCacheObservables(RooArgSet(*observable));
//		pdf->setBufferFraction(bufferFraction);
//		convolutedComponentsList->add(*pdf);
//	}
//	Debug("AdditiveConvolutionPdf::convoluteComponents", "convolutedComponentsList");
//	#ifdef USEDEBUG
//		convolutedComponentsList->Print();
//	#endif
//
//	// Convolute source components
//	for (unsigned i = 0; i < sourceComponentsList->getSize(); i++) {
//		RooAbsPdf* component = dynamic_cast<RooAbsPdf*>(sourceComponentsList->at(i));
//		const char* componentName = component->GetName();
//		const char* componentTitle = component->GetTitle();
//		RooFFTConvPdf* pdf = new RooFFTConvPdf(Form("%sConv", componentName), Form("%s convoluted", componentTitle), *observable, *component, *resolutionFunction);
//		pdf->setCacheObservables(RooArgSet(*observable));
//		pdf->setBufferFraction(bufferFraction); // because sometimes (on some ranges) convolution produced PDF with weird peak
//		convolutedSourceComponentsList->add(*pdf);
//	}
//	Debug("AdditiveConvolutionPdf::convoluteComponents", "convolutedSourceComponentsList");
//	#ifdef USEDEBUG
//		convolutedSourceComponentsList->Print();
//	#endif
}

void AdditiveConvolutionPdf::constructModel() {
	// std::cout << std::endl << "AdditiveConvolutionPdf::constructModel" << std::endl;
	// Sum components
	RooAbsPdf* sumComponents = ReverseAddPdf::add(componentsList, observable);
	RooAbsPdf* sumSourceComponents = ReverseAddPdf::add(sourceComponentsList, observable, "Source");

	modelNonConvoluted = new RooAddPdf("modelNonConvoluted", "Components model with source", RooArgList(*sumSourceComponents, *sumComponents), RooArgList(*sourceContributionNorm));
	modelNonConvoluted->fixAddCoefNormalization(RooArgSet(*observable));

	model = new RooFFTConvPdf("model", "Convoluted model with source contribution", *observable, *modelNonConvoluted, *resolutionFunction);
	((RooFFTConvPdf*)model)->setCacheObservables(RooArgSet(*observable));
	double bufferFraction = Constants::getInstance()->getBufferFraction();
	// On some ranges convolution produced PDF with weird peak
	((RooFFTConvPdf*)model)->setBufferFraction(bufferFraction);
}

void AdditiveConvolutionPdf::addBackground() {
	// Initialize background here
	RooPolynomial* bg = new RooPolynomial("bg", "y=1", *observable, RooArgSet());

	// Parameterize background as counts, (use arbitrary values here)
	RooRealVar* avgBgCount = new RooRealVar("background", "Background level counts", 100, "counts");
	RooRealVar* bins = new RooRealVar("bins", "Histogram bins", 1E3);
	RooRealVar* fullIntegral = new RooRealVar("integral", "Full histogram integral", 1E6);
	RooFormulaVar* intBg = new RooFormulaVar("intBg", "@0*@1/@2", RooArgList(*avgBgCount, *bins, *fullIntegral));

	// Sum convoluted model with background
	modelWithBg = new RooAddPdf("modelWithBg", "Convoluted model with background", RooArgList(*bg, *model), *intBg);
}

RooAbsPdf* AdditiveConvolutionPdf::getPdf() {
	return modelWithBg;
}

RooAbsPdf* AdditiveConvolutionPdf::getResolutionFunction() {
	return resolutionFunction;
}

RooArgList* AdditiveConvolutionPdf::getAllComponents() {
	return componentsList;
}

RooArgList* AdditiveConvolutionPdf::getConvolutedComponents() {
	return convolutedComponentsList;
}

RooAbsPdf* AdditiveConvolutionPdf::getSourceCompoment() {
	return sourcePdf;
}

RooAbsPdf* AdditiveConvolutionPdf::getConvolutedSourceComponent() {
	return convolutedSourcePdf;
}
