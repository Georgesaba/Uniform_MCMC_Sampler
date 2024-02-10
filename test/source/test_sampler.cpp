#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>
#include "Observations.hpp"

using namespace Catch::Matchers;

template<typename REAL>
void checkFileContents(Observations<REAL> &obs)
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