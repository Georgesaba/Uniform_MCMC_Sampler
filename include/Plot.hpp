#pragma once
#include "ParamInfo.hpp"
#include "ModelFunctions.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <matplot/matplot.h>
#include <functional>
#include <sstream>
#include <iomanip>

using namespace matplot;

template<typename REAL>
std::string formatREALToNDecimalPlaces(REAL value, uint dp = 3) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(dp) << value;
    return oss.str();
}

template<typename REAL>
void plot_histogram(const std::string &name, const std::string &filepath,const ParamInfo<REAL>& param_info, const std::vector<REAL> &marginal_distribution, uint num_fit_points = 10000){
    uint num_bins = marginal_distribution.size();
    std::vector<REAL> bin_midpoints;
    std::vector<REAL> marginal_probability_density;
    for (uint i = 0; i < num_bins; i++){
        bin_midpoints.push_back(param_info.min + (i + 0.5) * param_info.width/num_bins);
        marginal_probability_density.push_back(marginal_distribution[i]/(param_info.width/num_bins)); // need to compute marginal probabiity density
    }
    std::vector<REAL> smooth_x;
    std::vector<REAL> smooth_y;
    REAL param_step = param_info.width/num_fit_points;
    
    
    for (REAL i = param_info.min; i <= param_info.max; i+=param_step){
        smooth_x.push_back(i);
        smooth_y.push_back(gaussian_func<REAL>(i,param_info.standard_deviation, param_info.mean_parameter));
    }
    //figure_size(1000, 600); 
    figure();
    auto b = bar(bin_midpoints, marginal_probability_density);
    hold(on);
    b->face_color({0.2, 0.2, 0.8});
    auto p = plot(smooth_x, smooth_y, "r");
    p->line_width(2);
    title(name);
    xlabel("Parameter Value");
    ylabel("Marginal Distribution");
    std::string mu = formatREALToNDecimalPlaces<REAL>(param_info.mean_parameter,3);
    std::string sig = formatREALToNDecimalPlaces<REAL>(param_info.standard_deviation,3);
    
    legend({"Marginal Distribution","Gaussian Fit: μ = " + mu + ", σ  = " + sig});
    save(filepath);
    //close();
    //show();
}

template <typename REAL, std::size_t num_params>
void plot_fitted_data(std::string name, std::string filepath, std::string func_desc,std::array<REAL, num_params> params, const std::function<REAL(REAL,std::array<REAL,num_params>&)> &func,std::vector<REAL> x ,std::vector<REAL> y, uint num_fit_points = 10000){
    std::vector<REAL> smooth_x; // input and outputs of highly granular fit with resultant params
    std::vector<REAL> smooth_y;
    auto maxIt = std::max_element(x.begin(),x.end());
    auto minIt = std::min_element(x.begin(),x.end());
    REAL max_x = *maxIt;
    REAL min_x = *minIt;
    REAL x_step = (max_x - min_x)/num_fit_points;
        
    for (REAL i = min_x; i <= max_x; i+=x_step){
        smooth_x.push_back(i);
        smooth_y.push_back(func(i,params));
    }
    figure();
    scatter(x, y);
    hold(on);
    auto p = plot(smooth_x, smooth_y, "r");
    p -> line_width(2);
    title(name);
    xlabel("Input Data");
    ylabel("Output Data");
    legend({"Data","Best Fit - " + func_desc});
    save(filepath);
    //close();
}