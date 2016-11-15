/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MyPdfCache.cpp
 * Author: petrstepanov
 * 
 * Created on November 10, 2016, 2:21 AM
 */

#include "MyPdfCache.h"

MyPdfCache::MyPdfCache() {
}

MyPdfCache::MyPdfCache(const MyPdfCache& orig) {
}

MyPdfCache::~MyPdfCache() {
}

MyPdfCache::MyPdfCache(unsigned _cacheSize, int _minChannel, int _maxChannel) {
    cacheSize = _cacheSize;
    size = 0;
    minChannel = _minChannel;
    maxChannel = _maxChannel;
}

double MyPdfCache::getCachedOrEvaluate(double t, double lambdaJ, double lambdaBulk, double lambdaGrain, double lambdaVac, double kappaVac){
    // if hist with parameters exist in cache return cached value
    // std::cout << "For parameters t=" << t << ", lJ=" << lambdaJ << ", lB=" << lambdaBulk << ", lG=" << lambdaGrain << ", lV=" << lambdaVac << ", kV=" << kappaVac << std::endl;
    const StructParams tempParams(lambdaJ, lambdaBulk, lambdaGrain, lambdaVac, kappaVac);
    std::map<StructParams, TH1F*>::iterator it = histMap.find(tempParams);
    // if hist was not found in cache
    if (it == histMap.end()){
        // if hist with these paramaters does not exist - calculate it and remember
        if (size >= cacheSize){
                std::cout << "over the limit; deleting old hist" << std::endl;
                // delete std::prev(histMap.end());
                histMap.erase(std::prev(histMap.end()));
                size--;
        }

        TH1F* newHist = createHist(t, lambdaJ, lambdaBulk, lambdaGrain, lambdaVac, kappaVac);
        // newHist->Print();
        histMap.insert(std::pair<StructParams, TH1F*>(tempParams, newHist));
        size++;
        std::cout << "hists remembered: " << size << std::endl;
        // std::cout << "object id: " << id << std::endl;

        // return the correspondent value
        TAxis *xAxis = newHist->GetXaxis();
        Int_t bin = xAxis->FindBin(t);
        return newHist->GetBinContent(bin);
    }
    // if hist was found - return cached
    else {
        TH1F* hist = it->second;
        // std::cout << "using cached histogram \"" << hist->GetName() << "\""<< std::endl;
        TAxis *xAxis = hist->GetXaxis();
        Int_t bin = xAxis->FindBin(t);
        return hist->GetBinContent(bin);
    }
}

TH1F* MyPdfCache::createHist(double t, double lambdaJ, double lambdaBulk, double lambdaGrain, double lambdaVac, double kappaVac){

    TH1F* hist = new TH1F(TString::Format("cachedTH1F_%d", size + 1), TString::Format("cachedTH1F_%d", size + 1), maxChannel - minChannel + 1, 0, maxChannel-minChannel+1);
    for (unsigned i = 1; i <= hist->GetNbinsX(); i++){
            double val = evaluate(hist->GetBinCenter(i), lambdaJ, lambdaBulk, lambdaGrain, lambdaVac, kappaVac);
            hist->SetBinContent(i, val);
    }
    std::cout << "For parameters t=" << t << ", lJ=" << lambdaJ << ", lB=" << lambdaBulk << ", lG=" << lambdaGrain << ", lV=" << lambdaVac << ", kV=" << kappaVac << std::endl;
    std::cout << "making new historgam \"" << hist->GetName() << "\""<< std::endl;
    return hist;
}

double MyPdfCache::evaluate(double t, double lambdaJ, double lambdaBulk, double lambdaGrain, double lambdaVac, double kappaVac){
    if (t < 0) return 0.;
    double pi = 3.14159;
    double iterations = 5;

    // n_b
    double n_b = 0;
    for (unsigned n = 1; n <= iterations; n++){
            n_b += exp((-1.)*lambdaJ*n*n*t) / (n*n);
    }
    n_b = 6 * exp((-1.)*(lambdaBulk + kappaVac)*t) / pow(pi, 2) * n_b;

    // n_v
    double n_v = 0;
    for (unsigned n = 1; n <= iterations; n++){
            n_v += (exp((-1.)*lambdaVac*t) - exp((-1.)*(lambdaBulk + kappaVac + lambdaJ*n*n)*t)) / (n*n*(lambdaBulk + kappaVac + lambdaJ*n*n - lambdaVac));
    }
    n_v = 6 * kappaVac / pow(pi, 2) * n_v;

    // n_g
    double n_g = 0;
    for (unsigned n = 1; n <= iterations; n++){
            n_g += (exp((-1.)*lambdaGrain*t) - exp((-1.)*(lambdaBulk + kappaVac + lambdaJ*n*n)*t)) / (lambdaBulk + kappaVac + lambdaJ*n*n - lambdaGrain);
    }
    n_g = 6 * lambdaJ / pow(pi, 2) * n_g;

    return lambdaBulk*n_b + lambdaVac*n_v + lambdaGrain*n_g;
}

