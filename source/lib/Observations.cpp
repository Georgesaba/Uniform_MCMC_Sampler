#include "Observations.hpp"
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

template<typename REAL>
void Observations<REAL>::loadData(std::string filename)
{
    //ifstream means input file stream
    std::ifstream filestream(filename);
    std::string line;
    while(std::getline(filestream, line))
    {
        // A string stream can be used for parsing strings with multiple pieces of data in it
        std::istringstream line_stream(line);
        REAL buffer;
        line_stream >> buffer;
        inputs.push_back(buffer);
        line_stream >> buffer;
        outputs.push_back(buffer);
        line_stream >> buffer;
        sigmas.push_back(buffer);
    }
}

template void Observations<double>::loadData(std::string);
template void Observations<float>::loadData(std::string);