/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GraphicsHelper.cpp
 * Author: petrstepanov
 * 
 * Created on December 5, 2018, 10:12 AM
 */

#include "GraphicsHelper.h"
#include "StringUtils.h"
#include "Debug.h"
#include <TBox.h>
#include <TPaveText.h>
#include <RooRealVar.h>
#include <iostream>

const Double_t GraphicsHelper::FONT_SIZE_SMALL = 0.028;
const Double_t GraphicsHelper::FONT_SIZE_NORMAL = 0.05;
const Double_t GraphicsHelper::RESIDUALS_PAD_RELATIVE_HEIGHT = 0.35;
const Double_t GraphicsHelper::LEGEND_XMIN = 0.7;

Double_t GraphicsHelper::getSpectrumPadFontFactor(){
	return 0.5/(1-RESIDUALS_PAD_RELATIVE_HEIGHT);
}

Double_t GraphicsHelper::getResidualsPadFontFactor(){
	// Pad's font size depends on its height. Uneven pad heights result in different font sizes.
	// This function fives a coefficient that evens the font size out
	return 0.5/RESIDUALS_PAD_RELATIVE_HEIGHT;
}


void GraphicsHelper::drawRegion(RooPlot* frame, Int_t xMin, Int_t xMax) {
	Double_t yMin = frame->GetMinimum(); // frame->GetYaxis()->GetXmin();
	Double_t yMax = frame->GetMaximum(); // frame->GetYaxis()->GetXmax();
	TBox* sBox = new TBox(xMin, yMin, xMax, yMax);
	sBox->SetLineWidth(0);
	sBox->SetFillColorAlpha(15, 0.2);
	frame->addObject(sBox);
	Debug("GraphicsHelper::drawRegion", "xMin: " << xMin << ", xMax: " << xMax << "yMin: " << yMin << ", yMax: " << yMax);
}

void GraphicsHelper::printVariable(Int_t sigDigits, const char* options, Int_t& currentLine, RooRealVar* var, TPaveText* box, RooArgList* paramsList) {
	TString* formatted = var->format(sigDigits, options);
	const char* formattedString = formatted->Data();
	TText* t = box->AddText(formattedString);

	Bool_t notConstant = !var->isConstant();
	Bool_t atMaximum = var->getVal() + var->getError() > var->getMax();
	Bool_t atMinimum = var->getVal() - var->getError() < var->getMin();
	if (notConstant && (atMaximum || atMinimum)) t->SetTextColor(kOrange+8);
	paramsList->remove(*var);
	currentLine++;

	Debug("GraphicsHelper::printVariable", var->GetName() << "; notConstant " << notConstant << "; atMaximum " << atMaximum << "; atMinimum " << atMinimum);
}

TPaveText* GraphicsHelper::makePaveText(const RooArgSet& params, Double_t xmin, Double_t xmax, Double_t ymax) {

	Bool_t showConstants = kTRUE;

	// parse the options
//	TString opts = TString("NEU");
//	opts.ToLower();

	const char* options = "NEULP";


	// calculate the box's size, adjusting for constant parameters
	TIterator* pIter = params.createIterator();

	Double_t ymin(ymax), dy(0.03);
	RooRealVar *var = 0;
	while ((var = (RooRealVar*) pIter->Next())) {
		if (showConstants || !var->isConstant())
			ymin -= dy;
	}

	ymin -= dy*5; // New line on top abd bottom plus three lines

	// create the box and set its options
	TPaveText *box = new TPaveText(xmin, ymax, xmax, ymin, "BRNDC");
	if (!box)
		return 0;
	box->SetName("myParamBox");
	box->SetFillColor(0);
	box->SetBorderSize(1);
	box->SetTextAlign(12);
	box->SetTextSize(FONT_SIZE_SMALL);
	box->SetFillStyle(1001);
	box->SetFillColorAlpha(0, 0.9);
	//char buffer[512];

	RooArgList* paramsList = new RooArgList(params);
	paramsList->sort();

	// Line counter (to draw horizontal lines later)
	Int_t linesNumber = 0;
	std::vector<int> hrLineNumbers;

	// Empty first line (for better padding)
	box->AddText("");
	linesNumber++;

	Int_t sigDigits = 0;
	// Print "bins", "integral" and "background" variables first
	if (RooRealVar* var = findRooRealVarInList(paramsList, "bins")) {
		printVariable(sigDigits, options, linesNumber, var, box, paramsList);
	}

	if (RooRealVar* var = findRooRealVarInList(paramsList, "integral")) {
		printVariable(sigDigits, options, linesNumber, var, box, paramsList);
	}

	sigDigits = 3;
	if (RooRealVar* var = findRooRealVarInList(paramsList, "background")) {
		printVariable(sigDigits, options, linesNumber, var, box, paramsList);
	}

	// Add horizontal rule
	hrLineNumbers.push_back(linesNumber);
	box->AddText("");
	linesNumber++;

	// Print source contribution related variables
	while (RooRealVar* var = findRooRealVarInList(paramsList, "ource")) {
		printVariable(sigDigits, options, linesNumber, var, box, paramsList);
	}

	// Add horizontal rule
	hrLineNumbers.push_back(linesNumber);
	box->AddText("");
	linesNumber++;

	// Print resolution function related variables
	while (RooRealVar* var = findRooRealVarInList(paramsList, "gauss")) {
		printVariable(sigDigits, options, linesNumber, var, box, paramsList);
	}

	// Add horizontal rule
	hrLineNumbers.push_back(linesNumber);
	box->AddText("");
	linesNumber++;

	// Print other variables (model)
	pIter = paramsList->createIterator();
	while ((var = (RooRealVar*) pIter->Next())) {
		if (var->isConstant() && !showConstants) continue;
		printVariable(sigDigits, options, linesNumber, var, box, paramsList);
	}

	// Empty last line (for better padding)
	box->AddText("");
	linesNumber++;

	// Draw horizontal rules
	for (std::vector<int>::iterator it = hrLineNumbers.begin(); it != hrLineNumbers.end(); ++it){
		Debug("GraphicsHelper::makePaveText", "line at: " << *it << "; total lines: " << linesNumber);
		Double_t y = ((Double_t)*it + 0.5)/(Double_t)linesNumber;
		box->AddLine(0, 1-y, 1, 1-y);
	}

	delete pIter;
	return box;
}

// Finds RooRealVar in list by a name substring
RooRealVar* GraphicsHelper::findRooRealVarInList(RooArgList* list, const char* nameSubstring) {
	TIterator* it = list->createIterator();
	TObject* temp;
	while ((temp = it->Next())) {
		if (RooRealVar* rooRealVar = dynamic_cast<RooRealVar*>(temp)) {
			const char* name = rooRealVar->GetName();
			if (StringUtils::isSubstring(name, nameSubstring)) {
				return rooRealVar;
			}
		}
	}
	return NULL;
}
