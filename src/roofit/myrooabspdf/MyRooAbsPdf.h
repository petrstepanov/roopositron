/*
 * MyRooAbsPdf.h
 *
 *  Created on: Jun 23, 2019
 *      Author: petrstepanov
 */

#ifndef SRC_ROOFIT_MYROOABSPDF_H_
#define SRC_ROOFIT_MYROOABSPDF_H_

#include <RooAbsPdf.h>
#include "HasIndirectParameters.h"

// MyRooAbsPdf has a list of indirect parameters

class MyRooAbsPdf: public RooAbsPdf, public HasIndirectParameters {
public:
	MyRooAbsPdf();
	MyRooAbsPdf(const char *name, const char *title);
	MyRooAbsPdf(const MyRooAbsPdf& other, const char* name);

	virtual ~MyRooAbsPdf();

	ClassDef(MyRooAbsPdf,1)
};

#endif /* SRC_ROOFIT_MYROOABSPDF_H_ */
