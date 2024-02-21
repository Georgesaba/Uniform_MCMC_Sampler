#include <iostream>
#include <cstdlib>
#include "UniformSampler.hpp"
#include "ModelFunctions.hpp"

void HelpMessage(){
    std::cout << "Usage: Sample2D -f <file_path> -n <number_of_bins>\n"
              << "Options:\n"
              << "  -h               Show this help message\n"
              << "  -f <path>        Path to the data file\n"
              << "  -n <bins>        Number of bins for sampling\n"
              << "  -g <rigidity>    Strictness when Reading Data File" << std::endl;
}



int main()//int argc, char** argv)
{
    // std::string filepath;
    // uint num_bins = 0;
    // bool rigidity = false;

    // if (argc < 5){
    //     HelpMessage();
    //     return 1;
    // }

    // for (uint i = 0; i < argc; i+=2){
    //     std::string arg(argv[i]);
    //     if (arg == "-h"){
    //         HelpMessage();
    //         return 0;
    //     }
    //     else if (i + 1 < argc && arg == "-f"){
    //         filepath = argv[i + 1];
    //     }
    //     else if (i + 1 < argc && arg == "-n"){
    //         num_bins = std::atoi(argv[i + 1]);
    //     }
    //     else if (i + 1 < argc && arg == "-g"){
    //         if ((argv[i + 1] == "True") || (argv[i + 1] == "true")){
    //             rigidity = true;
    //         }
    //     }
    //     else {
    //         HelpMessage();
    //         return 1;
    //     }
    // }
    // if (filepath.empty() || num_bins <= 0){
    //     std::cout << "Invalid or Invalid Arguments" << std::endl;
    //     HelpMessage();
    //     return 1;
    // }

    // std::array<std::string,2> names = {"a", "b"};
    // std::array<double, 2> min_vals = {0, 0};
    // std::array<double, 2> max_vals = {5, 5};
    // try{
    //     UniformSampler<double, 2> uniform_sampler(filepath,param_2_model_func<double>,names, min_vals, max_vals, num_bins,rigidity);
    // }
    // catch(const std::exception &e){
    //     std::cerr << e.what() << std::endl;
    //     return 1;
    // }

    // uniform_sampler.sample();
    // uniform_sampler.summarise();


    // return 0;
}