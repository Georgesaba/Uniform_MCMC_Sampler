#include "ModelFunctions.hpp"
#define _USE_MATH_DEFINES
#include <cmath>


template <typename REAL>
REAL param_2_model_func(REAL x, std::array<REAL, 2> &params){
    return params[0] * std::pow(x,params[1]);
}

template <typename REAL>
REAL param_1_model_func(REAL x, std::array<REAL, 1> &params){
    return std::pow(x,params[0]);
}

template <typename REAL>
REAL param_test_model_func(REAL x, std::array<REAL, 2> &params){
    return params[0] * x + params[1];
}

template <typename REAL>
REAL param_3_test_model_func(REAL x, std::array<REAL, 3> &params){
    return params[0] * x * x + params[1] *x + params[2];
}

template <typename REAL>
REAL gaussian_func(REAL x, REAL sigma, REAL mean){
    return (1.0/(sigma * std::sqrt(2 * M_PI))) * std::exp(-0.5 * (x - mean) * (x - mean)/(sigma * sigma));
}

template <typename REAL>
REAL polynomial(REAL x, std::array<REAL, 4> &params){
    return params[0] * x * x * x + params[1] * x * x + params[2] * x + params[3];
}

template double param_2_model_func<double>(double, std::array<double, 2>&);
template double param_1_model_func<double>(double, std::array<double, 1>&);
template double param_test_model_func<double>(double, std::array<double, 2>&);
template double param_3_test_model_func<double>(double, std::array<double, 3>&);
template double gaussian_func<double>(double, double, double);
template double polynomial<double>(double, std::array<double, 4>&);

template float param_2_model_func<float>(float, std::array<float, 2>&);
template float param_1_model_func<float>(float, std::array<float, 1>&);
template float param_test_model_func<float>(float, std::array<float, 2>&);
template float param_3_test_model_func<float>(float, std::array<float, 3>&);
template float gaussian_func<float>(float, float, float);
template float polynomial<float>(float, std::array<float,4>&);