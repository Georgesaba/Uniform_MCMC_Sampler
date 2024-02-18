#pragma once
#include <vector>
#include <string>
#include "Observations.hpp"
#include <memory>
#include <iostream>
#include <map>
#include <cmath>
#include <functional>

template <typename REAL>
REAL param_2_model_func(REAL x, std::array<REAL, 2> params){
    return params[0] * std::pow(x,params[1]);
}


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
    REAL min = 0;
    REAL max = 1;
    REAL width = max - min;
    std::string name;
};


/**
 * @brief Abstract class template that derived sampling classes inherit from. Contains pure virtual member function sample to be overwritten and has functionality to 
 * calculate and store log likelihood values calculated using a model function that can be passed in. Stores file data in an observations object.
 * 
 * Constructor arguments:
 * filepath - File that the observation class object reads the data from.
 * func - model function that is fitted to the data.
 * num_bins (optional) - number of bins that each parameter has.
 *
 * @tparam REAL: type representing real numbers; usually float or double.
 * @tparam num_params: number of parameters that are being used to fit the model.
*/
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

    const std::map<std::array<REAL,num_params>,REAL>& get_param_likelihood() const{
        return parameter_likelihood;
    }
    
    private:
    uint bins;
    std::array<ParamInfo<REAL>, num_params> params_info;
    std::function<REAL(REAL,std::array<REAL,num_params>)> model_function;
    
    protected:
    REAL log_likelihood(std::array<REAL, num_params> params){
        REAL sum_likelihood = 0;
        if (!observations){
            throw std::runtime_error("The pointer to the observations pointer has not beeen correctly initiated.");
        }
        const Observations<REAL>& obs = *observations;

        for (uint i = 0; i < obs.num_points; i++){
            REAL func_output = model_function(obs.inputs[i],params);
            sum_likelihood += -(func_output - obs.outputs[i]) * (func_output - obs.outputs[i]) /(2 * obs.sigmas[i] * obs.sigmas[i]);
        }
        return sum_likelihood;
    }
    std::unique_ptr<Observations<REAL>> observations;
    std::map<std::array<REAL,num_params>,REAL> parameter_likelihood; //  Dict for parameter vector and liklihood.
    
};


/**
 * @brief Derived class template inheriting from base Sampler class template that uses a grid search technique to calculate the log likelihood of every parameter combination.
 * Includes the overwritten sample member function that uses a the private member recursive function combination_gen to find every possible combination of paramters across an n parameter space.
 * 
 * Constructor arguments:
 * filepath - File that the observation class object reads the data from.
 * func - model function that is fitted to the data.
 * num_bins (optional) - number of bins that each parameter has.
 *
 * @tparam REAL: type representing real numbers; usually float or double.
 * @tparam num_params: number of parameters that are being used to fit the model.
*/

template<typename REAL, std::size_t num_params>
class UniformSampler : public Sampler<REAL, num_params>{
    public:
    UniformSampler(const std::string &filepath, const std::function<REAL(REAL,std::array<REAL,num_params>)> &func, uint num_bins = 100) : Sampler<REAL, num_params>(filepath, func, num_bins){}
    void sample() override {
        const std::array<ParamInfo<REAL>, num_params>& param_info = this -> get_params_info();
        uint num_bins = this -> get_bins();
        std::vector<uint> combination;
        combination_gen(combination, num_params, param_info, num_bins);
    }


    private:
    void combination_gen(std::vector<uint>& combination, uint n, const std::array<ParamInfo<REAL>, num_params>& param_info,uint num_bins ){
        if (n == 0){
            std::array<REAL,num_params> parameters; 
            for (std::size_t idx = 0; idx < num_params; idx++){
                parameters[idx] = param_info[idx].min + (combination[idx] + 0.5) * param_info[idx].width/num_bins;
            }
            REAL likelihood = this ->log_likelihood(parameters);
            this -> parameter_likelihood[parameters] = likelihood;
            return;
        }
        for (uint i = 0; i < num_bins; i++){
            combination.push_back(i);
            combination_gen(combination, n - 1,param_info, num_bins);
            combination.pop_back();
        }
    }
};