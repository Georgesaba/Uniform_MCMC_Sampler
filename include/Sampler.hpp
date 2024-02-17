#pragma once
#include <vector>
#include <string>
#include "Observations.hpp"
#include <memory>
#include <iostream>
#include <map>
#include <cmath>
#include <functional>

/**
 * @brief Class for holding / passing information about parameters to be sampled
 * 
 * @tparam REAL: type representing real numbers; usually float or double.  
 */
#pragma once

template <typename REAL>
REAL param_2_model_func(REAL x, std::array<REAL, 2> params){
    return params[0] * std::pow(x,params[1]);
}


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

template<typename REAL, std::size_t num_params>
class Sampler
{
    public:
    Sampler(const std::string &filepath, const std::function<REAL(REAL,std::array<REAL,num_params>)> &func, uint num_bins = 100) : bins(num_bins) , model_function(func){
        observations = std::make_unique<Observations<REAL>>();
        observations->loadData(filepath);
    }
    virtual ~Sampler() = default;
    void set_param_info(std::array<std::string,num_params> names, std::array<REAL,num_params> min_values, std::array<REAL, num_params> max_values){
        for (std::size_t i = 0; i < num_params;i++){
            params_info[i].min = min_values[i];
            params_info[i].max = max_values[i];
            params_info[i].name = names[i];
        }
    }
    
    REAL log_likelihood(std::array<REAL, num_params> params){
        REAL sum_likelihood = 0;
        if (!observations){
            throw std::runtime_error("The pointer to the observations pointer has not beeen correctly initiated.")
        }
        Observations<REAL> obs = *observations;

        for (uint i = 0; i < obs.num_points; i++){
            sum_likelihood += -pow(model_function(obs.inputs[i],params) - obs.outputs[i],2)/(2 * pow(obs.sigmas[i],2))
        }
        return sum_likelihood;
    }
    virtual void sample() = 0;
    uint get_bins() const {
        return bins;
    }
    const std::array<ParamInfo<REAL>, num_params>& get_params_info() const{
        return params_info;
    }
    void set_bins(uint num_bins){
        bins = num_bins;
    }
    private:
    uint bins;
    std::array<ParamInfo<REAL>, num_params> params_info;
    std::function<REAL(REAL,std::array<REAL,num_params>)> model_function;
    
    protected:
    //REAL (*model_function)(REAL, std::array<REAL, num_params>) = nullptr; potential way to store function with raw pointer
    std::unique_ptr<Observations<REAL>> observations;
    std::map<std::array<REAL,num_params>,REAL> parameter_likelihood; //  Dict for parameter vector and liklihood.
    
};


template<typename REAL, std::size_t num_params>
class UniformSampler : public Sampler<REAL, num_params>{
    public:
    UniformSampler(const std::string &filepath, const std::function<REAL(REAL,std::array<REAL,num_params>)> &func, uint num_bins) : Sampler<REAL, num_params>(filepath, func, num_bins){}
    void sample() override {}
};