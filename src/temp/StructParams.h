/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   StructParams.h
 * Author: petrstepanov
 *
 * Created on November 14, 2016, 4:12 PM
 */

#ifndef STRUCTPARAMS_H
#define STRUCTPARAMS_H

#include <tuple>

struct StructParams {
    double lambdaJ;
    double lambdaBulk;
    double lambdaGrain;
    double lambdaVac;
    double kappaVac;
    StructParams(double _lambdaJ, double _lambdaBulk, double _lambdaGrain,
            double _lambdaVac, double _kappaVac) : lambdaJ(_lambdaJ), lambdaBulk(_lambdaBulk),
            lambdaGrain(_lambdaGrain), lambdaVac(_lambdaVac), kappaVac(_kappaVac){ }
    // Overriding operators inside the class, otherwise symbols error (linking)
    bool operator<(const StructParams& y) const {
        return std::tie(lambdaJ, lambdaBulk, lambdaGrain, lambdaVac, kappaVac) < std::tie(y.lambdaJ, y.lambdaBulk, y.lambdaGrain, y.lambdaVac, y.kappaVac);
    }
    bool operator==(const StructParams& y) const {
        return std::tie(lambdaJ, lambdaBulk, lambdaGrain, lambdaVac, kappaVac) == std::tie(y.lambdaJ, y.lambdaBulk, y.lambdaGrain, y.lambdaVac, y.kappaVac);
    }
};

//bool operator<(const StructParams& x, const StructParams& y) {
//    return std::tie(x.lambdaJ, x.lambdaBulk, x.lambdaGrain, x.lambdaVac, x.kappaVac) < std::tie(y.lambdaJ, y.lambdaBulk, y.lambdaGrain, y.lambdaVac, y.kappaVac);
//}

//bool operator ==(const StructParams& x, const StructParams& y) {
//    return std::tie(x.lambdaJ, x.lambdaBulk, x.lambdaGrain, x.lambdaVac, x.kappaVac) == std::tie(y.lambdaJ, y.lambdaBulk, y.lambdaGrain, y.lambdaVac, y.kappaVac);
//}

#endif /* STRUCTPARAMS_H */

