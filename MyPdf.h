/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MyPdf.h
 * Author: petrstepanov
 *
 * Created on November 9, 2016, 8:48 PM
 */

#ifndef MY_MYPDF
#define MY_MYPDF

#include "MyPdfCache.h"
#include "RooAbsReal.h"
#include "RooAbsPdf.h"
#include "TObject.h"
#include "RooRealProxy.h"
#include "TH1F.h"
#include "TAxis.h"

class MyPdf : public RooAbsPdf {
public:
    MyPdf() {};
    MyPdf(const char *name, const char *title,
            RooAbsReal& _t,
            RooAbsReal& _lambdaJ,
            RooAbsReal& _lambdaBulk,
            RooAbsReal& _lambdaGrain,
            RooAbsReal& _lambdaVac,
            RooAbsReal& _kappaVac
            );
    MyPdf(const MyPdf& other, const char* name = 0);
    virtual TObject* clone(const char* newname) const { return new MyPdf(*this, newname); }
    inline virtual ~MyPdf() { }     

    static MyPdfCache* myPdfCache;
    
protected:
    RooRealProxy t;
    RooRealProxy lambdaJ;
    RooRealProxy lambdaBulk;
    RooRealProxy lambdaGrain;
    RooRealProxy lambdaVac;
    RooRealProxy kappaVac;

    Double_t evaluate() const;
    
private:
    // ClassDef(MyPdf, 1) // Your description goes here...
};

#endif /* MY_MYPDF */

