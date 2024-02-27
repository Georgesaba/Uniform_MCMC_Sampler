#include <iostream>
#include <cstdlib>
#include "UniformSampler.hpp"
#include "ModelFunctions.hpp"
#include <memory>
/**
 * @brief: This function prints out a help message that helps the user use the Sample2D application.
*/
void HelpMessage(){
    std::cout << "This program uses uniform sampling to fit data to the equation y = ax^b with default parameter ranges from 0 to 5. \nThe data should come in the format of a txt file with columns inputs (x), outputs (y) or error (σ).\n\nBrief instructions can be found below." << std::endl;
    std::cout << "Usage: Sample2D -f <file_path> -n <number_of_bins>\n"
              << "Options:\n"
              << "  -h                Show this help message\n"
              << "  -f  <path>        Path to the data file\n"
              << "  -n  <bins>        Number of bins for sampling\n"
              << "  -ar <upper,lower> Range for parameter a             (optional: default = 0,5)\n"
              << "  -br <upper,lower> Range for parameter b             (optional: default = 0,5)\n"
              << "  -p  <plot>        Plot condition (Y/N)              (optional: default = Y)\n"
              << "  -g  <rigidity>    Strictness when Reading Data File (optional: default = false)" << std::endl;
}
// finds index of comma in string and then uses it as delimiter to split into two substrings. Converts string to double after.
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
    bool rigidity = false;
    bool filepath_set = false;
    bool num_bins_set = false;
    bool a_range_set = false; // track if values have been set already. std::optional could also have been used here however this approach is simpler as some conditions have default values.
    bool b_range_set = false;
    bool rigidity_set = false;
    bool plot_condition = true;
    bool plot_condition_set = false;
    std::array<double,2> a_range;
    std::array<double, 2> b_range;

    for (int i = 1; i < argc; i+=2){ // check for flags
        std::string arg(argv[i]);
        if (arg == "-h"){
            HelpMessage();
            return 0;
        }
        else if (arg == "-f") {
            if (filepath_set){
                std::cerr << "Error - Cannot set filepath twice!" << std::endl; // check for clashes caused by setting the same value to different conditions. 
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
        } // error handling 
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
                b_range = split(arg1.c_str());
            }
            catch(const std::invalid_argument&){
                std::cerr << "Error - the range of parameter b has had incorrect inputs!" << std::endl;
                HelpMessage();
                return 1;
            }
            b_range_set = true;
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
        else{ // extra error handling
            std::cerr << "Invalid Flag Detected: " << arg << std::endl;
            return 1;
        }
    }
    if (!(num_bins_set && filepath_set)){
        std::cerr << "Please enter the number of bins and the filepath!" << std::endl;
        HelpMessage();
        return 1;
    }

    if (filepath.empty() || num_bins <= 0){
        std::cout << "Invalid or Invalid Arguments" << std::endl;
        HelpMessage();
        return 1;
    }

    std::array<std::string,2> names = {"a", "b"}; // assign defaults or user input values
    std::array<double, 2> min_vals;
    std::array<double, 2> max_vals;
    if (a_range_set){
        max_vals[0] = a_range[1];
        min_vals[0] = a_range[0];
    }
    else{
        max_vals[0] = 5;
        min_vals[0] = 0;
    }
    if (b_range_set){
        max_vals[1] = b_range[1];
        min_vals[1] = b_range[0];
    }
    else{
        max_vals[1] = 5;
        min_vals[1] = 0;
    }

    std::unique_ptr<UniformSampler<double, 2>> uniform_sampler_ptr;

    try{
        uniform_sampler_ptr = std::make_unique<UniformSampler<double, 2>>(filepath,param_2_model_func<double>,names, min_vals, max_vals, num_bins,rigidity); // declared before so it exists outside of try scope. Use smart pointers for delayed construction of object
    }
    catch(const std::exception &e){
        std::cerr << e.what() << std::endl;
        HelpMessage();
        return 1;
    }

    uniform_sampler_ptr->sample();
    uniform_sampler_ptr->summarise();

    if (plot_condition){
        uniform_sampler_ptr->plot_histograms();
        uniform_sampler_ptr->plot_best_fit();
    }
    
    return 0;
}