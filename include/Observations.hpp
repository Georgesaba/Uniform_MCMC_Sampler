#pragma once

#include <vector>
#include <string>

/**
* @brief Class for holding results of obersvations in three lists
* for inputs / outputs / standard deviations.
**/
template<typename REAL>
class Observations
{
public:
    uint num_points;
    std::vector<REAL> inputs;
    std::vector<REAL> outputs;
    std::vector<REAL> sigmas;

    /**
     * @brief Member function used to load data from file into Observation class.
     * @param filepath: Filepath of the data that is fitted to the provided function.
     * @param rigidity: The flexibility of the Observations object when it reads data. (optional: default = false)
    */
    void loadData(const std::string& filename, const bool rigidity = false);
};