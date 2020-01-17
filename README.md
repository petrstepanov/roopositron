# RooPositron

A flexible positron lifetime fitting software. Supports the integration of custom models. The program is a console application. List of the implemented features:
* Implementation of custom fitting models (some c++ knowledge is required).
* In-depth control of the convolution operation. Change convolution bins number to balance fitting speed and precision.
* Fitting models support evaluation of the indirect parameters as functions of regular model parameters.
* Simultaneous fitting of multiple spectra with various channel widths.
* Plotting functionality and output of raster and vector plots.
* ASCII file output for plotting in other software.

The program is written in object-oriented C++ using CERN ROOT libraries. Fitting is performed via RooFit package functionality.

<figure>
 <img src="http://petrstepanov.com/static/screenshot-roopositron.png?123" alt="RooPositron desktop application" />
 <figcaption>Screenshot of the simultaneous fit of positron lifetime spectra made with RooPositron.</figcaption>
</figure> 

Following fitting models are supported:
* Multiexponential
* Standard Trapping model
* Diffusion Trapping model
* Dielectric powders model
* Liquids and solutions

Multiple spectra with various channel width can be fitted simultaneously.

## Installation on Linux or macOS

1. Install CERN ROOT on your system. Details can be found [here](https://medium.com/@petrstepanov/install-cern-root-roofit-on-macos-and-linux-for-dummies-df787fd41ef4).

2. Click on the "Clone or download" button on the top right. Extract the archive. Navigate to the extracted folder. In terminal run following:

```
make
sudo make install
```

## Installation on Windows

Not yet implemented.

## Users Guide

Once the program is installed, you will be able to run it in Terminal with the "roopositron" command. Program is currently a console application, user interface implementation is in progress [here](https://github.com/petrstepanov/roopositrongui).

### How to start the fit

RooPositron uses ORTEC's Maestro file format for the input spectra. Place your spectra .Spe files in a folder, navigate into it and run "roopositron". 

Application uses './constants.txt' located in the same folder, where spectrum properties and fitting directions are stored. For every fit RooPositron created a child folder with the following set of files:
* "parameters.txt" is a file containing all the parameters of the fit. These parameters include resolution function information, fitting model parameters and width of the channel for every spectrum.
* "fit-*.png" and "fit-*.root" are raster and vector images of the fit
* "data-*.txt" ASCII files generated from the plots with columns that allow plotting in other programs.

When launched, application checks the existence of the Constants file in the project folder. In case the file is missing the application with create Constants file with default values. Below find the list of variables in './constants.txt':

| Variable | Default value | Description |
| --- | --- | --- |
| `channels` | 8192 | Total number of channels to read from input files |
| `defaultChannelWidth` | 0.006186 | Default width of analyzer channel, in nanoseconds. In case of fitting multiple spectra, individual channel width for every spectrum can be set later in 'constants.txt' file. |
| `skipLines` | 12 | Number of lines to skip in input.Spe files. Usually, Maestro files have a header with some information. |
| `minChannel` | 600 | Trim spectra on the left by setting the minimum channel number for the fit. Please make sure the spectrum range [minChannel, minChannel+backgroundBins] is flat without humps. These bins are used for automatic background level evaluation. |
| `maxChannel` | 4000 | Trim spectra on the right end. |
| `excludeMinChannel` and `excludeMaxChannel` | 0 | Exclude a range of channels from the spectra fit and chi^2. Useful if experimental spectra have parasite humps. Set values to 0 to avoid excluding regions from a fit. |
| `minimizerType` | Minuit | Type of minimizer function to be used during the fit. Use one of the following values: `Minuit`, `Minuit2`, `Fumili`, `GSLMultiMin`. Learn more [here](https://root.cern/doc/master/classROOT_1_1Math_1_1Minimizer.html). |
| `decayModel` | exp | Name of the fitting model to be used. Following models are currently available: `exp`, `2exp`, `3exp`, `trapping`, `grain`, `powder`, `liquid`. It is possible to specify two comme-separated models together, e.g. `trapping,exp`. The resulting fitting model will be a linear combination of separate fitting models. |
| `resolutionFunction` | 2gauss | Number of Gaussians in resolution function. Currently support the following: `1gauss`, `2gauss` or `3gauss`. |
| `sourceComponents` | 1 | Number of exponential components in source contribution. Use integer values: `1`, `2`, `3`... For instance, if e+ source is sealed in Kapton, use 1 - one component. |
| `commonParameters` | ... | For multiple spectra fitting. A comma-separated list of fitting parameter names that are common for all spectra during the fit. By default, common parameters are number of bins in spectrum (bins), channel width (channelWidth), parameters of the resolution function (FWHM_gauss1, FWHM_gauss2, FWHM_gauss3, Int_gauss2, Int_gauss3), and source contribution decay parameters (#tau_source, Int_source, #tau1_source, #tau2_source, #tau3_source, Int_expSource2, Int_expSource3). |
| `bufferFraction` | 0.2 | Convolution operation parameter Default value is 20% of the spectrum width. Increase this value if the tail of your spectrum does not approach the background level. |
| `convolutionBins` | 1024 | Binning of the the convolution observable. Affects the sampling density of the FFT convolution operation. Increase to 2048 or 4096 if working with shorter lifetimes or higher channel width. |
| `backgroundBins` | 150 | Number of first spectrum bins used for background level estimation. Set to 0 to make background free during the fit. Fitting works best with the fixed automatic background. |
| `asciiDelimeter` | \t | Column delimiter used in ASCII plot output files. C++ escaped. The default is tab. |
| `imageWidth` | 1200 | Width of the raster plot images. |
| `imageHeight` | 800 | Height of the raster plot images. |

If the value of a parameter is a combination of comma-separated items, make sure that there are no spaces between them. Any missing parameter in the './constants.txt' file it will be replaced by the default one on the first program run.

### Integrated fitting models

Currently, RooPositron supports a number of conventional fitting models. Below please find model descriptions along with their fitting parameter names and descriptions. On order to use a certain fitting model please set the `decayModel` variable value in './parameters.txt' to a certain model's identifier. Additionally, you can specify a comma-separated list of models' identifiers as a value of `decayModel` parameter. For instance, `exp,exp,exp,exp` would give a four-component exponential decay. A complete description of the fitting models will be published in my thesis in May 2020.

| Model name | Identifier | Description |
| --- | --- | --- |
| **Mono-exponential** | `exp` | For fitting spectra from reference defect-free samples. |
| **Two-exponential** | `2exp` | For materials saturated with one type of defects. Trapping speed is supposed to be very high. |
| **Three-exponential** | `3exp` | Three-exponential decay. Conventional deconvolution. |
| **Mult-exponential** | `exp,exp,exp,exp` | Multi-exponential deconvolution. Please specify as many comma-separated 'exp' entries as you need. |
| **Trapping model** | `stm` | One state trapping model. |
| **Diffusion-trapping model** | `dtm` | For grained materials with point defects. The model accounts for: positron annihilation in bulk; e+ trapping into the vacancies and annihilation therein; e+ diffusion towards the grain boundaries; trapping and annihilation from intergrain space. |
| **Powder model** | `powder` | For dielectric nanopowders. The model accounts for: e+ annihilation in bulk of crystallites; e+ trapping and annihilation in vacancies within crystallites; Ps formation and escape outside of the crystallites; Ps thermalization and annihilation in inter-crystalline space. |
| **Two-state powder model** | `powder2` | **Powder model** with additional bounded state of the Ps atom on the powder surface. |
| **Liquids and solutions** | `liquid` | For liquids and liquid solutions. The model accounts for: formation of qf-Ps and its annihilation; transformation of qf-Ps into a bubble state; pick-off annihilation of Ps from bubble state; possible reactions of Ps with solvents and intratrack radicals (oxidation and ortho-para conversion). |

Every model accounts for an extra contribution of the annihilation of positrons in the source material.

### How to adjust model fitting parameters

 Once program constants are defined we move on to setting the default model parameters for the fit. Run RooPositron from the Terminal. It will prompt to enter the starting parameters selected model. Input values using the keyboard. 

Alternatively, you can interrupt the program by `CTRL+C` and navigate to the automatically generated `parameters.txt` file in a child folder. Use any text editor to open the `parameters.txt`. Set starting values, limits and select certain parameters to be `fixed` or `free` during the fit. Using a text editor is much quicker rather than entering model values from the keyboard. Especially when working with models that have many parameters.

 <figure>
 <img src="http://petrstepanov.com/static/screenshot-roopositron-parameters.png?123" alt="Editing parameters of the RooPoritron fit" style="width: 100%;"/>
 <figcaption>Editing fitting parameter values and limits in plain text editor.</figcaption>
</figure> 

### How to implement a custom fitting Model

To implement a custom fitting model it is needed to have some object-oriented C++ knowledge. We have to create two classes. One will be responsible for the model equation and optional analytical integral. Another will provide default values for model parameters as well as formulas for the indirect model parameters calculated after the fit. Finally, we will have to come up with the new model identifier (name) and map the model with its identifier in the ModelProvider class.

#### Defining the Model equation

Extend a base `RooAbsPdf` class to define the model parameters and equation. Create your `CustomModel.cpp` and `CustomModel.h` files. Conventional location for model classes is `./src/roofit/pdfs/`. Apparently your `CustomModel` filename has to be unique. Override `RooAbsPdf::evaluate()` function and return your model equation there.

Optionally in order to dramatically improve the convolution speed please override `RooAbsPdf::getAnalyticalIntegral()` and `RooAbsPdf::analyticalIntegral()`. 

Since lifetime spectra are one-dimensional and integration is carried out along one variable, just return `1` in `getAnalyticalIntegral`. For a given range provided as an `analyticalIntegral()` function argument you have to return the value of the definite integral of your model equation. 

It is reasonable to calculate the definite integral using the Newton-Leibniz formula. If you refer to the existing models you will see that `analyticalIntegral()` simply does `return indefiniteIntegral(x2) - indefiniteIntegral(x1);`. An indefinite integral is evaluated in `indefiniteIntegral()` function of your model class.

It is reasonable to use computational software (we used a Wolfram Mathematica in trial mode) to calculate the indefinite integrals for models' equations. Mathematica's `CForm[%]` command outputs any expression as C++ code. So user can copy expression from Mathematica and directly paste it in C++ `indefiniteIntegral()` function. Examples of Mathematica scripts that calculate indefinite integrals for already implemented models are located in `./wolfram` folder.

#### Specifying default Model parameter values

Next, implement a wrapper class for your custom model. Conventionally we name them `CustomModelProvider.class` and they are located in `./src/roofit/providers/`. In RooFit every function variable is an object. So the wrapper instantiates model parameter objects with default values. Next wrapper passes them to the Model and returns it.

Optionally if you want your model to calculate some indirect parameters please override `getIndirectParameters()` method in your `CustomModelProvider` class. The list of formulas (`RooFormulaVar` objects) you return in this function will be displayed in the legend on the plots along with regular model parameters.

#### Adding Model identificator

Lastly in order to be able to use our model we need to give it an identifier (char string used in './constants.txt'). In order to do so modify the `PdfFactory` class. First, in `PdfFactory.h` add a new value to the `PdfHashNames` enumeration.

Come up with a model identifier and map it to the enumeration in `PdfFactory::hashPdfName`. Add a new case for the switch statement in `PdfFactory::getPdf()` and return your new `CustomModel` object.

If your model has indirect parameters, add a new case to the switch statement in `PdfFactory::getIndirectParameters` and return the list of your indirect parameters.

### How to contribute

Contributors are welcome. Please clone the repo, make your changes and submit the Pull Request on GitHub. Feel free to contact me with questions about RooPositron.