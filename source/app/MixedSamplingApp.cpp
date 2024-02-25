#include <iostream>
#include <cstdlib>
#include "UniformSampler.hpp"
#include "MetropolisHastingsSampler.hpp"
#include "ModelFunctions.hpp"
#include <memory>

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





void HelpMessage(){
    std::cout << "This program uses uniform sampling to fit data to the equation y = ax^3 + bx^2 + cx + d with default parameter ranges from 0 to 5.\nThe step size used for the Metropolis Hastings Sampler is 0.01. \nThe data should come in the format of a txt file with columns inputs (x), outputs (y) or error (σ)\n\nBrief Instructions can be found below." << std::endl;
    std::cout << "Usage: Sample4D -f <file_path> -n <number_of_bins> -s <number of samples>\n"
              << "Options:\n"
              << "  -h                       Show this help message\n"
              << "  -f <path>                Path to the data file\n"
              << "  -n <bins>                Number of bins for sampling\n"
              << "  -ar <upper,lower>        Range for parameter a\n"
              << "  -br <upper,lower>        Range for parameter b\n"
              << "  -cr <upper,lower>        Range for parameter c\n"
              << "  -dr <upper,lower>        Range for parameter d\n"
              << "  -s  <number_samples>     Number of Samples to take\n"
              << "  -p  <plot>               Plot condition (Y/N)\n"
              << "  -g <rigidity>            Strictness when Reading Data File (Bool)" << std::endl;
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

    for (int i = 1; i < argc; i+=2){
        std::string arg(argv[i]);
        if (arg == "-h"){
            HelpMessage();
            return 0;
        }
        else if (arg == "-f") {
            if (filepath_set){
                std::cerr << "Error - Cannot set filepath twice!" << std::endl;
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
            a_range = split(arg1.c_str());
            a_range_set = true;
        }
        else if (arg == "-br"){
            if (b_range_set){
                std::cerr << "Error - the range of parameter b has already been set!" << std::endl;
                HelpMessage();
                return 1;
            }
            std::string arg1(argv[i+1]);
            b_range = split(arg1.c_str());
            b_range_set = true;
        }
        else if (arg == "-cr"){
            if (c_range_set){
                std::cerr << "Error - the range of parameter c has already been set!" << std::endl;
                HelpMessage();
                return 1;
            }
            std::string arg1(argv[i+1]);
            c_range = split(arg1.c_str());
            c_range_set = true;
        }
        else if (arg == "-dr"){
            if (d_range_set){
                std::cerr << "Error - the range of parameter d has already been set!" << std::endl;
                HelpMessage();
                return 1;
            }
            std::string arg1(argv[i+1]);
            d_range = split(arg1.c_str());
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
            std::cerr << "Invalid Flag Detected: " << arg << std::endl;
            return 1;
        }
    }
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
        sampler_ptr = SamplerGen<double, 4>(filepath,polynomial<double>,names, min_vals, max_vals, num_bins, 0.01, num_samples,rigidity);
    }
    catch(const std::exception &e){
        std::cerr << e.what() << std::endl;
        HelpMessage();
        return 1;
    }

    sampler_ptr->sample();
    sampler_ptr->summarise();

    std::string sample_mode;
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