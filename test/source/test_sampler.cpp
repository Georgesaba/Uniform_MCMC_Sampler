#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>
#include "Observations.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace Catch::Matchers;

template<typename REAL>
void checkFileContents(const Observations<REAL> &obs)
{
    REQUIRE(obs.inputs.size() == 3);
    REQUIRE(obs.outputs.size() == 3);
    REQUIRE(obs.sigmas.size() == 3);

    REQUIRE_THAT(obs.inputs[0], WithinRel(0.94907, 0.0001));
    REQUIRE_THAT(obs.inputs[1], WithinRel(0.49062, 0.0001));
    REQUIRE_THAT(obs.inputs[2], WithinRel(0.98349, 0.0001));

    REQUIRE_THAT(obs.outputs[0], WithinRel(0.97454, 0.0001));
    REQUIRE_THAT(obs.outputs[1], WithinRel(0.74531, 0.0001));
    REQUIRE_THAT(obs.outputs[2], WithinRel(0.99174, 0.0001));

    for(auto sig : obs.sigmas)
    {
        REQUIRE_THAT(sig, WithinRel(1.0, 0.0001));
    }
}

template<typename REAL, std::size_t num_rows>
void checkFileContents1(const Observations<REAL> &obs,const std::array<REAL, num_rows> &in_check,const std::array<REAL, num_rows> &out_check,const std::array<REAL, num_rows> &sig_check){
    REQUIRE(obs.inputs.size() == num_rows);
    REQUIRE(obs.outputs.size() == num_rows);
    REQUIRE(obs.sigmas.size() == num_rows);

    for (std::size_t i = 0; i<num_rows; i++){
        REQUIRE_THAT(obs.inputs[i], WithinRel(in_check[i],0.001));
        REQUIRE_THAT(obs.outputs[i], WithinRel(out_check[i],0.001));
        REQUIRE_THAT(obs.sigmas[i], WithinRel(sig_check[i],0.001));
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
    REQUIRE_THROWS_AS(obs.loadData("test/test_data/no_file.txt"),std::invalid_argument);
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
    std::string expectedOutput = "Unexpected data exceeding three feature x, y and sigma format in line:  9.490792840979749290e-01 9.745396420489874645e-01 1.000000000000000000e+00 7.453083689569964809e-01\n";
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

