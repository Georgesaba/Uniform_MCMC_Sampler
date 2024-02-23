#pragma once
#include "Sampler.hpp"
#include <random>

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
        num_sample_points = sample_points;
        step_size = step_s;
    }

    void sample() override {
        if (this -> been_sampled){
            std::cerr << "Error - Procedure aborted as this MetropolisHastingsSampler instance has already sampled the data points." << std::endl;
        }
        const std::array<ParamInfo<REAL>, num_params>& params_info = this -> get_params_info();
        std::default_random_engine generator(42);
        std::array<REAL,num_params> unit_hypercube;
        std::array<REAL, num_params> new_unit_hypercube;
        std::array<REAL, num_params> params;
        std::array<REAL, num_params> new_params;
        uint number_bins = this -> get_bins();
        std::uniform_real_distribution<REAL> initial_dist(0,1);
        std::normal_distribution<REAL> step_dist(0,step_size);
        REAL bin_width;
        uint bin_number;
        REAL lg_likelihood;
        for (std::size_t i = 0; i < num_params; i++){
            unit_hypercube[i] = initial_dist(generator);
            params[i] = params_info[i].min + unit_hypercube[i] * params_info[i].width;
            bin_width = params_info[i].width/number_bins;
            bin_number = static_cast<uint>(std::floor(params[i]/bin_width));
            this -> marginal_distribution[i][bin_number]++;
        }
        this -> parameter_likelihood[params] = this -> log_likelihood(params);
        for (uint j = 0; j < num_sample_points; j++){
            for (std::size_t i = 0; i < num_params; i++){
                new_unit_hypercube[i] = unit_hypercube[0] + step_dist(generator);
                if (new_unit_hypercube[i] > 1){
                    new_unit_hypercube[i]--;
                }
                else if (new_unit_hypercube[i] < 0){
                    new_unit_hypercube[i]++;
                }
                new_params[i] = params_info[i].min + new_unit_hypercube[i] * params_info[i].width;
            }

            //use acceptance criterion
            lg_likelihood = this -> log_likelihood(new_params);
            if (lg_likelihood >= this -> parameter_likelihood[params]){
                params = new_params;
                unit_hypercube = new_unit_hypercube;
                this -> parameter_likelihood[new_params] = lg_likelihood;
            }
            else{
                //std::cout << (lg_likelihood - this -> parameter_likelihood[params]) << " : " << std::log(initial_dist(generator)) << std::endl;
                if ((lg_likelihood - this -> parameter_likelihood[params]) > std::log(initial_dist(generator))){
                    params = new_params;
                    unit_hypercube = new_unit_hypercube;
                    this -> parameter_likelihood[new_params] = lg_likelihood;
                }
                else{
                    new_params = params;
                    new_unit_hypercube = unit_hypercube;
                }
            }

            //std::cout << "(";
            for (std::size_t i = 0; i < num_params; i++){
                bin_width = params_info[i].width/number_bins;
                bin_number = static_cast<uint>(std::floor((params[i] - params_info[i].min)/bin_width));
                this -> marginal_distribution[i][bin_number]++;
                //std::cout << params[i] << ", ";
            }
            //std::cout << rank << ")" << std::endl;
        }
        this -> normalise_marginal_distribution();
        this -> been_sampled = true;
    }

    private:
    uint num_sample_points;
    REAL step_size;
};