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

#include "TH1F.h"
#include "TCanvas.h"
#include "TImage.h"
#include "dirent.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <list>
#include <string>

class FileUtils {
public:
    static std::vector<std::string> getFilenamesInCurrentDrectory(const char* extension);
    static TH1F* importTH1F(std::string filename, int i);
    static void saveImage(TCanvas*, const char*);
    static void createDirectory(std::string directoryName);
    
private:
    std::string getCurrentPath();
    const char kPathSeparator =
#ifdef _WIN32
    '\\';
#else
    '/';
#endif
};

#endif /* MY_FILEUTILS */

