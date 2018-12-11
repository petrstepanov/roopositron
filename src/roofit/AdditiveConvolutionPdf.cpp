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
#include "providers/SourceProvider.h"
#include <RooFFTConvPdf.h>
#include <RooAddPdf.h>

AdditiveConvolutionPdf::AdditiveConvolutionPdf(std::vector<std::string> componentIds, const char* resolutionId, RooRealVar* observable) {
    pdfServer = new PdfServer();
    componentsNumber = componentIds.size();
    initComponents(componentIds, observable);
    initResolutionModel(resolutionId, observable);
    initCoefficients();
    convoluteComponents(observable);
    constructModel();
}

AdditiveConvolutionPdf::~AdditiveConvolutionPdf() {}

void AdditiveConvolutionPdf::initComponents(std::vector<std::string> componentIds, RooRealVar* observable) {
//    std::cout << std::endl << "AdditiveConvolutionPdf::initComponents" << std::endl;
//    std::cout << "Found " << componentsNumber << " components" << std::endl;
//    ObjectNamer* pdfNamer = new ObjectNamer();
//    ObjectNamer* coefficientsNamer = new ObjectNamer();

//    ObjectNamer* pdfAndCoeficientsNamer = new ObjectNamer();

    // Build component PDFs
    for (std::vector<std::string>::const_iterator it = componentIds.begin(); it != componentIds.end(); ++it){
//	std::cout << "Component " << *it << std::endl;
	RooAbsPdf* pdf = pdfServer->getPdf(((std::string)*it).c_str(), observable);
	// pdfNamer makes sure there are no two pdfs of a same type with same name (want exp, exp2, exp3)
//	pdfNamer->fixUniqueName(pdf);

//	pdfAndCoeficientsNamer->fixUniquePdfAndParameterNames(pdf, observable);

	// coefficientsNamer makes sure there are no pdf coeficients with same name (want exp, exp2, exp3)
//	RooArgSet* params = pdf->getParameters(*observable);
//	TIterator* it2 = params->createIterator();
//	TObject* temp;
//	while((temp = it2->Next())){
//	    TNamed* named = dynamic_cast<TNamed*>(temp);
//	    if(named){
//		coefficientsNamer->fixUniqueName(named);
//	    }
//	}
//	pdf->Print();
	// Add PDF to the components list
	componentsList->add(*pdf);
    }
//    componentsList->Print();
    
    // Build source contribution PDF from ExpPdf, replace "tau" parameter name with "tauSource"
    SourceProvider* sp = new SourceProvider(observable);
    sourcePdf = sp->getPdf();
//    sourcePdf->Print();    
}

void AdditiveConvolutionPdf::initResolutionModel(const char* resolutionId, RooRealVar* observable) {
//    std::cout << std::endl << "AdditiveConvolutionPdf::initResolutionModel" << std::endl;
    resolutionFunction = pdfServer->getPdf(resolutionId, observable);
//    resolutionFunction->Print();    
}

void AdditiveConvolutionPdf::initCoefficients(){
//    std::cout << std::endl << "AdditiveConvolutionPdf::initCoefficients" << std::endl;
    // Initialize recursive coefficients
    // https://app.box.com/s/ddokty1zxp8x0wqoug52e260apx2q5gv
    for (unsigned i = 0; i < componentsNumber-1; i++){
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
//    recursiveCoefficients->Print();
    
    // Initialize source contribution coefficient
    ISource = new RooRealVar("ISource", "Source contribution", 11, 5, 20, "%");
    ISourceNorm = new RooFormulaVar("ISourceNorm", "@0/100", *ISource);
}

void AdditiveConvolutionPdf::convoluteComponents(RooRealVar *observable){
    // Convolute components
    for (unsigned i = 0; i < componentsNumber; i++){
	RooAbsArg* arg = (componentsList->at(i));
	RooAbsPdf* component = dynamic_cast<RooAbsPdf*>(arg);
	RooFFTConvPdf* convCompPdf = new RooFFTConvPdf(TString::Format("%dconvolutedComponent", i), TString::Format("%d Convoluted component", i), *observable, *component, *resolutionFunction);
	convCompPdf->setBufferFraction(0.2);
	convolutedComponentsList->add(*convCompPdf);
    }
    convolutedComponentsList->Print();
    
    // Convolute source contribution
    convolutedSourcePdf = new RooFFTConvPdf("convolutedSourcePdf", "Convoluted source PDF", *observable, *sourcePdf, *resolutionFunction);
    ((RooFFTConvPdf*)convolutedSourcePdf)->setBufferFraction(0.2);
}

void AdditiveConvolutionPdf::constructModel(){
//    std::cout << std::endl << "AdditiveConvolutionPdf::constructModel" << std::endl;
    // Sum components
    RooAbsPdf* sumConvolutedComponents;
    if (componentsNumber == 1){
	RooAbsArg* temp = convolutedComponentsList->at(0);
	RooAbsPdf* pdf = dynamic_cast<RooAbsPdf*>(temp);
	if (pdf){
	    sumConvolutedComponents = pdf;
    	}
    } else {
//	sumConvolutedComponents = new RooAddPdf("componentsModel", "Components model", *convolutedComponentsList, *recursiveCoefficients, kTRUE);	
	sumConvolutedComponents = new RooAddPdf("componentsModel", "Components model", *convolutedComponentsList, *normalizedCoefficients, kTRUE);	
    }

    model = new RooAddPdf("componentsSourceModel", "Components model with source", RooArgList(*convolutedSourcePdf,*sumConvolutedComponents), RooArgList(*ISourceNorm));   
}

RooAbsPdf* AdditiveConvolutionPdf::getPdf() {
    return model;
}

RooAbsPdf* AdditiveConvolutionPdf::getResolutionFunction() {
    return resolutionFunction;
}

RooArgList* AdditiveConvolutionPdf::getAllComponents() {
    return componentsList ;
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
