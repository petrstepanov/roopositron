/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ThreeGaussian.cpp
 * Author: petrstepanov
 * 
 * Created on November 9, 2016, 7:42 PM
 */

#include "ThreeGaussian.h"

ThreeGaussian::ThreeGaussian(const char *name, const char *title,
	RooAbsReal& _x,
	RooAbsReal& _mean1,
	RooAbsReal& _sigma1,
	RooAbsReal& _mean2,
	RooAbsReal& _sigma2,
	RooAbsReal& _sigma3,        
	RooAbsReal& _i2,
        RooAbsReal& _i3) :
	RooAbsPdf(name, title),
	x("x", "Observable", this, _x),
	mean1("mean1", "Mean1", this, _mean1),
	sigma1("sigma1", "Width1", this, _sigma1),
	mean2("mean2", "Mean2", this, _mean2),
	sigma2("sigma2", "Width2", this, _sigma2),
	sigma3("sigma3", "Width3", this, _sigma3),
	i2("i2", "I2", this, _i2),
	i3("i3", "I3", this, _i3)
{
}

ThreeGaussian::ThreeGaussian(const ThreeGaussian& other, const char* name) :
RooAbsPdf(other, name), x("x", this, other.x), mean1("mean1", this, other.mean1),
sigma1("sigma1", this, other.sigma1), mean2("mean2", this, other.mean2),
sigma2("sigma2", this, other.sigma2), sigma3("sigma3", this, other.sigma3), i2("i2", this, other.i2), i3("i3", this, other.i3)
{
}

Double_t ThreeGaussian::evaluate() const
{
	Double_t pi = 3.14159;

	Double_t arg1 = x - mean1;
	Double_t ret1 = exp(-0.5*arg1*arg1 / (sigma1*sigma1)) / sigma1 / sqrt(2 * pi);

	Double_t arg2 = x - mean2;
	Double_t ret2 = exp(-0.5*arg2*arg2 / (sigma2*sigma2)) / sigma2 / sqrt(2 * pi);

	Double_t ret3 = exp(-0.5*arg1*arg1 / (sigma3*sigma3)) / sigma3 / sqrt(2 * pi);        
        
	return (1.-i2-i3)*ret1 + i2*ret2 + i3*ret3;
}