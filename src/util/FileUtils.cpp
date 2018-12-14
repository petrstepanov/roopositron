/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FileUtils.cpp
 * Author: petrstepanov
 * 
 * Created on November 9, 2016, 11:41 PM
 */

#include "FileUtils.h"
#include "../model/Constants.h"
#include "../util/StringUtils.h"
#include <TUnixSystem.h>

TH1F* FileUtils::importTH1F(std::string filename, int i){
    Constants* constants = Constants::getInstance();
    const int minChannel = constants->getMinChannel();
    const int maxChannel = constants->getMaxChannel();
    const int skipLines =  constants->getSkipLines();

    TH1F* histogram = new TH1F(TString::Format("histogram_%d", i + 1), TString::Format("histogram_%d", i + 1), maxChannel - minChannel + 1, 0, maxChannel - minChannel + 1);
    // Open file
    std::ifstream inFile(filename.c_str());
    // Treminate if error
    if (!inFile){
	std::cout << "File \"" << filename << "\" not found." << std::endl;
	exit(1);
    }

    // Skip spectrum header
    std::string sLine = filename;
    for (unsigned j = 0; j < skipLines; j++){
	getline(inFile, sLine);
    }
    
    // Now `sLine` contains first line with data. Read spectrum data
    // Skip minChannel - 1 lines;
    for (unsigned j = 0; j < minChannel - 1; j++){
	    getline(inFile, sLine);
    }
	
    // Fill in histogram
    for (unsigned j = 1; j <= maxChannel-minChannel+1; j++){
	getline(inFile, sLine);
	std::istringstream streamLine(sLine);
	Double_t dCount;
	streamLine >> dCount;
	if (streamLine.fail()){
	    std::cout << "Failed parsing file." << std::endl;
	    streamLine.clear();
	    dCount = 0;
	}
	histogram->SetBinContent(j, dCount + 1); // The RooFit chi2 fit does not work when the bins have zero entries.
	histogram->SetBinError(j, sqrt(dCount+1));
    }
    
    inFile.close();
    return histogram;
}

void FileUtils::createDirectory(std::string directoryName) {
    std::string createDirCommand = "mkdir " + directoryName;
    gSystem->Exec(createDirCommand.c_str());
}

std::string FileUtils::getCurrentPath(){
    char currentPath[] = "./";
    currentPath[1] = kPathSeparator;
    std::string path (currentPath);
    return path;
}

std::vector<std::string> FileUtils::getFilenamesInCurrentDrectory(const char* ext){
    // Grab spectra filenames from input directory
    struct dirent *ent;
    std::vector<std::string> lFileNames;
    DIR *dir = opendir("./");
    if (dir != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            std::string filename(ent->d_name);
            std::cout << filename << std::endl;
            std::string extension = ext;
            if (StringUtils::stringEnds(filename, extension)) {
                lFileNames.push_back(filename);
                printf("%s\n", ent->d_name);
            }
        }
        closedir(dir);
    }
    else {
        // Could not open directory
        perror("");
    }
    return lFileNames;
}

void FileUtils::saveImage(TCanvas* canvas, const char* filename){
    TImage *imgCanvas = TImage::Create();
    imgCanvas->FromPad(canvas);
    imgCanvas->WriteImage(filename);
    std::cout << "FileUtils::saveImage " << filename << std::endl;
    delete imgCanvas;
}