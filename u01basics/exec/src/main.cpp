
#include <iostream>
#include <vector>

#include <armadillo>
#include "cmdArgs.h"

int main(int argc, char* argv[])
{
    std::cout << "Welcome to Matrix Contructor" << std::endl;

    std::vector<int> args = getIntArgs(argc, argv);

    arma::mat matr(args[0], args[1], arma::fill::randu);
    std::cout << matr << std::endl;
}
