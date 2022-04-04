#include <iostream>

#include <armadillo>
#include "matrixMath.h"

arma::mat getRandomMat(int h, int w)
{
    arma::mat matr(h, w, arma::fill::randu);
    std::cout << "Generating matrix with " << matr.size() << " random elements \n";
    return matr;
}

