/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FileUtils.h
 * Author: petrstepanov
 *
 * Created on November 9, 2016, 11:41 PM
 */

#ifndef MY_FILEUTILS
#define MY_FILEUTILS

#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include <TH1F.h>
#include <RooArgList.h>
#include <RooRealVar.h>

class FileUtils {
public:
    static std::vector<std::string> getFilenamesInCurrentDrectory(const char* extension);
    static TH1F* importTH1F(std::string filename, int i);
    static void createDirectory(std::string directoryName);
    static void savePlotsToFile(RooPlot* spectrumPlot, RooPlot* residualsPlot, const char* fileName,  RooRealVar* observable, Double_t conversionValue = 0, Double_t zeroChannel = 0, const char* conversionUnit = "");
    
private:
    std::string getCurrentPath();
    const char kPathSeparator = '/';
};

#endif /* MY_FILEUTILS */

