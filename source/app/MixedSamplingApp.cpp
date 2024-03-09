#include <iostream>
#include <cstdlib>
#include "UniformSampler.hpp"
#include "MetropolisHastingsSampler.hpp"
#include "ModelFunctions.hpp"
#include <memory>


/**
 * @brief Factory method function for producing a unique pointer to either a Metropolis Hastings Sampler or Uniform Sampler based on if the total parameter space is larger than or equal to the number of sample points specified.
 * @param filepath: Filepath to data that the sampling technique will use to fit the parameters of the model.
 * @param func: Function that the data is being fit to. For this application it is y = ax^3 + bx^2 + cx + d. This function must have two arguments: x input value and array of all parameters.
 * @param names: Array of the names of all the parameters.
 * @param min_values: Array of the minimum values of all the parameters.
 * @param max_values: Array of the maximum values of all the parameters.
 * @param num_bins: Number of bins used to discretise parameter space in each dimension.
 * @param step_size: Standard deviation of the mean centered normal distribution that is used to increment the parameter vector in the unit hypercube space.
 * @param num_sample_points: Max number of points used to sample the distribution.
 * @param rigidity: Rigidity setting for Observations object that loads data. true means that an exception is throw if there is an error with the data. false means that an error message is printed and the erroneous row is skipped but the file still is read.
 * @tparam REAL: type representing real numbers; usually float or double.
 * @tparam num_params: number of parameters that are being used to fit the model.
 * @return Unique pointer to class that is derived from the base abstract Sampler class. Either Uniform Sampler or MCMC sampler.
*/
template<typename REAL, std::size_t num_params>
std::unique_ptr<Sampler<REAL, num_params>> SamplerGen( 
    const std::string &filepath,
    const std::function<REAL(REAL, std::array<REAL, num_params>&)> &func,
    std::array<std::string, num_params> &names,
    std::array<REAL, num_params> &min_values,
    std::array<REAL, num_params> &max_values,
    uint num_bins = 100, 
    REAL step_size = 0.01, 
    uint num_sample_points = 100000, 
    bool rigidity = false)
    {
        if (num_sample_points >= std::pow(num_bins,num_params)){
            std::cout << "Uniform Sampler Initiated" << std::endl;
            return std::make_unique<UniformSampler<REAL,num_params>>(filepath, func, names, min_values, max_values, num_bins, rigidity);
        }
        else{
            std::cout << "Metropolis Hastings Sampler Initiated" << std::endl;
            return std::make_unique<MetropolisHastingSampler<REAL,num_params>>(filepath, func, names, min_values, max_values, num_sample_points, step_size, num_bins, rigidity);
        }
    }


/**
 * @brief: This function prints a help message for the Sampl4D application.
*/
void HelpMessage(){
    std::cout << "This program uses a combination of uniform and MCMC sampling to fit data to the equation y = ax^3 + bx^2 + cx + d with default parameter ranges from 0 to 5.\nThe step size used for the Metropolis Hastings Sampler is 0.01. \nThe data should come in the format of a txt file with columns inputs (x), outputs (y) or error (σ).\n\nBrief instructions can be found below." << std::endl;
    std::cout << "Usage: Sample4D -f <file_path> -n <number_of_bins> -s <number of samples>\n"
              << "Options:\n"
              << "  -h                       Show this help message\n"
              << "  -f  <path>               Path to the data file\n"
              << "  -n  <bins>               Number of bins for sampling\n"
              << "  -s  <number_samples>     Number of Samples to take\n"
              << "  -ar <upper,lower>        Range for parameter a                       (optional: default = -3,3)\n"
              << "  -br <upper,lower>        Range for parameter b                       (optional: default = -3,3)\n"
              << "  -cr <upper,lower>        Range for parameter c                       (optional: default = -3,3)\n"
              << "  -dr <upper,lower>        Range for parameter d                       (optional: default = -3,3)\n"
              << "  -p  <plot>               Plot condition (Y/N)                        (optional: default = Y)\n"
              << "  -g  <rigidity>           Strictness when Reading Data File (Bool)    (optional: default = false)" << std::endl;
}

std::array<double,2> split(std::string ranges){
    uint idx_comma = 0;
    for (uint i = 0; i < ranges.size(); i++){
        if (ranges[i] == ','){
            idx_comma = i;
        }
    }
    std::string upper = "";
    std::string lower = "";
    for (uint i = 0; i < ranges.size(); i++){
        if (i < idx_comma){
            lower += ranges[i];
        }
        else if (i > idx_comma){
            upper += ranges[i];
        }
        else{
            continue;
        }
    }
    return {std::stod(lower), std::stod(upper)};
}


int main(int argc, char** argv)
{
    // initialising all conditions and parameters for sampling class
    std::string filepath;
    uint num_bins = 0;
    uint num_samples = 0;
    bool rigidity = false;
    bool filepath_set = false;
    bool num_bins_set = false;
    bool a_range_set = false;
    bool b_range_set = false;
    bool c_range_set = false;
    bool d_range_set = false;
    bool rigidity_set = false;
    bool plot_condition = true;
    bool plot_condition_set = false;
    bool number_samples_set = false;
    std::array<double,2> a_range;
    std::array<double, 2> b_range;
    std::array<double, 2> c_range;
    std::array<double, 2> d_range;

    for (int i = 1; i < argc; i+=2){ // pass through every item in the stream. Incrementing by 2 as flag is followed by argument.
        std::string arg(argv[i]);
        if (arg == "-h"){ // check for help flag
            HelpMessage();
            return 0;
        }
        else if (arg == "-f") {
            if (filepath_set){
                std::cerr << "Error - Cannot set filepath twice!" << std::endl; // fix clashed of two different values of the same parameter being set.
                HelpMessage();
                return 1;
            }
            std::string arg1(argv[i+1]);
            filepath = arg1;
            filepath_set = true;
        }
        else if (arg == "-n"){
            if (num_bins_set){
                std::cerr << "Error - Cannot set the number of bins twice!" << std::endl;
                HelpMessage();
                return 1;
            }
            std::string arg1(argv[i+1]);
            num_bins = std::atoi(arg1.c_str());
            num_bins_set = true;
        }
        else if (arg == "-s"){
            if (number_samples_set){
                std::cerr << "Error - Cannot set the number of samples twice!" << std::endl;
                HelpMessage();
                return 1;
            }
            std::string arg1(argv[i+1]);
            num_samples = std::atoi(arg1.c_str());
            number_samples_set = true;
        }
        else if (arg == "-g"){
            if (rigidity_set){
                std::cerr << "Error - Cannot set the rigidity twice!" << std::endl;
                HelpMessage();
                return 1;
            }
            std::string arg1(argv[i+1]);
            if ((arg1 == "True") || (arg1 == "true")){
                rigidity = true;
                rigidity_set = true;
            }
            else if((arg1 == "False") || (arg1 == "false")){
                rigidity = false;
                rigidity_set = true;
            }
            else{
                std::cerr << "Error - please input valid rigidity setting!" << std::endl;
                HelpMessage();
                return 1;
            }
        }
        else if (arg == "-ar"){
            if (a_range_set){
                std::cerr << "Error - the range of parameter a has already been set!" << std::endl;
                HelpMessage();
                return 1;
            }
            std::string arg1(argv[i+1]);
            try{
                a_range = split(arg1.c_str());   // test operation for parameter a. 
            }
            catch(const std::invalid_argument&){
                std::cerr << "Error - the range of parameter a has had incorrect inputs!" << std::endl;
                HelpMessage();
                return 1;
            }
            a_range_set = true;
        }
        else if (arg == "-br"){
            if (b_range_set){
                std::cerr << "Error - the range of parameter b has already been set!" << std::endl;
                HelpMessage();
                return 1;
            }
            std::string arg1(argv[i+1]);
            try{
                b_range = split(arg1.c_str());   // test operation for parameter b. 
            }
            catch(const std::invalid_argument&){
                std::cerr << "Error - the range of parameter b has had incorrect inputs!" << std::endl;
                HelpMessage();
                return 1;
            }
            b_range_set = true;
        }
        else if (arg == "-cr"){
            if (c_range_set){
                std::cerr << "Error - the range of parameter c has already been set!" << std::endl;
                HelpMessage();
                return 1;
            }
            std::string arg1(argv[i+1]);
            try{
                c_range = split(arg1.c_str());   // test operation for parameter c. 
            }
            catch(const std::invalid_argument&){
                std::cerr << "Error - the range of parameter c has had incorrect inputs!" << std::endl;
                HelpMessage();
                return 1;
            }
            c_range_set = true;
        }
        else if (arg == "-dr"){
            if (d_range_set){
                std::cerr << "Error - the range of parameter d has already been set!" << std::endl;
                HelpMessage();
                return 1;
            }
            std::string arg1(argv[i+1]);
            try{
                d_range = split(arg1.c_str());   // test operation for parameter d. 
            }
            catch(const std::invalid_argument&){
                std::cerr << "Error - the range of parameter d has had incorrect inputs!" << std::endl;
                HelpMessage();
                return 1;
            }
            d_range_set = true;
        }
        else if (arg == "-p"){
            if (plot_condition_set){
                std::cerr << "Error - the plot condition cannot be set twice!" << std::endl;
                HelpMessage();
                return 1;
            }
            std::string arg1(argv[i + 1]);
            if ((arg1 == "Y") || (arg1 == "y")){
                plot_condition = true;
                plot_condition_set = true;
            }
            else if((arg1 == "N") || (arg1 == "n")){
                plot_condition = false;
                plot_condition_set = true;
            }
            else{
                std::cerr << "Error - please input valid plot condition!" << std::endl;
                HelpMessage();
                return 1;
            }
        }
        else{
            std::cerr << "Invalid Flag Detected: " << arg << std::endl; // outlier flags.
            return 1;
        }
    }// checking for invalid flags or insufficient flags
    if (!(num_bins_set && filepath_set && number_samples_set)){
        std::cerr << "Please enter the number of bins, filepath and the number of parameters to sample!" << std::endl;
        HelpMessage();
        return 1;
    }

    if (filepath.empty() || num_bins <= 0){
        std::cout << "Invalid or Invalid Arguments" << std::endl;
        HelpMessage();
        return 1;
    }
    // choosing between default values and user input values.
    std::array<std::string, 4> names = {"a", "b", "c", "d"};
    std::array<double, 4> min_vals;
    std::array<double, 4> max_vals;
    if (a_range_set){
        max_vals[0] = a_range[1];
        min_vals[0] = a_range[0];
    }
    else{
        max_vals[0] = 3;
        min_vals[0] = -3;
    }
    if (b_range_set){
        max_vals[1] = b_range[1];
        min_vals[1] = b_range[0];
    }
    else{
        max_vals[1] = 3;
        min_vals[1] = -3;
    }
    if (c_range_set){
        max_vals[2] = c_range[1];
        min_vals[2] = c_range[0];
    }
    else{
        max_vals[2] = 3;
        min_vals[2] = -3;
    }
    if (d_range_set){
        max_vals[3] = d_range[1];
        min_vals[3] = d_range[0];
    }
    else{
        max_vals[3] = 3;
        min_vals[3] = -3;
    }

    std::unique_ptr<Sampler<double, 4>> sampler_ptr;

    try{
        sampler_ptr = SamplerGen<double, 4>(filepath,polynomial<double>,names, min_vals, max_vals, num_bins, 0.01, num_samples,rigidity); // use of factory method which returns value which is assigned to unique pointer for Sampler base class. Example of polymorphism.
    }
    catch(const std::exception &e){
        std::cerr << e.what() << std::endl;
        HelpMessage();
        return 1;
    }

    sampler_ptr->sample();
    sampler_ptr->summarise();

    std::string sample_mode; // so files sent to correct folder based off sampling technique.
    if (num_samples >= std::pow(num_bins, 4)){
        sample_mode = "Uniform";
    }
    else{
        sample_mode = "MHS";
    }

    if (plot_condition){
        sampler_ptr->plot_histograms("cubic","Sample4D/" + sample_mode);
        sampler_ptr->plot_best_fit("cubic", "Sample4D/" + sample_mode);
    }
    
    return 0;
}