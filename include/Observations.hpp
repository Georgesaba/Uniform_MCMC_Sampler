#pragma once

#include <vector>
#include <string>

/**
* Class for holding results of obersvations in three lists
* for inputs / outputs / standard deviations.
**/
template<typename REAL>
class Observations
{
public:
    std::vector<REAL> inputs;
    std::vector<REAL> outputs;
    std::vector<REAL> sigmas;

    void loadData(std::string filename);
};