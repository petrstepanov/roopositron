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
#include "RootHelper.h"
#include "StringUtils.h"
#include "Debug.h"
#include <TBox.h>
#include <TAttText.h>
#include <TPaveText.h>
#include <iostream>

const Double_t GraphicsHelper::FONT_SIZE_SMALLER = 0.02;
const Double_t GraphicsHelper::FONT_SIZE_SMALL = 0.028;
const Double_t GraphicsHelper::FONT_SIZE_NORMAL = 0.05;
const Font_t GraphicsHelper::FONT_TEXT = 42; // https://root.cern.ch/doc/master/classTAttText.html#T5
const Double_t GraphicsHelper::RESIDUALS_PAD_RELATIVE_HEIGHT = 0.35;
const Double_t GraphicsHelper::LEGEND_XMIN = 0.7;
const Double_t GraphicsHelper::MARKER_SIZE = 0.3;
const Double_t GraphicsHelper::LEGEND_LINE_HEIGHT = 0.0248;
const Style_t GraphicsHelper::COLORS[16] = {kGray, kOrange + 1, kSpring - 5, kAzure + 8, kPink + 1, kViolet - 4, kRed - 7, kViolet + 6};

const char* GraphicsHelper::ATTR_IS_PRINTED = "attrIsPrinted";

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

void GraphicsHelper::printVariable(Int_t sigDigits, const char* options, Int_t& currentLine, RooAbsArg* rooAbsArg, TPaveText* box) {
	// Debug what are we printing
	#ifdef USEDEBUG
		std::cout << "GraphicsHelper::printVariable" << std::endl;
		rooAbsArg->Print();
	#endif

	// Don't print variable that was printed before
	if (rooAbsArg->getAttribute(ATTR_IS_PRINTED) == kTRUE) return;

	if (RooRealVar* var = dynamic_cast<RooRealVar*>(rooAbsArg)){
		TString* formatted = var->format(sigDigits, options);
		const char* formattedString = formatted->Data();

		Bool_t isConstant = var->isConstant();
		Bool_t atMaximum = var->getVal() + var->getError() > var->getMax();
		Bool_t atMinimum = var->getVal() - var->getError() < var->getMin();

		#ifdef USEDEBUG
			std::cout << "GraphicsHelper::printVariable" << std::endl;
			std::cout << var->GetName() << "; constant " << isConstant << "; atMinimum " << atMinimum << "; atMaximum " << atMaximum << std::endl;
		#endif

		if (!isConstant && (atMaximum || atMinimum)){
			TText* t = box->AddText(TString::Format("%s (at limit)", formattedString).Data());
			t->SetTextColor(kOrange+8);
		} else {
			box->AddText(formattedString);
		}
		// Mark variable as printed
		rooAbsArg->setAttribute(ATTR_IS_PRINTED, kTRUE);
		currentLine++;
	}
	else if (RooFormulaVar* var = dynamic_cast<RooFormulaVar*>(rooAbsArg)){
		#ifdef USEDEBUG
			std::cout << "GraphicsHelper::printVariable" << std::endl;
			std::cout << var->GetName() << std::endl;
		#endif
		TString fmt("%s = %.#f (indirect)");
		fmt.ReplaceAll("#", TString::Format("%d", sigDigits).Data());
		TString formatted = TString::Format(fmt.Data(), var->GetName(), var->evaluate());
		const char* formattedString = formatted.Data();
		TText* t = box->AddText(formattedString);
		t->SetTextColor(kGray+1);
		// Mark variable as printed
		rooAbsArg->setAttribute(ATTR_IS_PRINTED, kTRUE);
		currentLine++;
	}
}

TPaveText* GraphicsHelper::makePaveText(const RooArgSet& params, Double_t xmin, Double_t xmax, Double_t ymax) {

	std::cout << "GraphicsHelper::makePaveText" << std::endl;
	params.Print("V");

	Bool_t showConstants = kTRUE;
	const char* options = "NEULP";
	TIterator* pIter = params.createIterator();

	// Calculate bottom Legend coordinate (ymin)
	Double_t ymin = ymax;
	while (RooAbsArg* var = (RooAbsArg*) pIter->Next()) {
		if (showConstants || !var->isConstant()) ymin -= LEGEND_LINE_HEIGHT;
	}
	ymin -= LEGEND_LINE_HEIGHT*5; // 2 empty lines (top and bottom) + 3 separators

	// Create the box and set its options
	TPaveText *box = new TPaveText(xmin, ymax, xmax, ymin, "BRNDC"); //
	box->SetName("myParamBox");
	box->SetFillColor(EColor::kBlack);
	box->SetBorderSize(1);
	box->SetTextAlign(ETextAlign::kHAlignLeft + ETextAlign::kVAlignCenter);
	box->SetTextSize(FONT_SIZE_SMALL);
	box->SetTextFont(FONT_TEXT);
	box->SetFillStyle(1001);
	box->SetFillColorAlpha(EColor::kWhite, 0.9);

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
	if (RooAbsArg* var = RootHelper::findArgNameSubstring(paramsList, "bins")) {
		printVariable(sigDigits, options, linesNumber, var, box);
	}

	if (RooAbsArg* var = RootHelper::findArgNameSubstring(paramsList, "integral")) {
		printVariable(sigDigits, options, linesNumber, var, box);
	}

	sigDigits = 3;
	if (RooAbsArg* var = RootHelper::findArgNameSubstring(paramsList, "background")) {
		printVariable(sigDigits, options, linesNumber, var, box);
	}

	sigDigits = 5;
	if (RooAbsArg* var = RootHelper::findArgNameSubstring(paramsList, "channelWidth")) {
		printVariable(sigDigits, options, linesNumber, var, box);
	}

	// Add horizontal rule
	hrLineNumbers.push_back(linesNumber);
	box->AddText("");
	linesNumber++;

	sigDigits = 3;
	// Print source contribution related variables
	RooArgList* sourceVarsList = RootHelper::findArgsNameSubstring(paramsList, "ource");
	for (auto arg : *sourceVarsList) {
		printVariable(sigDigits, options, linesNumber, arg, box);
	}

	// Add horizontal rule
	hrLineNumbers.push_back(linesNumber);
	box->AddText("");
	linesNumber++;

	// Print resolution function related variables
	RooArgList* gaussVarsList = RootHelper::findArgsNameSubstring(paramsList, "gauss");
	for (auto arg : *gaussVarsList) {
		printVariable(sigDigits, options, linesNumber, arg, box);
	}

	// Add horizontal rule
	hrLineNumbers.push_back(linesNumber);
	box->AddText("");
	linesNumber++;

	// Print other variables (model)
	pIter = paramsList->createIterator();
	while (TObject* obj = pIter->Next()) {
		if (RooAbsArg* var = dynamic_cast<RooAbsArg*>(obj)){

			//if (realVar->isConstant() && !showConstants) continue;
			printVariable(sigDigits, options, linesNumber, var, box);
		}
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

	// Remove printed flags
	pIter = params.createIterator();
	while (TObject* obj = pIter->Next()) {
		if (RooAbsArg* var = dynamic_cast<RooAbsArg*>(obj)){
			if (var->getAttribute(ATTR_IS_PRINTED) == kTRUE){
				var->setAttribute(ATTR_IS_PRINTED, kFALSE);
			}
		}
	}

	delete pIter;
	return box;
}
