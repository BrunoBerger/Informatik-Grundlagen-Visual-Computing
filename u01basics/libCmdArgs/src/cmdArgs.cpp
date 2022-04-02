#include "cmdArgs.h"

#include <iostream>
#include <vector>

std::vector<int> getIntArgs(int argc, char* argv[]) 
{
    std::vector<int> v;
    for (int i=1; i<argc; i++)
    {   
        std::size_t pos{};
        try
        {
            v.push_back(std::stoi(argv[i], &pos));
        }
        catch (std::invalid_argument){}
    }
    std::cout << "Found " << v.size() << " int-args" << std::endl;
    return v;
}
