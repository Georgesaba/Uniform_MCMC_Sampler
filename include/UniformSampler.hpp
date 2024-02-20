#pragma once
#include "Sampler.hpp"

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
    UniformSampler(const std::string &filepath, const std::function<REAL(REAL,std::array<REAL,num_params>&)> &func,std::array<std::string,num_params> names, std::array<REAL,num_params> min_values, std::array<REAL, num_params> max_values, uint num_bins = 100, const bool rigidity = false) : Sampler<REAL, num_params>(filepath, func,names, min_values, max_values, num_bins, rigidity){}
    void sample() override {
        if (this -> been_sampled){
            std::cerr << "Error - Procedure aborted as UniformSampler instance has already sampled the data points." << std::endl;
        }

        const std::array<ParamInfo<REAL>, num_params>& param_info = this -> get_params_info();
        uint num_bins = this -> get_bins();
        std::vector<uint> combination;
        combination_gen(combination, num_params, param_info, num_bins);
        normalise_marginal_distribution();
        this -> been_sampled = true;
    }

    private:
    void normalise_marginal_distribution(){
        for (std::size_t i = 0; i < num_params; i++){
            REAL total_prob = std::accumulate(this -> marginal_distribution[i].begin(), this -> marginal_distribution[i].end(),0.0);//sum of marginal distribution must equal 1
            for (REAL &num: this -> marginal_distribution[i]){
                num /= total_prob;
            }
        }
    }

    void combination_gen(std::vector<uint>& combination, uint n, const std::array<ParamInfo<REAL>, num_params>& param_info,uint num_bins ){
        if (n == 0){
            std::array<REAL,num_params> parameters; 
            for (std::size_t idx = 0; idx < num_params; idx++){
                parameters[idx] = param_info[idx].min + (combination[idx] + 0.5) * param_info[idx].width/num_bins;
            }
            REAL lg_likelihood = this ->log_likelihood(parameters);
            //std::cout << lg_likelihood << std::endl;
            this -> parameter_likelihood[parameters] = lg_likelihood;
            REAL likelihood = std::exp(lg_likelihood);
            //std::cout << likelihood << std::endl; /// debugging
            for (std::size_t j = 0; j < num_params; j++){
                this -> marginal_distribution[j][combination[j]] += likelihood;
            }
            return;
        }
        for (uint i = 0; i < num_bins; i++){
            combination.push_back(i);
            combination_gen(combination, n - 1,param_info, num_bins);
            combination.pop_back();
        }
    }
};