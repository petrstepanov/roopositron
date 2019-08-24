/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   HistProcessor.cpp
 * Author: petrstepanov
 * 
 * Created on December 8, 2018, 11:35 AM
 */

#include "HistProcessor.h"

HistProcessor::HistProcessor() {
}

HistProcessor::HistProcessor(const HistProcessor& orig) {
}

HistProcessor::~HistProcessor() {
}

Double_t HistProcessor::getConstBackgroundFraction(TH1F* hist){
    Int_t nBins = hist->GetXaxis()->GetNbins();
    const Int_t wingBins = 20;
    Double_t fullInt = hist->Integral(1, nBins);
    Double_t leftWingAverage = (hist->Integral(1, wingBins)) / (Double_t)(wingBins);
//    Double_t rightWingAverage = (hist->Integral(nBins - wingBins, nBins)) / (Double_t)(wingBins);
//    Double_t bgInt = nBins * rightWingAverage;
    Double_t bgInt = nBins * leftWingAverage;
    return bgInt / fullInt;
}

// Calculate average background value on the left side of the spectrum
Double_t HistProcessor::getAverageBackground(TH1F* hist, Int_t leftWingBins) {
    Double_t wingIntegral = hist->Integral(1, leftWingBins);
    return wingIntegral / leftWingBins;
}

void HistProcessor::setZeroErrors(RooHist* h){
	// Weird but in RooHist bins numbered 0 to < nbins opposed to TH1 (from 1 to <= nbins)
	for (UInt_t i=0; i<h->GetXaxis()->GetNbins(); i++){
		h->SetPointEYhigh(i, 0);
		h->SetPointEYlow(i, 0);
	}
}

TH1F* HistProcessor::subtractCurve(const char *newname, TH1F* hist, RooCurve* curve){
	Double_t xMin = hist->GetXaxis()->GetXmin();
	Double_t xMax = hist->GetXaxis()->GetXmax();
	Double_t nBins = hist->GetXaxis()->GetNbins();

//        RootHelper::deleteObject("histMinusCurve");
	TH1F* histMinusCurve = new TH1F(newname, "Histogram minus curve", nBins, xMin, xMax);
	for (int i = 1; i <= nBins; i++){
		Double_t bg = (curve == NULL) ? 0.0 : curve->Eval(hist->GetXaxis()->GetBinCenter(i));
		if (hist->GetBinContent(i) - bg >= 0) {
			histMinusCurve->SetBinContent(i, hist->GetBinContent(i) - bg);
			histMinusCurve->SetBinError(i, hist->GetBinError(i));
		}
		else {
			histMinusCurve->SetBinContent(i, 0);
			histMinusCurve->SetBinError(i, 0);
		}
	}
	return histMinusCurve;
}
