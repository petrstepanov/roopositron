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
#include "ReverseAddPdf.h"
#include "../util/StringUtils.h"
#include "../model/Constants.h"
#include <RooFFTConvPdf.h>
#include <RooAddPdf.h>

AdditiveConvolutionPdf::AdditiveConvolutionPdf(std::vector<std::string> componentIds, const char* resolutionId, int sourceComponents, RooRealVar* observable) {
    pdfServer = new PdfServer();
    componentsNumber = componentIds.size();
    sourceComponentsNumber = sourceComponents;
    initComponents(componentIds, sourceComponents, observable);
    initResolutionModel(resolutionId, observable);
    initCoefficients();
    convoluteComponents(observable);
    constructModel();
}

AdditiveConvolutionPdf::~AdditiveConvolutionPdf() {}

void AdditiveConvolutionPdf::initComponents(std::vector<std::string> componentIds, int sourceComponents, RooRealVar* observable) {
//    std::cout << std::endl << "AdditiveConvolutionPdf::initComponents" << std::endl;
//    std::cout << "Found " << componentsNumber << " components" << std::endl;
//    ObjectNamer* pdfNamer = new ObjectNamer();
//    ObjectNamer* coefficientsNamer = new ObjectNamer();

//    ObjectNamer* pdfAndCoeficientsNamer = new ObjectNamer();

    // Build component PDFs
    for (std::vector<std::string>::const_iterator it = componentIds.begin(); it != componentIds.end(); ++it){
//	std::cout << "Component " << *it << std::endl;
	const char* componentId = ((std::string)*it).c_str();
	RooAbsPdf* pdf = pdfServer->getPdf(componentId, observable);
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
    for (unsigned i = 1; i <= sourceComponents; i++){
        SourceProvider* sp = new SourceProvider(observable);
	RooAbsPdf* sourceComponent = sp->getPdf(i);
	sourceComponentsList->add(*sourceComponent);
    }
    
//    sourcePdf->Print();    
}

void AdditiveConvolutionPdf::initResolutionModel(const char* resolutionId, RooRealVar* observable) {
//    std::cout << std::endl << "AdditiveConvolutionPdf::initResolutionModel" << std::endl;
    resolutionFunction = pdfServer->getPdf(resolutionId, observable);
//    resolutionFunction->Print();    
}

void AdditiveConvolutionPdf::initCoefficients(){
        // Initialize source contribution coefficient
    ISource = new RooRealVar("ISource", "Source contribution", 11, 5, 20, "%");
    ISourceNorm = new RooFormulaVar("ISourceNorm", "@0/100", *ISource);
}

void AdditiveConvolutionPdf::convoluteComponents(RooRealVar *observable){
    // Convolute components
    double bufferFraction = Constants::getInstance()->getBufferFraction();
    for (unsigned i = 0; i < componentsNumber; i++){
	RooAbsArg* arg = (componentsList->at(i));
	RooAbsPdf* component = dynamic_cast<RooAbsPdf*>(arg);
	RooFFTConvPdf* pdf = new RooFFTConvPdf(StringUtils::suffix("convolutedComponent", i+1).c_str(), 
		StringUtils::ordinal("convoluted component", i+1).c_str(), *observable, *component, *resolutionFunction);
	pdf->setBufferFraction(bufferFraction);
//	pdf->setBufferStrategy(RooFFTConvPdf::Extend);	
	convolutedComponentsList->add(*pdf);
    }
    convolutedComponentsList->Print();

    // Convolute source components
    for (unsigned i = 0; i < sourceComponentsNumber; i++){
	RooAbsArg* arg = (sourceComponentsList->at(i));
	RooAbsPdf* component = dynamic_cast<RooAbsPdf*>(arg);
	RooFFTConvPdf* pdf = new RooFFTConvPdf(StringUtils::suffix("sourceConvolutedComponent", i+1).c_str(), 
		StringUtils::ordinal("source convoluted component", i+1).c_str(), *observable, *component, *resolutionFunction);
	pdf->setBufferFraction(bufferFraction); // because sometimes (on some ranges) convolution produced PDF with weird peak
//	pdf->setBufferStrategy(RooFFTConvPdf::Extend); // large lifetimes ~ 4.8ns give "p.d.f. normalization integral is zero" Extend=0, Mirror=1, Flat=2
	convolutedSourceComponentsList->add(*pdf);
    }
    convolutedSourceComponentsList->Print();
}

void AdditiveConvolutionPdf::constructModel(){
//    std::cout << std::endl << "AdditiveConvolutionPdf::constructModel" << std::endl;
    // Sum components
    RooAbsPdf* sumConvolutedComponents = ReverseAddPdf::add(convolutedComponentsList);
    RooAbsPdf* sumSourceConvolutedComponents = ReverseAddPdf::add(convolutedSourceComponentsList, "Src");

    model = new RooAddPdf("componentsSourceModel", "Components model with source", RooArgList(*sumSourceConvolutedComponents,*sumConvolutedComponents), RooArgList(*ISourceNorm));   
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
