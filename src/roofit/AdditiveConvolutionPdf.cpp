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
#include "PdfFactory.h"

const char* AdditiveConvolutionPdf::VAR_CHANNEL_WIDTH_NAME = "channelWidth";
const char* AdditiveConvolutionPdf::VAR_MEAN_GAUSS_NAME = "meanGauss";
const char* AdditiveConvolutionPdf::VAR_BINS_NAME = "bins";
const char* AdditiveConvolutionPdf::VAR_INTEGRAL_NAME = "integral";
const char* AdditiveConvolutionPdf::VAR_INT_SOURCE_NAME = "Int_source";
const char* AdditiveConvolutionPdf::PDF_SOURCE_NAME = "sourcePdf";
const char* AdditiveConvolutionPdf::PDF_MATERIAL_NAME = "materialPdf";

const char* AdditiveConvolutionPdf::VAR_BACKGROUND_COUNT_NAME = "background";
const char* AdditiveConvolutionPdf::PDF_BACKGROUND_NAME = "backgroundPdf";

const char* AdditiveConvolutionPdf::PDF_NON_CONVOLUTED_NAME = "modelNonConvolutedPdf";
const char* AdditiveConvolutionPdf::PDF_RESOLUTION_FUNCTION_NAME = "resolutionPdf";
const char* AdditiveConvolutionPdf::PDF_CONVOLUTED_NAME = "modelConvolutedPdf";

AdditiveConvolutionPdf::AdditiveConvolutionPdf(std::vector<std::string> componentIds, const char* resolutionId, int sourceComponents, RooRealVar* observable) {
	Debug("AdditiveConvolutionPdf::AdditiveConvolutionPdf");
	this->observable = observable;

	Double_t channelWidthValue = Constants::getInstance()->getDefaultChannelWidth();
	this->channelWidth = new RooRealVar(VAR_CHANNEL_WIDTH_NAME, "Width of channel", channelWidthValue, 0, 0.1, "ns");
	this->channelWidth->setConstant(kTRUE);

	pdfFactory = new PdfFactory();
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
		RooAbsPdf* pdf = pdfFactory->getPdf(componentId, observable, channelWidth);
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
				ExpSourceProvider* esp = new ExpSourceProvider(observable, channelWidth);
				sourceComponent = esp->getPdf();
				// if one component fix it at 385 ps
//				if (RooRealVar* tauSource = (RooRealVar*) (sourceComponent->getParameters(*observable))->find("#tau")) {
//					tauSource->setConstant(kTRUE);
//					tauSource->setVal(0.385);
//				}
				break;
	        }
		    case 2: {
				TwoExpSourceProvider* tesp = new TwoExpSourceProvider(observable, channelWidth);
				sourceComponent = tesp->getPdf();
				break;
	        }
		    case 3: {
				ThreeExpSourceProvider* tesp = new ThreeExpSourceProvider(observable, channelWidth);
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
			ExpSourceProvider* esp = new ExpSourceProvider(observable, channelWidth);
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
	resolutionFunction = pdfFactory->getPdf(resolutionId, observable, channelWidth);
	resolutionFunction->SetName(PDF_RESOLUTION_FUNCTION_NAME);
}

void AdditiveConvolutionPdf::constructModel() {
	// Sum theoretical components
	TIterator* it = componentsList->createIterator();
	while(TObject* object = it->Next()){
		if (RooAbsPdf* pdf = dynamic_cast<RooAbsPdf*>(object)){
			pdf->setAttribute("drawOnRooPlot", kTRUE);
		}
	}
	RooAbsPdf* sumMaterialComponents = ReverseAddPdf::add(componentsList, observable, PDF_MATERIAL_NAME);

	// Source contribution
	RooRealVar* Int_source = new RooRealVar(VAR_INT_SOURCE_NAME, "Source contribution", 11, 5, 20, "%");
	RooFormulaVar* Int_sourceNorm = new RooFormulaVar("Int_sourceNorm", "@0/100", *Int_source);
	RooAbsPdf* sumSourceComponents = ReverseAddPdf::add(sourceComponentsList, observable);
	sumSourceComponents->SetName(PDF_SOURCE_NAME);
	sumSourceComponents->SetTitle("Source contribution PDF");
	sumSourceComponents->setAttribute("drawOnRooPlot", kTRUE);

	// Flat background
	RooRealVar* background = new RooRealVar(VAR_BACKGROUND_COUNT_NAME, "Average background counts", 100, "counts");
	RooRealVar* bins = new RooRealVar(VAR_BINS_NAME, "Histogram bins", 1E3);
	RooRealVar* integral = new RooRealVar(VAR_INTEGRAL_NAME, "Full histogram integral", 1E6);
	RooFormulaVar* Int_bgNorm = new RooFormulaVar("Int_bgNorm", "@0*@1/@2", RooArgList(*background, *bins, *integral));
	RooPolynomial* bg = new RooPolynomial(PDF_BACKGROUND_NAME, "Background", *observable, RooArgSet());
	bg->setAttribute("drawOnRooPlot", kTRUE);

	// Add source and sample components together
	RooAbsPdf* modelNonConvoluted = new RooAddPdf(PDF_NON_CONVOLUTED_NAME, "Components model with source and background", RooArgList(*bg, *sumSourceComponents, *sumMaterialComponents), RooArgList(*Int_bgNorm, *Int_sourceNorm), kTRUE);
	modelNonConvoluted->fixAddCoefNormalization(RooArgSet(*observable));

	// Convolute model
	model = new RooFFTConvPdf(PDF_CONVOLUTED_NAME, "Convoluted model with source contribution", *observable, *modelNonConvoluted, *resolutionFunction);
	Double_t bufferFraction = Constants::getInstance()->getBufferFraction();
	((RooFFTConvPdf*)model)->setBufferFraction(bufferFraction);
}

RooAbsPdf* AdditiveConvolutionPdf::getPdf() {
	return model;
}
