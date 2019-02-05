/*
 * MathUtil.cpp
 *
 *  Created on: Feb 4, 2019
 *      Author: student
 */

#include "MathUtil.h"

Int_t MathUtil::orderOfMagnitude(Double_t number) {
	Int_t mag = 0;

	while (number > 1) {
		mag++;
		number = number / 10;
	};

	return --mag;
}
