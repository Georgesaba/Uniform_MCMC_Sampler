#pragma once
#include <array>

/**
 * @brief All the functions declared below are model functions that are passed into the derived classes. The data can be fit to certain relationships here.
*/


template <typename REAL>
REAL param_2_model_func(REAL x, std::array<REAL, 2> &params);

template <typename REAL>
REAL param_1_model_func(REAL x, std::array<REAL, 1> &params);

template <typename REAL>
REAL param_test_model_func(REAL x, std::array<REAL, 2> &params);

template <typename REAL>
REAL param_3_test_model_func(REAL x, std::array<REAL, 3> &params);

template <typename REAL>
REAL gaussian_func(REAL x, REAL sigma, REAL mean);

template <typename REAL>
REAL polynomial(REAL x, std::array<REAL, 4> &params);