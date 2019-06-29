/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GraphicsHelper.h
 * Author: petrstepanov
 *
 * Created on December 5, 2018, 10:12 AM
 */

#ifndef GRAPHICSHELPER_H
#define GRAPHICSHELPER_H
#include <RooPlot.h>
#include <RooAbsPdf.h>
#include <RooRealVar.h>
#include <RooFormulaVar.h>

class GraphicsHelper {
public:
	static void drawRegion(RooPlot* frame, Int_t xMin, Int_t xMax);
	static TPaveText* makePaveText(const RooArgSet& params, Double_t xmin, Double_t xmax, Double_t ymax);

//    GraphicsHelper();
//    GraphicsHelper(const GraphicsHelper& orig);
//    virtual ~GraphicsHelper();

	static const Double_t FONT_SIZE_SMALLER;
	static const Double_t FONT_SIZE_SMALL;
	static const Double_t FONT_SIZE_NORMAL;
	static const Double_t RESIDUALS_PAD_RELATIVE_HEIGHT;
	static const Double_t LEGEND_XMIN;
	static const Double_t MARKER_SIZE;
	static const Style_t COLORS[16];

	static const Style_t GRAPH_COLOR = kViolet + 6;

	static Double_t getSpectrumPadFontFactor();
	static Double_t getResidualsPadFontFactor();

private:
	static void printVariable(Int_t sigDigits, const char* options, Int_t& currentLine, RooAbsArg* rooAbsReal, TPaveText* box, RooArgList* paramsList);
};

#endif /* GRAPHICSHELPER_H */

