#pragma once
#include "Sampler.hpp"

/**
 * @brief
 * 
 * 
 * @tparam REAL:
 * 
 * @tparam num_params: 
 * 
*/

template<typename REAL, std::size_t num_params>
class MetropolisHastingSampler : public Sampler<REAL, num_params>{
    public:
    MetropolisHastingSampler(const std::string &filepath, const std::function<REAL(REAL,std::array<REAL,num_params>&)> &func,
    std::array<std::string,num_params> names, std::array<REAL,num_params> min_values, std::array<REAL, num_params> max_values,
    uint sample_points = 100000, REAL step_s = 0.01,
    uint num_bins = 100, const bool rigidity = false) : Sampler<REAL, num_params>(filepath, func,names, min_values, max_values, num_bins, rigidity)
    {
        num_sample_points(sample_points);
        step_size(step_s);
    }

    void sample(){}

    void sample_loop(){}

    private:
    uint num_sample_points;
    REAL step_size;
};