/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MyPdfCache.h
 * Author: petrstepanov
 *
 * Created on November 10, 2016, 2:21 AM
 */

#ifndef MY_MYPDFCACHE
#define MY_MYPDFCACHE

#include "StructParams.h"
#include "RooAbsReal.h"
#include "RooAbsPdf.h"
#include "TObject.h"
#include "RooRealProxy.h"
#include "TH1F.h"
#include "TAxis.h"

class MyPdfCache {
public:
    MyPdfCache();
    MyPdfCache(const MyPdfCache& orig);
    virtual ~MyPdfCache();    
    MyPdfCache(unsigned cacheSize, int minChannel, int maxChannel);
    double getCachedOrEvaluate(double, double, double, double, double, double);

protected:
    unsigned cacheSize;
    std::map<StructParams, TH1F*> histMap;
    unsigned size;
    int minChannel;
    int maxChannel;
    double evaluate(double, double, double, double, double, double);
//    TH1F* createHist(double, double, double, double, double, double);
    TH1F* createHist(double, double, double, double, double, double);
};

#endif /* MY_MYPDFCACHE */

