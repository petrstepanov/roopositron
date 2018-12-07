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
#include "TBox.h"
#include <iostream>

void GraphicsHelper::drawRegion(RooPlot* frame, Int_t xMin, Int_t xMax) {
    Double_t yMin = frame->GetMinimum(); // frame->GetYaxis()->GetXmin();
    Double_t yMax = frame->GetMaximum(); // frame->GetYaxis()->GetXmax();
    TBox* sBox = new TBox(xMin, yMin, xMax, yMax);
    sBox->SetLineWidth(0);
    sBox->SetFillColorAlpha(15, 0.2);
    frame->addObject(sBox);

    std::cout << "xMin: " << xMin << ", xMax: " << xMax << std::endl;
    std::cout << "yMin: " << yMin << ", yMax: " << yMax<< std::endl;
}
