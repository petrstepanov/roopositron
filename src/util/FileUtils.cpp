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
#include <TSystemDirectory.h>
#include <TSystemFile.h>
#include <TList.h>
#include "../model/Constants.h"
#include "../util/StringUtils.h"
#include "../util/Debug.h"

TH1F* FileUtils::importTH1F(std::string filename, int i) {
	Constants* constants = Constants::getInstance();
	const int minChannel = constants->getMinChannel();
	const int maxChannel = constants->getMaxChannel();
	const int skipLines = constants->getSkipLines();
	const int bins = maxChannel - minChannel + 1;
	TH1F* histogram = new TH1F(TString::Format("histogram_%d", i+1), TString::Format("ROOT histogram %d", i+1), bins, 0, bins);

	// Open file
	std::ifstream inFile(filename.c_str());
	// Treminate if error
	if (!inFile) {
		std::cout << "File \"" << filename << "\" not found." << std::endl;
		exit(1);
	}

	// Skip spectrum header
	std::string sLine = filename;
	for (unsigned j = 0; j < skipLines; j++) {
		getline(inFile, sLine);
	}

	// Now `sLine` contains first line with data. Read spectrum data
	// Skip minChannel - 1 lines;
	for (unsigned j = 0; j < minChannel - 1; j++) {
		getline(inFile, sLine);
	}

	// Fill in histogram
	for (unsigned j = 1; j <= maxChannel - minChannel + 1; j++) {
		getline(inFile, sLine);
		std::istringstream streamLine(sLine);
		Double_t dCount;
		streamLine >> dCount;
		if (streamLine.fail()) {
			std::cout << "Failed parsing file." << std::endl;
			streamLine.clear();
			dCount = 0;
		}
		histogram->SetBinContent(j, dCount + 1); // The RooFit chi2 fit does not work when the bins have zero entries.
		histogram->SetBinError(j, sqrt(dCount + 1));
	}

	inFile.close();
	return histogram;
}

void FileUtils::createDirectory(std::string directoryName) {
	std::string createDirCommand = "mkdir " + directoryName;
	gSystem->Exec(createDirCommand.c_str());
}

std::string FileUtils::getCurrentPath() {
	char currentPath[] = "./";
	currentPath[1] = kPathSeparator;
	std::string path(currentPath);
	return path;
}

std::vector<std::string> FileUtils::getFilenamesInCurrentDrectory(const char* ext) {
	Debug("FileUtils::getFilenamesInCurrentDrectory", "Searching for \"" << ext << "\" files in current directory:");
	// Grab spectra filenames from input directory
	std::vector<std::string> lFileNames;
	TSystemDirectory *dir = new TSystemDirectory(".", gSystem->WorkingDirectory());
	if (TList *files = dir->GetListOfFiles()){
		TIter next(files);
		while (TSystemFile* file = (TSystemFile*)next()){
			TString fileName = file->GetName();
			if (!file->IsDirectory() && fileName.EndsWith(ext)) {
				std::cout << "* " << fileName << std::endl;
				std::string fN = fileName.Data();
				lFileNames.push_back(fN);
			} else {
				std::cout << "  " << fileName << std::endl;
			}
		}
	}
	return lFileNames;
}
