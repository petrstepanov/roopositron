# RooPositron

A flexible terminal-based positron lifetime fitting software. Supports integration of custom models.

Program is written in object-oriented C++ using CERN ROOT libraries. Fitting is performed via RooFit package functionality.

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
make install
```

## Installation on Windows

Not yet implemented. Contributors that familiar with CMAKE are welome.

## Users Guide

Program can be called by typing `roopositron` in terminal. Launch program in a directory that contains a set of Maestro .Spe files.
