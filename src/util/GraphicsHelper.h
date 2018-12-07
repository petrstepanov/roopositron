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
#include "RooPlot.h"

class GraphicsHelper {
public:
    static void drawRegion(RooPlot* frame, Int_t xMin, Int_t xMax);
//    GraphicsHelper();
//    GraphicsHelper(const GraphicsHelper& orig);
//    virtual ~GraphicsHelper();
};

#endif /* GRAPHICSHELPER_H */

