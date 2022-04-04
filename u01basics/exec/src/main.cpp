
#include <iostream>
#include <vector>

#include <armadillo>
#include "cmdArgs.h"
#include "matrixMath.h"

int main(int argc, char* argv[])
{
    std::vector<int> args = getIntArgs(argc, argv);

    arma::mat matr1 = getRandomMat(args[0], args[1]);
    arma::mat matr2 = getRandomMat(args[1], args[0]);
    auto res = matr1 * matr2;
    std::cout << matr1 << " * " << matr2 << " = " << res << std::endl;
}
