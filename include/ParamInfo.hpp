#pragma once
#include <string>

/**
 * @brief Class for holding / passing information about parameters to be sampled
 * 
 * @tparam REAL: type representing real numbers; usually float or double.  
 */

template <typename REAL>
class ParamInfo
{
public:
    ParamInfo() = default;
    ParamInfo(REAL min, REAL max, std::string name) : min(min), max(max), name(name)
    {
        width = max - min;
    }
    void set_values(REAL min, REAL max, std::string name)  { // if has already been initialised
        this ->min = min;
        this -> max = max;
        this -> name = name;
        this -> width = max - min;
    }
    REAL min = 0;
    REAL max = 1;
    REAL width = max - min;
    std::string name;

    REAL marginal_distribution_peak;
    REAL mean_parameter;
    REAL standard_deviation;    
};