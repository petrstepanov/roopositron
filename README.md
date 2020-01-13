# RooPositron

A flexible terminal-based positron lifetime fitting software. Supports integration of custom models. Program works in console. List of implemented feature:
* Implementation of custom fitting models (some c++ knowledge is required).
* In-depth control of the convolution operation. Change convolution bins number to balance fitting speed and precision.
* Fitting models support evaluation of the indirect parameters as functions of regular model parameters.
* Simultaneous fitting of multiple spectra with various channel widths.
* Plotting functionality and output of raster and vector plots.
* ASCII file output for plotting in other software.

Program is written in object-oriented C++ using CERN ROOT libraries. Fitting is performed via RooFit package functionality.

<img src="http://petrstepanov.com/static/screenshot-roopositron.png?123" alt="RooPositron desktop application" style="width: 100%;"/>

Following fitting models are supported:
* Multiexponential
* Trapping model
* Grain boundary model
* Model in liquids accounting on o-Ps oxidation and o-p conversion

Multiple spectra with various channel width can be fitted simultaneously.

## Installation on Linux or macOS

1. Install CERN ROOT on your system. Details can be found [here](https://medium.com/@petrstepanov/install-cern-root-roofit-on-macos-and-linux-for-dummies-df787fd41ef4).

2. Click on "Clone or download" button on the top right. Extract the archive. Navigate to the extracted folder. In terminal run following:

```
make
sudo make install
```

## Installation on Windows

Not yet implemented.

## Users Guide

Once the program is installed, you will be able to run it in Terminal with "roopositron" command. Program is currently a console application, user interface implementation is in progress [here](https://github.com/petrstepanov/roopositrongui).

### How to start the fit

RooPositron uses ORTEC's Maestro file format for the input spectra. Place your .Spe files in a folder, navidate into it and run "roopositron". 

Application uses './constants.txt' located in the same folder, where spectrum properties and fitting directions are stored. For every fit RooPositron created a child folder with following set of files:
* "parameters.txt" is a file cointaining all the parameters of the fit. These parameters include resolutiuon function information, fitting model parameters and width of the channel for every spectrum.
* "fit-*.png" and "fit-*.root" are raster and vector images of the fit
* "data-*.txt" ASCII files generated from the plots with columns that allow plotting in other programs.

First thing the application checks if Constants file is present in current folder. In case the file is missing the application with create Constants file with default values. Below find the list of constants:

**channels**
Total number of bins to read from input files. Default value is 8192.

**defaultChannelWidth**
Default width of analyzer channel, unit is nanoseconds. In case of fitting multiple spectra, individual channel width for every spectrum can be set later in 'parameters.txt' file. Default value is 0.006186.

**skipLines**
Number of lines to skip in input .Spe files. Usually Maestro files have header with some information. Default value is 12.

**minChannel**
Trim spectra on the left by setting the minimum channel number for the fit. Please make sure the spectrum range [minChannel, minChannel+backgroundBins] is flat without humps. These bins are used for automatic background level evaluation. Default value is 600.

**maxChannel**
Trim spectra on the right end. Default value is 4000.

**excludeMinChannel** and **excludeMaxChannel**
Exclude a range of channels from the spectra fit and chi^2. Useful if experimantal spectra have parasyte humps. Set values to 0 to avoid excluding refions from fit. Default values are 0.

**minimizerType**
Type of minimizer function used during the fit. Use one of the following values: "Minuit", "Minuit2", "Fumili", "GSLMultiMin". Default value is "Minuit". Learn more here 

**decayModel**
Comma-separated list of names of the fitting models to be used. Following models are currently available: "exp", "2exp", "3exp", "trapping", "grain", "powder", "liquid". If specified two or more models, e.g. "trapping,exp" final fitting model will be a linear combination of separate fitting models. Default value is "exp" - one exponent.

**resolutionFunction**
Number of Gaussians in resolution function. Currently support following: "1gauss", "2gauss" or "3gauss". Default value is "2gauss".

**sourceComponents**
Number of exponential components in source contribution. Use integer values: 1, 2, 3... For instance if e+ source is sealed in Kapton, use 1 - one component.

**commonParameters**
For multiple spectra fitting. Comma-separated list of fitting parameter names that have to be common across the fit. By default, common parameters are number of bins in spectrum (bins), channel width (channelWidth), parameters of the resolution function (FWHM_gauss1,FWHM_gauss2,FWHM_gauss3,Int_gauss2,Int_gauss3), and source contribution decay parameters (#tau_source,Int_source,#tau1_source,#tau2_source,#tau3_source,Int_expSource2,Int_expSource3).

**bufferFraction**
Convolution operation parameter Default value is 0.2 (20% of the spectrum width). Increase this value if the tail of your spectrum does not reach the background level.

**convolutionBins***
Binning of the the convolution observable. Affects the sampling density of the FFT convolution operation. Default value is 1024. Increase to 2048 or 4096 if working with shorter lifetimes or higher channel width.

**backgroundBins**
Number of first spectrum bins used for background level estimation. Default value is 150. Set to 0 to make background free during the fit. This is not recommended though.

**asciiDelimeter**
Desired coilumn delimeter used in ASCII plot output files.

**imageWidth**
Width of the raster plot images.

**imageHeight**
Height of the raster plot images.

### How to adjust fitting parameters

Please refer to the "parameters.txt" file generated in a correspondent subfolder after the fit is complete. 

### How to implement a custom fitting model