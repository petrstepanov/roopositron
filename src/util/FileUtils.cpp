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
#include <TObjString.h>
#include <TSystemFile.h>
#include <TList.h>
#include <RooPlot.h>
#include <RooHist.h>
#include <RooCurve.h>
#include <RooAbsBinning.h>
#include "TMath.h"
#include "TMatrixD.h"
#include "../model/Constants.h"
#include "StringUtils.h"
#include "RootHelper.h"
#include "Debug.h"

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
		files->Sort();
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

void FileUtils::savePlotsToFile(RooPlot* spectrumPlot, RooPlot* residualsPlot, const char* fileName, RooRealVar* observable, Double_t conversionValue, Double_t zeroChannel, const char* conversionUnit){
	std::pair<TMatrixD,TList*> spectrumMatrixAndHeader = RootHelper::rooPlotToMatrix(observable, spectrumPlot, conversionValue, zeroChannel, conversionUnit);
	std::pair<TMatrixD,TList*> residualsMatrixAndHeader = RootHelper::rooPlotToMatrix(observable, residualsPlot);
	TMatrixD spectrumMatrix = (spectrumMatrixAndHeader.first);
	TMatrixD residualsMatrix = (residualsMatrixAndHeader.first);

	Int_t numberOfRows = TMath::Min(spectrumMatrix.GetNrows(), residualsMatrix.GetNrows());
	Int_t numberOfColumns = spectrumMatrix.GetNcols() + residualsMatrix.GetNcols();

	TMatrixD matrix(numberOfRows, numberOfColumns);
	for (Int_t j=0; j<numberOfRows; j++){
		for (Int_t i=0; i<numberOfColumns; i++){
			if (i<spectrumMatrix.GetNcols()){
				matrix(j,i) = spectrumMatrix(j,i);
			}
			else {
				matrix(j,i) = residualsMatrix(j,i-spectrumMatrix.GetNcols());
			}
		}
	}

	std::ofstream outputFile;
	outputFile.open(fileName);

	// Print header names to file
	Constants* constants = Constants::getInstance();
	std::string delimeter = StringUtils::unescape(constants->getDelimeter());

	TList* columnNames = new TList();
	columnNames->AddAll(spectrumMatrixAndHeader.second);
	columnNames->AddAll(residualsMatrixAndHeader.second);
//		TIterator* it = columnNames->MakeIterator();
//		while (TObject* temp = it->Next()) {
//			if (TObjString* str = dynamic_cast<TObjString*>(temp)){
//				if (it != columnNames->end()){
//					outputFile << (str->String()).Data() << delimeter.c_str();
//				} else {
//					outputFile << (str->String()).Data();
//				}
//			}
//		}
	for (Int_t i=0; i<columnNames->GetSize(); i++){
		TObject* object = columnNames->At(i);
		if (TObjString* str = dynamic_cast<TObjString*>(object)){
			outputFile << (str->String()).Data();
		}
		if (i != columnNames->GetSize()-1){
			outputFile << delimeter.c_str();
		}
	}

	outputFile << std::endl;

	// Print matrix to file
	for (Int_t j=0; j<numberOfRows; j++){
		for (Int_t i=0; i<numberOfColumns; i++){
			if (i != numberOfColumns-1){
				outputFile << matrix(j,i) << delimeter.c_str();
			} else {
				outputFile << matrix(j,i);
			}
		}
		outputFile << std::endl;
	}
}
