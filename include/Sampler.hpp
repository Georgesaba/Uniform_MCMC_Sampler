#pragma once
#include <vector>
#include <string>
#include "Observations.hpp"
#include <memory>
#include <iostream>
#include <map>
#include <cmath>
#include <functional>
#include <numeric>
#include <ParamInfo.hpp>
#include "Plot.hpp"
#include <optional>


/**
 * @brief Abstract class template that derived sampling classes inherit from. Contains pure virtual member function sample to be overwritten and has functionality to 
 * calculate and store log likelihood values calculated using a model function that can be passed in. Stores file data in an observations object. 
 * Once the marginal distribution has been calculated in the derived class this class contains functionality to summarise and plot the data.
 *
 * @tparam REAL: type representing real numbers; usually float or double.
 * @tparam num_params: number of parameters that are being used to fit the model.
*/
template<typename REAL, std::size_t num_params>
class Sampler
{
    public:
    /**
     * @brief: Constructor that assigns the number of bins, the model function and the empty array of the marginal distribution as member variables. Sets the parameter information stored in the ParamInfo objects. Loads data using the Observations object.
     * @param filepath: Filepath of the data that is fitted to the provided function.
     * @param func: Function that is used to fit the data. Takes the independent variable and parameter array.
     * @param names: The names of each of the parameters.
     * @param min_values: The minimum value of each parameter in the space.
     * @param max_values: The maximum value of each parameter in the space.
     * @param num_bins: The number of bins used to sample each parameter. (optional: default = 100)
     * @param rigidity: The flexibility of the Observations object when it reads data. (optional: default = false)
    */
    Sampler(const std::string &filepath, const std::function<REAL(REAL,std::array<REAL,num_params>&)> &func, std::array<std::string,num_params> names, std::array<REAL,num_params> min_values, std::array<REAL, num_params> max_values, uint num_bins = 100,const bool rigidity = false) : bins(num_bins) , model_function(func){
        if (num_bins > 400000000){ // check for negative value inputted causing uint to cycle back to maximum possible value.
            throw std::domain_error("Error - Abnormally large number of bins selected above 400,000,000. Please use a smaller number of bins.");
        }
        if (num_bins == 0){
            throw std::domain_error("Error - Number of bins cannot be 0.");
        }
        observations.loadData(filepath,rigidity);
        set_param_info(names, min_values, max_values);
        marginal_distribution = std::vector<std::vector<REAL>>(num_params, std::vector<REAL>(num_bins, 0));
    }
    virtual ~Sampler() = default;
    
    void set_param_info(std::array<std::string,num_params> names, std::array<REAL,num_params> min_values, std::array<REAL, num_params> max_values){
        for (std::size_t i = 0; i < num_params;i++){
            params_info[i].set_values(min_values[i],max_values[i],names[i]);
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
    

    /**
     * @brief: Calculates the log likelihood of the function using specific parameters being a fit for the data we are modelling.
     * @return: log likelihood value at that specific parameter vector.
    */
    REAL log_likelihood(std::array<REAL, num_params> params){
        REAL sum_likelihood = 0;
        for (uint i = 0; i < observations.num_points; i++){
            REAL func_output = model_function(observations.inputs[i],params);
            sum_likelihood += -(func_output - observations.outputs[i]) * (func_output - observations.outputs[i]) /(2 * observations.sigmas[i] * observations.sigmas[i]);
        }
        return sum_likelihood;
    }

    /**
     * @brief: This member function provides summary statistics for the marginal distribution such as the mean, standard deviation and the midpoint of the bin that houses the largest marginal probability for each parameter.
     * Adds the statistics that are calculated to member variables of the ParamInfo object. Evaluates the mean as \sum_i a_i M_a[i] and the standard deviation as  \sqrt{(\sum_i a_i^2 M_a[i]) - mean_a}. i is the bin index and a is the param.
    */
    void summarise(bool print = true){
        if (!been_sampled){
            throw std::logic_error("Error - Sample() has not been called."); // can not be summarised before sampling happens.
        }
        for (std::size_t i = 0; i < num_params; i++){
            REAL marginal_peak = 0;
            REAL param_max = 0;
            REAL param_mean = 0;
            REAL param_incomplete_sigma = 0; // \sum ((a_i)^2 M_a[i])

            // reference to prevent reindexing over all bins
            const std::vector<REAL>& current_marginal_distribution = marginal_distribution[i];
            ParamInfo<REAL>& current_params_info = params_info[i];
            
            // calculate stats iterating through all bins per param
            for (uint j = 0; j < bins; j++){
                REAL param_val = current_params_info.min + (j + 0.5) * current_params_info.width/bins; //convert index to param

                if (current_marginal_distribution[j]  > marginal_peak){
                    marginal_peak = current_marginal_distribution[j];
                    param_max = param_val;
                }
                
                param_mean += param_val * current_marginal_distribution[j];
                param_incomplete_sigma += param_val * param_val * current_marginal_distribution[j];
            }
            REAL standard_deviation = std::sqrt(param_incomplete_sigma - param_mean * param_mean);
            current_params_info.marginal_distribution_peak = param_max;
            current_params_info.mean_parameter = param_mean;
            current_params_info.standard_deviation = standard_deviation;
            if (print){
                std::cout << "Parameter " + current_params_info.name + " : \n" + "Standard Deviation - " << standard_deviation << "\n";
                std::cout << "Mean - " << param_mean << "\n";
                std::cout << "Parameter at Marginal Distribution Peak - " << param_max << "\n" << std::endl;
            }
        }
    }
    
    /**
     * @brief: Member function that plots marginal distribution histograms for every parameter. Generates filepath key depending on parameters.
     * @param func_desc: Description of function used to fit data.
     * @param application_name: Name of application that is using this class. Useful to identify correct location to store plots. Can also include type of sampling for Sample4D.
    */
    void plot_histograms(std::string func_desc = "y=ax^b", std::string application_name = "Sample2D") const {
        for (std::size_t i = 0; i < num_params; i++){
            std::string name = "Param " + params_info[i].name + " Marginal Distribution (" + std::to_string(bins) + " bins) - " + func_desc;
            std::string minimum_param_val = removeTrailingDecimalPlaces<REAL>(params_info[i].min);
            std::string maximum_param_val = removeTrailingDecimalPlaces<REAL>(params_info[i].max);
            std::string label_extension = "_";
            if (extra_settings){
                for (const auto& pair:extra_settings.value()){
                    label_extension += pair.first + "_" + pair.second + "_"; //specify number of sampled points and step size.
                }
            }
            std::string filepath = "plots/" + application_name + "/MarginalDistribution/"  + "dist_" + params_info[i].name + label_extension + minimum_param_val + "_" + maximum_param_val + "_" + std::to_string(bins) + "_" + func_desc + ".png";
            plot_histogram<REAL>(name, filepath, params_info[i], marginal_distribution[i]);
        }
    }

    /**
     * @brief: Member function that plots best fit using sampled parameters. Generates filepath key depending on parameters.
     * @param func_desc: Description of function used to fit data.
     * @param application_name: Name of application that is using this class. Useful to identify correct location to store plots. Can also include type of sampling for Sample4D.
    */
    void plot_best_fit(std::string func_desc = "y=ax^b", std::string application_name = "Sample2D") const {
        std::string label_extension = "_";
        std::string param_ranges;
        std::string file_param_ranges;
        std::array<REAL, num_params> fit_params;
        for (std::size_t i = 0; i < num_params; i++){
            if (i != 0){
                param_ranges += ",";
                file_param_ranges += "_";
            }
            file_param_ranges += params_info[i].name + "_" + removeTrailingDecimalPlaces<REAL>(params_info[i].min) + "_" + removeTrailingDecimalPlaces<REAL>(params_info[i].max); 
            param_ranges += params_info[i].name + "(" + removeTrailingDecimalPlaces<REAL>(params_info[i].min) + "," + removeTrailingDecimalPlaces<REAL>(params_info[i].max) + ")";
            fit_params[i] = params_info[i].mean_parameter;
        }
        if (extra_settings){       // Extra tags for MHS sampler
            for (const auto& pair:extra_settings.value()){
                label_extension += pair.first + "_" + pair.second + "_";
            }
        }

        std::string name = "Fitted Data with params " + param_ranges + " - " + std::to_string(bins) + " bins";
        std::string filepath = "plots/"+ application_name + "/CurveFit/fit_" + file_param_ranges + "_" + std::to_string(bins) + label_extension + func_desc + ".png";

        plot_fitted_data<REAL, num_params>(name, filepath,func_desc, fit_params, model_function, observations.inputs, observations.outputs, observations.sigmas);
    }

    private:
    uint bins;
    std::array<ParamInfo<REAL>, num_params> params_info;
    std::function<REAL(REAL,std::array<REAL, num_params>&)> model_function;
    std::optional<std::map<std::string, std::string>> extra_settings;
    
    protected:
    /**
     * @brief: Normalise the marginal probabilities in the marginal distribution vector. They should all sum to 1.
    */
    void normalise_marginal_distribution(){
        for (std::size_t i = 0; i < num_params; i++){
            REAL total_prob = std::accumulate(this -> marginal_distribution[i].begin(), this -> marginal_distribution[i].end(),0.0);//sum of marginal distribution must equal 1
            for (REAL &num: marginal_distribution[i]){
                num /= total_prob;
            }
        }
    }
    
    // for derived classes that have extra conditions so they can be included in plots.
    void set_extra_settings(std::map<std::string,std::string> settings){
        extra_settings = settings;
    }


    Observations<REAL> observations; // std::vector stores data on heap. Observations mainly stores three vectors so can store it on stack.
    std::map<std::array<REAL, num_params>,REAL> parameter_likelihood; //  Dict for parameter vector and liklihood.
    std::vector<std::vector<REAL>> marginal_distribution;
    bool been_sampled = false;
};