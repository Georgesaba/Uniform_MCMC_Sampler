#pragma once
#include <optional>
#include "ParamInfo.hpp"
#include "ModelFunctions.hpp"
#include <iostream>
#include <vector>
#include <fstream>
#include <iterator>
#include <string>
#include <matplot/matplot.h>
#include <functional>
#include <sstream>
#include <iomanip>
#include <optional>

using namespace matplot;

/**
 * @tparam REAL: type representing real numbers; usually float or double.
 * @tparam num_params: Number of parameters that are being sampled.
*/


template<typename REAL>
std::string formatREALToNDecimalPlaces(REAL value, uint dp = 3) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(dp) << value;
    return oss.str();
}

/**
 * @brief: Rounds value to decimal place if value is the same as if it was rounded to one more decimal place. e.g. 2.50003 is rounded to 2.5. 2.05 is rounded to 2. 2.34340 is 2.3434. Therefore cuts off values from the first zero to the left.
 * @returns Formatted string with trailing zeros cut off from the left.
*/
template<typename REAL>
std::string removeTrailingDecimalPlaces(REAL value, uint max_dp = 3){
    std::optional<uint> idx;
    for (uint i = 0; i < max_dp - 1; i++){
        double rd1 =  std::stod(formatREALToNDecimalPlaces(value,i)); // converting to double for validation purpose
        double rd2 = std::stod(formatREALToNDecimalPlaces(value,i + 1));
        if (rd1 == rd2){
            idx.emplace(i);
            break;
        }
    }
    if (!(idx)){
        idx.emplace(max_dp - 1);
    }
    return formatREALToNDecimalPlaces(value,idx.value());
}


/**
 * @brief: Removes trailing zeros in values starting from the most amount of decimal places. Cuts off values from the first zero to the right.
 * @returns: Formatted string with trailing zeros cut off from the right.
*/
template<typename REAL>
std::string findsigfig(REAL number){
    uint idx = 0;
    std::string result = "";
    std::string num_as_string = std::to_string(number);
    for (uint i = num_as_string.size() - 1; i > 0; i--){
        if (num_as_string[i] != '0'){
            idx = i;
            break;
        }
    }
    for (uint i = 0; i < num_as_string.size(); i++){
        result += num_as_string[i];
        if (i == idx){
            break;
        }
    }
    return result;
}

/**
 * @brief: Function that plots histogram and gaussian pdf fit of parameter marginal distribution using the calculated mean and standard deviation. The marginal distribution is normalised using the width of a single bin.
 * @param name: This string is the title of the graph.
 * @param filepath: This is the path of the image that the graph is saved to.
 * @param param_info: This is the ParamInfo instance that is attributed to the specific parameter.
 * @param marginal_distribution: This is the marginal distribution of the sampled parameter.
 * @param num_fit_points: This is the number of points that is used to plot the gaussain fit. Default value at 10,000.
*/
template<typename REAL>
void plot_histogram(const std::string &name, const std::string &filepath,const ParamInfo<REAL>& param_info, const std::vector<REAL> &marginal_distribution, uint num_fit_points = 10000){
    uint num_bins = marginal_distribution.size();
    std::vector<REAL> bin_midpoints;
    std::vector<REAL> marginal_probability_density;
    for (uint i = 0; i < num_bins; i++){
        bin_midpoints.push_back(param_info.min + (i + 0.5) * param_info.width/num_bins);
        marginal_probability_density.push_back(marginal_distribution[i]/(param_info.width/num_bins)); // need to compute marginal probabiity density from marginal probability
    }
    std::vector<REAL> smooth_x;
    std::vector<REAL> smooth_y;
    REAL param_step = param_info.width/num_fit_points;
    
    for (REAL i = param_info.min; i <= param_info.max; i+=param_step){ //x and y of gaussian fit
        smooth_x.push_back(i);
        smooth_y.push_back(gaussian_func<REAL>(i,param_info.standard_deviation, param_info.mean_parameter));
    }

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
}


/**
 * @brief: Function that plots the observational data fitted to the function using the means of each parameter. Displays the ranges of the parameters used for the fit in the title.
 * @param name: The title of the plot.
 * @param filepath: The path that the image containing the plots is saved to.
 * @param func_desc: The desciption of the function used for the fit. This is either a desciption such as cubic or the actual equation.
 * @param func: This is the function that the data is fit to.
 * @param x: This is the independent variable of the data that is recorded.
 * @param y: This is the dependent variable of the data that is recorded.
 * @param sigma: This is the error of the speciifc measurement/dependent variable.
 * @param num_fit_points: This is the number of points that the data is fit to/
*/
template <typename REAL, std::size_t num_params>
void plot_fitted_data(const std::string &name, const std::string &filepath,const std::string &func_desc,std::array<REAL, num_params> &params, const std::function<REAL(REAL,std::array<REAL,num_params>&)> &func, const std::vector<REAL> &x ,const std::vector<REAL> &y,const std::vector<REAL> &sigma, uint num_fit_points = 10000){
    std::vector<REAL> smooth_x; // input and outputs of highly granular fit with resultant params
    std::vector<REAL> smooth_y;
    auto maxIt = std::max_element(x.begin(), x.end());
    auto minIt = std::min_element(x.begin(), x.end());
    REAL max_x = *maxIt;
    REAL min_x = *minIt;
    REAL x_step = (max_x - min_x)/num_fit_points;
        
    for (REAL i = min_x; i <= max_x; i+=x_step){
        smooth_x.push_back(i);
        smooth_y.push_back(func(i,params));
    }
    figure();
    errorbar(x, y, sigma, "none");
    hold(on);
    auto p = plot(smooth_x, smooth_y, "r");
    p -> line_width(2);
    title(name);
    if (func_desc == "cubic"){
        gca()->title_font_size_multiplier(0.8);
    }
    xlabel("Input");
    ylabel("Output");
    legend({"Observations","Best Fit - " + func_desc});
    save(filepath);
}
