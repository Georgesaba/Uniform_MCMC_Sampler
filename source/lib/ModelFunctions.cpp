#include "ModelFunctions.hpp"
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


template double param_2_model_func<double>(double, std::array<double, 2>&);
template double param_1_model_func<double>(double, std::array<double, 1>&);
template double param_test_model_func<double>(double, std::array<double, 2>&);
template double param_3_test_model_func<double>(double, std::array<double, 3>&);

template float param_2_model_func<float>(float, std::array<float, 2>&);
template float param_1_model_func<float>(float, std::array<float, 1>&);
template float param_test_model_func<float>(float, std::array<float, 2>&);
template float param_3_test_model_func<float>(float, std::array<float, 3>&);