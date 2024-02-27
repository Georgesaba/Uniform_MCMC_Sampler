#pragma once
#include "Sampler.hpp"

/**
 * @brief Derived class template inheriting from base Sampler class template that uses a grid search technique to calculate the log likelihood of every parameter combination.
 * Includes the overwritten sample member function that uses a the private member recursive function combination_gen to find every possible combination of paramters across an n parameter space.
 * @tparam REAL: type representing real numbers; usually float or double.
 * @tparam num_params: number of parameters that are being used to fit the model.
*/
template<typename REAL, std::size_t num_params>
class UniformSampler : public Sampler<REAL, num_params>{
    public:
    /**
     * @brief Constructor for Uniform Sampler. Instantiates base abstract class member variables and classs and contains some error checking for a non-sensical number of bins.
     * @param filepath: Filepath of the data that is fitted to the provided function.
     * @param func: Function that is used to fit the data. Takes the independent variable and parameter array.
     * @param names: The names of each of the parameters.
     * @param min_values: The minimum value of each parameter in the space.
     * @param max_values: The maximum value of each parameter in the space.
     * @param num_bins: The number of bins used to sample each parameter. (optional: default = 100)
     * @param rigidity: The flexibility of the Observations object when it reads data. (optional: default = false)
    */
    UniformSampler(const std::string &filepath, const std::function<REAL(REAL,std::array<REAL,num_params>&)> &func,std::array<std::string,num_params> &names, std::array<REAL,num_params> &min_values, std::array<REAL, num_params> &max_values, uint num_bins = 100, const bool rigidity = false) : Sampler<REAL, num_params>(filepath, func,names, min_values, max_values, num_bins, rigidity){
        if (std::pow(num_bins,num_params) > 1000000000){
            std::cerr << "Warning - Total parameter space exceeds 1,000,000,000. Uniform Sampling techniques are inefficient at this scale." << std::endl;
        }
    }


    /**
     * @brief Sampling method that uses uniform sampling technique. To sample every parameter combination across n bins and n parameters a recursive function is used. Overrides abstract virtual member function.
    */
    void sample() override {
        if (this -> been_sampled){
            std::cerr << "Error - Procedure aborted as this UniformSampler instance has already sampled the data points." << std::endl;
        }
        // obtain number of bins and array of ParamInfo objects. combination_gen is recursive function
        const std::array<ParamInfo<REAL>, num_params>& param_info = this -> get_params_info();
        uint num_bins = this -> get_bins();
        std::vector<uint> combination;
        combination_gen(combination, num_params, param_info, num_bins);
        this -> normalise_marginal_distribution();
        this -> been_sampled = true;
    }

    private:
    /**
     * @brief Recursive helper function that calls itself so that x parameters with n bins can be sampled. Starts with the for loop and adds the first bin index to the combination vector before it calls itself with one less parameter.
     * Another index is added to the combination vector. This process repeats until the value of n is 0 which corresponds to the number of parameters. #
     * When this happens the exit criteria is satisfied which is where the parameter vector is passed to the likelihood function and the marginal distribution obtains a contribution. This process repeats until every combination is sampled.
     * 
     * @param combination: vector of size num_param that contains the current combination of bin indices.
     * @param n: Initially set to be the number of params. In the recursive call decreased by 1 to move through parameters.
     * @param param_info: ParamInfo object containing param max and min.
     * @param num_bins: Number of bins as is private in base class.
     * 
    */
    void combination_gen(std::vector<uint>& combination, uint n, const std::array<ParamInfo<REAL>, num_params>& param_info,uint num_bins ){
        if (n == 0){
            std::array<REAL,num_params> parameters; 
            for (std::size_t idx = 0; idx < num_params; idx++){
                parameters[idx] = param_info[idx].min + (combination[idx] + 0.5) * param_info[idx].width/num_bins;
            }
            REAL lg_likelihood = this ->log_likelihood(parameters);
            this -> parameter_likelihood[parameters] = lg_likelihood;
            REAL likelihood = std::exp(lg_likelihood);
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