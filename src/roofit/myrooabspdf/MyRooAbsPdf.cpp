/*
 * MyRooAbsPdf.cpp
 *
 *  Created on: Jun 23, 2019
 *      Author: petrstepanov
 */

#include "MyRooAbsPdf.h"
ClassImp(MyRooAbsPdf);

MyRooAbsPdf::MyRooAbsPdf() {
}

MyRooAbsPdf::MyRooAbsPdf(const char *name, const char *title) : RooAbsPdf(name,title){
}

MyRooAbsPdf::MyRooAbsPdf(const MyRooAbsPdf& other, const char* name) : RooAbsPdf(other,name){
}

MyRooAbsPdf::~MyRooAbsPdf() {
	// TODO Auto-generated destructor stub
}

