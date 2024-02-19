#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>
#include "data.h"
#include "Observations.hpp"
#include "Sampler.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace Catch::Matchers;

template <typename REAL>
REAL param_test_model_func(REAL x, std::array<REAL, 2> params){
    return params[0] * x + params[1];
}

template <typename REAL>
REAL param_3_test_model_func(REAL x, std::array<REAL, 3> params){
    return params[0] * x* x + params[1] *x + params[2];
}

template<typename REAL>
void checkFileContents(const Observations<REAL> &obs)
{
    REQUIRE(obs.inputs.size() == 3);
    REQUIRE(obs.outputs.size() == 3);
    REQUIRE(obs.sigmas.size() == 3);

    REQUIRE_THAT(obs.inputs[0], WithinRel(0.94908, 1e-5));
    REQUIRE_THAT(obs.inputs[1], WithinRel(0.49062, 1e-5));
    REQUIRE_THAT(obs.inputs[2], WithinRel(0.98349, 1e-5));

    REQUIRE_THAT(obs.outputs[0], WithinRel(0.97454, 1e-5));
    REQUIRE_THAT(obs.outputs[1], WithinRel(0.74531, 1e-5));
    REQUIRE_THAT(obs.outputs[2], WithinRel(0.99174, 1e-5));

    for(auto sig : obs.sigmas)
    {
        REQUIRE_THAT(sig, WithinRel(1.0, 1e-6));
    }
}

template<typename REAL, std::size_t num_params>
void print_likelihood_map(std::map<std::array<REAL, num_params>, REAL> myMap){
    for (const auto& pair : myMap) {
        if (num_params == 1){
            std::cout << "(" << pair.first[0] << "): " << pair.second << std::endl;
        }
        else{
            std::cout << "(" << pair.first[0];
            for (std::size_t i = 1; i < num_params; i++){
                std::cout << ", " << pair.first[i];
            }
            std::cout << "): " << pair.second << std::endl;
        }
    }
}

template<typename REAL, std::size_t num_params>
void check_sampled_points(std::map<std::array<REAL, num_params>, REAL>& myMap, std::vector<std::array<REAL,num_params>>& validation_points){
    uint j = 0;
    for (const auto& pair:myMap){
        for (std::size_t i = 1; i < num_params; i++){
            CHECK_THAT(pair.first[i], WithinRel(validation_points[j][i],1e-5));
        }
        j +=1;
    }
}

template<typename REAL, std::size_t num_rows>
void checkFileContents1(const Observations<REAL> &obs,const std::array<REAL, num_rows> &in_check,const std::array<REAL, num_rows> &out_check,const std::array<REAL, num_rows> &sig_check){
    REQUIRE(obs.inputs.size() == num_rows);
    REQUIRE(obs.outputs.size() == num_rows);
    REQUIRE(obs.sigmas.size() == num_rows);

    for (std::size_t i = 0; i<num_rows; i++){
        REQUIRE_THAT(obs.inputs[i], WithinRel(in_check[i],1e-6));
        REQUIRE_THAT(obs.outputs[i], WithinRel(out_check[i],1e-6));
        REQUIRE_THAT(obs.sigmas[i], WithinRel(sig_check[i],1e-6));
    }
}

TEST_CASE("Test file reader (double)", "[File Read]")
{
    Observations<double> obs;
    obs.loadData("test/test_data/testing_data_2D.txt");

    checkFileContents(obs);
}

TEST_CASE("Test file reader (float)", "[File Read]")
{
    Observations<float> obs;
    obs.loadData("test/test_data/testing_data_2D.txt");

    checkFileContents(obs);
}

TEST_CASE("Test file reader with incorrect path given.","[File Read]"){
    Observations<float> obs;
    REQUIRE_THROWS_AS(obs.loadData("test/test_data/no_file.txt"),std::runtime_error);
    REQUIRE_THROWS_WITH(obs.loadData("test/test_data/no_file.txt"),"Unable to open file: test/test_data/no_file.txt");

}

TEST_CASE("Test file reader (float) on csv", "[File Read]"){
    Observations<float> obs;
    REQUIRE_THROWS_AS(obs.loadData("test/test_data/testing_data_2D.csv"),std::invalid_argument);
    REQUIRE_THROWS_WITH(obs.loadData("test/test_data/testing_data_2D.csv"),"Incorrect file extension: .csv instead of .txt for file test/test_data/testing_data_2D.csv .");
}

TEST_CASE("Test file reader (double) when reading ints", "[File Read]"){
    Observations<double> obs;
    obs.loadData("test/test_data/testing_data.txt");
    std::array<double, 4> in = {1.0, 2.0, 9.0, 12.0};
    std::array<double, 4> out = {2.0, 5.0, 99.0, 33.0};
    std::array<double, 4> sig = {3.0, 3.0, 3.0, 45.0};

    checkFileContents1<double, 4>(obs,in ,out,sig);
}

TEST_CASE("Test file reader (double) with excess lines", "[File Read]"){
    Observations<double> obs;
    std::array<double, 3> in_check = {9.490792840979749290e-01, 4.906167379139929619e-01, 9.834871049063151904e-01};
    std::array<double, 3> out_check = {9.745396420489874645e-01, 7.453083689569964809e-01, 9.917435524531575952e-01};
    std::array<double, 3> sig_check = {1.0, 1.0, 1.0};

    //redirect cerr to different stream
    auto originalCerrBuff = std::cerr.rdbuf();
    std::ostringstream capturedOutput;
    std::cerr.rdbuf(capturedOutput.rdbuf());
    obs.loadData("test/test_data/testing_data1.txt");
    checkFileContents1<double, 3>(obs,in_check, out_check, sig_check);
    std::cerr.rdbuf(originalCerrBuff);
    std::string expectedOutput = "Unexpected data exceeding three feature x, y and sigma format in line 1 :  9.490792840979749290e-01 9.745396420489874645e-01 1.000000000000000000e+00 7.453083689569964809e-01\n";
    CHECK(capturedOutput.str() == expectedOutput);
}

TEST_CASE("Test file reader (double) with incomplete first, fourth and fith row","[File Read]"){
    Observations<double> obs;

    std::array<double, 2> in_check = {4.906167379139929619e-01, 9.834871049063151904e-01};
    std::array<double, 2> out_check = {7.453083689569964809e-01, 9.917435524531575952e-01};
    std::array<double, 2> sig_check = {1.000000000000000000e+00, 1.000000000000000000e+00};

    auto originalCerrBuff = std::cerr.rdbuf();
    std::ostringstream capturedOutput;
    std::cerr.rdbuf(capturedOutput.rdbuf());
    obs.loadData("test/test_data/testing_data2.txt");
    checkFileContents1<double, 2>(obs, in_check, out_check, sig_check);
    std::cerr.rdbuf(originalCerrBuff);
    std::string expectedOutput = "Skipping row - Error reading sigma data from line 1 : 9.490792840979749290e-01 9.745396420489874645e-01\nSkipping row - Error reading output data from line 4 : 9.834871049063151904e-01\nSkipping row - Error reading output data from line 5 : 9.834871049063151904e-01 N/A\n";
    CHECK(capturedOutput.str() == expectedOutput);
}

TEST_CASE("Test file reader (double) with letter string entries.", "[File Read]"){    
    Observations<double> obs;

    //redirect cerr to different stream
    auto originalCerrBuff = std::cerr.rdbuf();
    std::ostringstream capturedOutput;
    std::cerr.rdbuf(capturedOutput.rdbuf());
    obs.loadData("test/test_data/testing_data3.txt");
    std::cerr.rdbuf(originalCerrBuff);
    std::string expectedOutput = "Skipping row - Error reading input data from line 1 : abs dd s\nSkipping row - Error reading input data from line 2 : s sdd ff\nSkipping row - Error reading input data from line 3 : d ddd ww\n";
    CHECK(capturedOutput.str() == expectedOutput);
}

TEST_CASE("Test file reader (double) with negative sigma value in second row", "[File Read]"){
    Observations<double> obs;

    std::array<double, 3> in_check = {9.490792840979749290e-01, 9.834871049063151904e-01, 9.490792840979749290e-01};
    std::array<double, 3> out_check = {9.745396420489874645e-01, 9.917435524531575952e-01, 9.745396420489874645e-01};
    std::array<double, 3> sig_check = {1.000000000000000000e+00, 3.000000000000000000e+00, 1.000000000000000000e+00};

    auto originalCerrBuff = std::cerr.rdbuf();
    std::ostringstream capturedOutput;
    std::cerr.rdbuf(capturedOutput.rdbuf());
    obs.loadData("test/test_data/testing_data4.txt");
    checkFileContents1<double, 3>(obs, in_check, out_check, sig_check);
    std::cerr.rdbuf(originalCerrBuff);
    std::string expectedOutput = "Skipping row - Sigma value is negative where standard deviation is inherently positive in line 2 : 4.906167379139929619e-01 7.453083689569964809e-01 -2.000000000000000000e+00\n";
    CHECK(capturedOutput.str() == expectedOutput);

}

// TEST_CASE("Test Sampler constructor", "[Sampler]"){
//     int placeholder;
// }

TEST_CASE("Preliminary Test uniform sampling 1 to 2 dimensions with range (0,1)","[Uniform Sampler]"){
    // instantiate Uniform Sampler with double data type and two parameters.
    std::array<std::string, 2> names = {"a", "b"};
    std::array<double, 2> min_vals = {0,0};
    std::array<double, 2> max_vals = {1, 1};
    UniformSampler<double, 2> uniform_sampler("data/problem_data_2D.txt",param_2_model_func<double>,names, min_vals, max_vals,2);
    uniform_sampler.sample();
    std::map<std::array<double, 2>,double> likelihood_map = uniform_sampler.get_param_likelihood();
    
    //capture print
    auto originalCoutBuff = std::cout.rdbuf();
    std::ostringstream capturedOutput;
    std::cout.rdbuf(capturedOutput.rdbuf());
    print_likelihood_map<double, 2>(likelihood_map);
    std::cout.rdbuf(originalCoutBuff);
    std::string expected_output = "(0.25, 0.25): -2767.73\n(0.25, 0.75): -2776.7\n(0.75, 0.25): -2074.02\n(0.75, 0.75): -1586.32\n";
    CHECK(capturedOutput.str() == expected_output);

    // instantiate Uniform Sampler with double data type and 1 parameter.
    std::array<std::string, 1> names2 = {"a"};
    std::array<double, 1> min_vals2 = {0};
    std::array<double, 1> max_vals2 = {1};
    UniformSampler<double, 1> uniform_sampler2("data/problem_data_2D.txt",param_1_model_func<double>,names2, min_vals2, max_vals2,5);
    uniform_sampler2.sample();
    std::map<std::array<double, 1>,double> likelihood_map2 = uniform_sampler2.get_param_likelihood();
    
    //capture print
    auto originalCoutBuff2 = std::cout.rdbuf();
    std::ostringstream capturedOutput2;
    std::cout.rdbuf(capturedOutput2.rdbuf());
    print_likelihood_map<double, 1>(likelihood_map2);
    std::cout.rdbuf(originalCoutBuff2);
    std::string expected_output2 = "(0.1): -3063.44\n(0.3): -2182\n(0.5): -1695.09\n(0.7): -1411.3\n(0.9): -1242.56\n";
    CHECK(capturedOutput2.str() == expected_output2);
}

TEST_CASE("Test Sample Function for Uniform Sampler from 2 to 3 dimensions", "[Uniform Sampler]"){
    //2 dimension param space, 3 bins, 0-1 range
    std::array<std::string, 2> names = {"a", "b"};
    std::array<double, 2> min_vals = {0,0};
    std::array<double, 2> max_vals = {1, 1};
    UniformSampler<double, 2> uniform_sampler("test/test_data/testing_data_2D.txt",param_2_model_func<double>,names, min_vals, max_vals,3);
    uniform_sampler.sample();
    std::map<std::array<double, 2>,double> likelihood_map = uniform_sampler.get_param_likelihood();
    std::vector<std::array<double, 2>> validation_points = {{1.0/6.0,1.0/6.0},{1.0/6.0,1.0/2.0},{1.0/6.0,5.0/6.0},{1.0/2.0,1.0/6.0},{1.0/2.0,1.0/2.0},{1.0/2.0,5.0/6.0},{5.0/6.0,1.0/6.0},{5.0/6.0,1.0/2.0},{5.0/6.0,5.0/6.0}};
    check_sampled_points<double,2>(likelihood_map, validation_points);

    //3 dimension, 2 bins, 0-1 range
    std::array<std::string, 3> names2 = {"a","b","c"};
    std::array<double, 3> min_vals2 = {0,0,0};
    std::array<double, 3> max_vals2 = {1,1,1};
    UniformSampler<double, 3> uniform_sampler2("test/test_data/testing_data_2D.txt",param_3_test_model_func<double>,names2, min_vals2, max_vals2,2);
    uniform_sampler2.sample();
    std::map<std::array<double, 3>,double> likelihood_map2 = uniform_sampler2.get_param_likelihood();
    std::vector<std::array<double,3>> validation_points2 = {{0.25,0.25,0.25},{0.25,0.25,0.75},{0.25,0.75,0.25},{0.25,0.75,0.75},{0.75,0.25,0.25},{0.75,0.25,0.75},{0.75,0.75,0.25},{0.75,0.75,0.75}};
    
    check_sampled_points<double,3>(likelihood_map2,validation_points2);
    
}

TEST_CASE("Test likelihood function","[Likelihood Calc]"){
    test_data test_likelihood;
    std::array<std::string, 2> names = {"a", "b"};
    std::array<double, 2> min_vals = {0,0};
    std::array<double, 2> max_vals = {1, 1};
    UniformSampler<double, 2> uniform_sampler("test/test_data/testing_data_2D.txt", param_test_model_func<double>, names, min_vals, max_vals, 3);  
    
    for (uint i = 0; i < test_likelihood.sample_likelihoods.size(); i++){
        CHECK_THAT(std::exp(uniform_sampler.log_likelihood(test_likelihood.sample_points_2d[i])),WithinRel(test_likelihood.sample_likelihoods[i],0.01)); // e is raised to the power of the log-likelihood to get the likelihood.
    }
}
