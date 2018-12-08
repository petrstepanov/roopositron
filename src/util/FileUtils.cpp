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
#include <TUnixSystem.h>

FileUtils::FileUtils() {
}

FileUtils::FileUtils(const FileUtils& orig) {
}

FileUtils::~FileUtils() {
}

void FileUtils::importTH1F(TH1F* fullTH1F, std::string& filename, int channels, int skipLines, int minChannel, int maxChannel){
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
//                std::cout << sLine << std::endl;
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
            fullTH1F->SetBinContent(j, dCount + 1); // The RooFit chi2 fit does not work when the bins have zero entries.
            fullTH1F->SetBinError(j, sqrt(dCount+1));
	}        
//	while (iChannel <= channels) {
//		Double_t dCount, dError;
//		std::istringstream streamLine(sLine);
//		streamLine >> dCount;
//		if (iChannel >= minChannel && iChannel <= maxChannel){
//			fullTH1F->SetBinContent(iChannel - minChannel + 1, dCount + 1); // The RooFit chi2 fit does not work when the bins have zero entries.
//			fullTH1F->SetBinError(iChannel - minChannel + 1, sqrt(dCount+1));
//		}
//		getline(inFile, sLine);
////                std::cout << sLine << std::endl;
//		iChannel++;
//	}
	inFile.close();
	fullTH1F->Print();
}

void FileUtils::createDirectory(std::string directoryName) {
    std::string createDirCommand = "mkdir " + directoryName;
    gSystem->Exec(createDirCommand.c_str());
}


bool FileUtils::stringEnds(std::string& string, std::string& ending) {
    if (string.length() >= ending.length()) {
        return (0 == string.compare(string.length() - ending.length(), ending.length(), ending));
    }
    else {
        return false;
    }
}

std::string FileUtils::getCurrentPath(){
    char currentPath[] = "./";
    currentPath[1] = kPathSeparator;
    std::string path (currentPath);
    return path;
}

std::list<std::string> FileUtils::getFilenamesInDrectory(std::string directoryPath, const char* ext){
    // Grab spectra filenames from input directory
    struct dirent *ent;
    std::list<std::string> lFileNames;
//    std::string path = (directoryPath == "") ? this->getCurrentPath() : directoryPath;
//    std::cout << "FileUtils: opening directory \"" << path << "\"" << std::endl;
    DIR *dir = opendir("./");
    if (dir != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            std::string filename(ent->d_name);
            std::cout << filename << std::endl;
            std::string extension = ext;
            if (this->stringEnds(filename, extension)) {
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