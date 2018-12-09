/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   RootUtils.cpp
 * Author: petrstepanov
 *
 * Created on December 4, 2018, 2:09 PM
 */

#include "RootHelper.h"
#include <TUnixSystem.h>
#include <iostream>

//void RootHelper::deleteObject(const char* name){
//    TObject* obj = gROOT->FindObject(name);
//    deleteObject(obj);
//}
//
//void RootHelper::deleteObject(TObject* obj){
//    if (obj){
//        obj->Delete();
//    }
//}

Int_t RootHelper::getNumCpu(){
    SysInfo_t sysInfo;
    gSystem->GetSysInfo(&sysInfo);
    std::cout << "RootHelper::getNumCpu: sysInfo.fCpus = " << sysInfo.fCpus << std::endl;
    return (sysInfo.fCpus >= 0) ? sysInfo.fCpus : 1;
}

TStopwatch* RootHelper::watch = new TStopwatch();

void RootHelper::startTimer(){
    watch->Start();    
}

void RootHelper::stopAndPrintTimer(){
    watch->Stop();
    watch->Print();
}

void RootHelper::printPdfCoefficientNames(RooAbsPdf* pdf, RooRealVar* observable) {
    std::cout << pdf->GetName() << ": ";

    RooArgSet* params = pdf->getParameters(*observable);
    TIterator* it = params->createIterator();
    TObject* temp;
    while((temp = it->Next())){
	TNamed* named = dynamic_cast<TNamed*>(temp);
	if (named){
	    std::cout << named->GetName() << " ";
	}
    }
    std::cout << std::endl;
}