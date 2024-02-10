#include <vector>
#include <string> 

/**
 * @brief Class for holding / passing information about parameters to be sampled
 * 
 * @tparam REAL: type representing real numbers; usually float or double.  
 */
#pragma once

template <typename REAL>
class ParamInfo
{
public:
    ParamInfo() = default;
    ParamInfo(REAL min, REAL max, std::string name) : min(min), max(max), name(name)
    {
        width = max - min;
    }
    REAL min = 0;
    REAL max = 1;
    REAL width = max - min;
    std::string name;
};

template<typename REAL>
class Sampler
{
    
};