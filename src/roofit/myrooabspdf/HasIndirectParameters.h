/*
 * FormulaVarsList.h
 *
 *  Created on: Jun 22, 2019
 *      Author: petrstepanov
 */

#ifndef SRC_ROOFIT_HASINDIRECTPARAMETERS_H_
#define SRC_ROOFIT_HASINDIRECTPARAMETERS_H_

#include <RooArgList.h>
#include <RooAbsPdf.h>
#include <RooFormulaVar.h>

class HasIndirectParameters {
public:
	HasIndirectParameters();
	virtual ~HasIndirectParameters();

	void addIndirectParameter(RooFormulaVar* formula);
	RooArgList* getIndirectParameters();

private:
	RooArgList* indirectParametersList;
};

#endif /* SRC_ROOFIT_HASINDIRECTPARAMETERS_H_ */
