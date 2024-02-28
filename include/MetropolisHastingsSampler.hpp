#pragma once
#include "Sampler.hpp"
#include <random>

/**
 * @brief Derived class template from base abstract class template that uses the Monte Carlo Markov Chain sampling method using the Metropolis Hastings algorithm. 
 * Overrides the sample method and samples a user defined amount of points by comparing the likelihoods of the new and old points using an acceptance criteria to propagate the chain.
 * Introduces two new member variables which are the step size and number of points that are sampled.
 * 
 * @tparam REAL: type representing real numbers; usually float or double.
 * 
 * @tparam num_params: number of parameters that are being used to fit the model.
*/
template<typename REAL, std::size_t num_params>
class MetropolisHastingSampler : public Sampler<REAL, num_params>{
    public:
    /**
     * @brief Constructor for MetropolisHastingSampler. Contains error handling for excessive number of sample points. Calls the constructor of base abstract sampler class.
     * @param filepath: Filepath of the data that is fitted to the provided function.
     * @param func: Function that is used to fit the data. Takes the independent variable and parameter array.
     * @param names: The names of each of the parameters.
     * @param min_values: The minimum value of each parameter in the space.
     * @param max_values: The maximum value of each parameter in the space.
     * @param sample_points: The maximum number of points that the sampler will sample.
     * @param step_s: Standard deviation of 0 centered normal distribution that is used to propogate the position in the unit hypercube.
     * @param num_bins: The number of bins used to sample each parameter. (optional: default = 100)
     * @param rigidity: The flexibility of the Observations object when it reads data. (optional: default = false)
    */
    MetropolisHastingSampler(const std::string &filepath, const std::function<REAL(REAL,std::array<REAL,num_params>&)> &func,
    std::array<std::string,num_params> names, std::array<REAL,num_params> min_values, std::array<REAL, num_params> max_values,
    uint sample_points = 100000, REAL step_s = 0.01,
    uint num_bins = 100, const bool rigidity = false) : Sampler<REAL, num_params>(filepath, func,names, min_values, max_values, num_bins, rigidity)
    {
        if (sample_points > 1000000000){ // sample_pints is a uint so if a negative value is accidentally put in this warning will show. If unintended user can terminate program.
            std::cerr << "Warning: The number of points to be sampled exceeds 1,000,000,000. This amount is excessively high and may take a while. Please make sure you want to keep sampling!" << std::endl;
        }
        
        num_sample_points = sample_points;
        step_size = step_s;
    }

    /**
     * @brief: Sampling method that uses the Metropolis Hastings algorithm to propogate the parameter vector of the system. 
     * It uses a uniform distribution from the std::default_random_engine type that is seeded at 42 to generate an initial position in the unit hyperspace. 
     * A vector is then added to the unit hypercube thhat is generated from a zero mean normal distribution with standard deviation as the step size. If the log likelihood of the new parameter value is higher than the old the chain is advanced.
     * Otherwise a uniform distribution generates a number u between 0 and 1. If log(u) < new log likelihood - old log likelihood then the new positon is accepted. If not it is rejected.
    */
    void sample() override {
        if (this -> been_sampled){
            throw std::logic_error("Error - Procedure aborted as this MetropolisHastingsSampler instance has already sampled the data points.");
        }
        const std::array<ParamInfo<REAL>, num_params>& params_info = this -> get_params_info();
        std::default_random_engine generator(42); //initiating generator with random seed 42 to keep results consistent and reproductable.
        std::array<REAL, num_params> unit_hypercube;
        std::array<REAL, num_params> new_unit_hypercube;
        std::array<REAL, num_params> params;
        std::array<REAL, num_params> new_params;
        uint number_bins = this -> get_bins();
        std::uniform_real_distribution<REAL> initial_dist(0, 1);
        std::normal_distribution<REAL> step_dist(0, step_size);
        
        uint bin_number;
        REAL lg_likelihood;

        for (std::size_t i = 0; i < num_params; i++){
            unit_hypercube[i] = initial_dist(generator);
            params[i] = params_info[i].min + unit_hypercube[i] * params_info[i].width;
                      
            bin_number = static_cast<uint>(std::floor(unit_hypercube[i] * number_bins));
            
            this -> marginal_distribution[i][bin_number]++;
        }
        
        this -> parameter_likelihood[params] = this -> log_likelihood(params);
        
        for (uint j = 0; j < num_sample_points; j++){
            for (std::size_t i = 0; i < num_params; i++){
                new_unit_hypercube[i] = unit_hypercube[i] + step_dist(generator);
                if (new_unit_hypercube[i] > 1){ //boundary conditions of unit hypercube applied here
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
            else{           // second acceptance criterion
                if ((lg_likelihood - this -> parameter_likelihood[params]) > std::log(initial_dist(generator))){
                    params = new_params;
                    unit_hypercube = new_unit_hypercube;
                    this -> parameter_likelihood[new_params] = lg_likelihood;
                    
                }
                else{
                    new_params = params; // reject
                    new_unit_hypercube = unit_hypercube;
                }
            }
            for (std::size_t i = 0; i < num_params; i++){
                bin_number = static_cast<uint>(std::floor(unit_hypercube[i] * number_bins));
                this -> marginal_distribution[i][bin_number]++;
            }
        }
        this -> normalise_marginal_distribution(); //normalise and add conditions to extra setting map. Used add tags to he plot filenames.
        this -> been_sampled = true;
        this -> set_extra_settings({{"step_size", findsigfig<REAL>(step_size)},{"N_sample",std::to_string(num_sample_points)}});
    }

    private:
    uint num_sample_points;
    REAL step_size;
};