/*
 * FormulaVarsList.cpp
 *
 *  Created on: Jun 22, 2019
 *      Author: petrstepanov
 */

#include "HasIndirectParameters.h"
#include "../../util/Debug.h"

HasIndirectParameters::HasIndirectParameters(){
	indirectParametersList = new RooArgList();
}

HasIndirectParameters::~HasIndirectParameters() {}

void HasIndirectParameters::addIndirectParameter(RooFormulaVar* formulaVar){
	indirectParametersList->add(*formulaVar);
}

RooArgList* HasIndirectParameters::getIndirectParameters(){
	Debug("HasIndirectParameters::getIndirectParameters");
	#ifdef USEDEBUG
		indirectParametersList->Print();
	#endif
	return indirectParametersList;
}

