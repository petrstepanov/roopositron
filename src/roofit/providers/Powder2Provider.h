/*
 * Powder2Provider.h
 *
 *  Created on: Feb 8, 2019
 *      Author: student
 */

#ifndef SRC_ROOFIT_PROVIDERS_POWDER2PROVIDER_H_
#define SRC_ROOFIT_PROVIDERS_POWDER2PROVIDER_H_

#include "AbstractProvider.h"
#include "RooRealVar.h"

class Powder2Provider : public AbstractProvider{
public:
    Powder2Provider(RooRealVar *observable);
    virtual ~Powder2Provider();

private:
    RooAbsPdf* initPdf(int i);
};

#endif /* SRC_ROOFIT_PROVIDERS_POWDER2PROVIDER_H_ */
