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

class GraphicsHelper {
public:
	static void drawRegion(RooPlot* frame, Int_t xMin, Int_t xMax);
	static TPaveText* makePaveText(const RooArgSet& params, Int_t sigDigits, Double_t xmin, Double_t xmax, Double_t ymax);

//    GraphicsHelper();
//    GraphicsHelper(const GraphicsHelper& orig);
//    virtual ~GraphicsHelper();

private:
	static RooRealVar* findRooRealVarInList(RooArgList* list, const char* name);
};

#endif /* GRAPHICSHELPER_H */

