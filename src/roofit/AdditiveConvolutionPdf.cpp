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
#include "RooFFTConvPdf.h"

#include "../util/StringUtils.h"
#include "PdfServer.h"
#include "providers/SourceProvider.h"
#include "../util/ObjectNamer.h"
#include "providers/ExpProvider.h"

AdditiveConvolutionPdf::AdditiveConvolutionPdf(std::vector<const char*> componentIds, const char* resolutionId, RooRealVar* observable) {
    componentsNumber = componentIds.size();
    componentsList = initComponents(componentIds, observable);
    resolutionFunction = initResolutionModel(resolutionId, observable);

    initCoefficients();
    convoluteComponents(observable);
    constructModel();
}

void AdditiveConvolutionPdf::initCoefficients(){
    // Initialize recursive coefficients
    // https://app.box.com/s/ddokty1zxp8x0wqoug52e260apx2q5gv
    for (unsigned i = 0; i < componentsNumber - 1; i++){
	// Initialize percent coefficients
	RooRealVar* I = new RooRealVar(TString::Format("I%d", i+1), TString::Format("Component %d fraction", i+1), 100/componentsNumber, 0, 100, "%");
	coefficientsList->add(*I);
	RooFormulaVar* INorm = new RooFormulaVar(TString::Format("I%dNorm", i+1), "@0/100.", *I);
	normalizedCoefficients->add(*INorm);
	RooFormulaVar* IRec = (i==0) ? 
	    new RooFormulaVar(TString::Format("I%dRecursive", i+1), "@0", *INorm):
	    new RooFormulaVar(TString::Format("I%dRecursive", i+1), "@0/@1*@2/(1-@2)", RooArgList(*(coefficientsList->at(i)),*(coefficientsList->at(i-1)),*(normalizedCoefficients->at(i))));
	recursiveCoefficients->add(*IRec);
    }
    
    // Initialize source contribution coefficient
    ISource = new RooRealVar("ISource", "Source contribution", 11, 5, 20, "%");
    ISourceNorm = new RooFormulaVar("ISourceNorm", "@0/100", *ISource);
}

RooAbsPdf* AdditiveConvolutionPdf::initResolutionModel(const char* resolutionId, RooRealVar* observable) {
    return PdfServer::getPdf(resolutionId, observable);
}

RooArgList* AdditiveConvolutionPdf::initComponents(std::vector<const char*> componentIds, RooRealVar* observable) {
    ObjectNamer* pdfNamer = new ObjectNamer();
    ObjectNamer* coefficientsNamer = new ObjectNamer();
    
    // Build component PDFs
    for (std::vector<const char*>::const_iterator it = componentIds.begin(); it != componentIds.end(); ++it){
	std::cout << "AdditiveConvolutionPdf::initComponents" << std::endl;
	std::cout << "Component " << *it << std::endl;
	RooAbsPdf* pdf = PdfServer::getPdf(*it, observable);

	// pdfNamer makes sure there are no two pdfs of a same type with same name (want exp, exp2, exp3)
	pdfNamer->fixUniqueName(pdf);
	// coefficientsNamer makes sure there are no pdf coeficients with same name (want exp, exp2, exp3)
	RooArgSet* params = pdf->getParameters(*observable);
	TIterator* pIt = params->createIterator();
	TObject* tempObj=0;
	while((tempObj=pIt->Next())){
	  TNamed* tNamed = dynamic_cast<TNamed*>(tempObj);
	  if(tNamed){
	    coefficientsNamer->fixUniqueName(tNamed);
	  }
	}
	// Add PDF to the components list
	componentsList->add(*pdf);
    }
    
    // Build source contribution PDF from ExpPdf, replace "tau" parameter name with "tauSource"
    SourceProvider* sp = new SourceProvider(observable);
    sourcePdf = sp->getPdf();

    return NULL;
}

void AdditiveConvolutionPdf::convoluteComponents(RooRealVar *observable){
    // Convolute components
    for (unsigned i = 0; i < componentsNumber - 1; i++){
	RooAbsArg* arg = (componentsList->at(i));
	RooAbsPdf* component = dynamic_cast<RooAbsPdf*>(arg);	
	RooFFTConvPdf* convCompPdf = new RooFFTConvPdf(TString::Format("compConv%d", i + 1), TString::Format("Convoluted component %d", i + 1), *observable, *component, *resolutionFunction);
	convolutedComponentsList->add(*convCompPdf);
    }
    // Convolute source contribution
    convolutedSourcePdf = new RooFFTConvPdf("convolutedSourcePdf", "Convoluted source PDF", *observable, *sourcePdf, *resolutionFunction);
}


void AdditiveConvolutionPdf::constructModel(){
    // Sum components
    RooAddPdf* sumConvolutedComponents = new RooAddPdf("componentsModel", "Components model", *convolutedComponentsList, *recursiveCoefficients, kTRUE);
    // 
    model = new RooAddPdf("componentsSourceModel", "Components model with source", RooArgList(*convolutedSourcePdf,*sumConvolutedComponents), RooArgList(*ISourceNorm));   
}

RooAbsPdf* AdditiveConvolutionPdf::getPdf() {
    return model;
}
