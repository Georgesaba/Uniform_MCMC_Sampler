#include "Observations.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <assert.h>

template<typename REAL>
void Observations<REAL>::loadData(const std::string& filename)
{
    // check extension of file.
    std::string last_four_characters = filename.substr(filename.length() - 4);
    if (last_four_characters != ".txt"){
        std::string error_message = "Incorrect file extension: " + last_four_characters + " instead of .txt for file " + filename +  " .";
        throw std::invalid_argument(error_message);
    }
    
    //ifstream means input file stream
    std::ifstream filestream(filename);
    if (!filestream.is_open()) {
        
        std::string error_message = "Unable to open file: " + filename;
        throw std::runtime_error(error_message);
    }
    std::string line;
    uint row_num = 0;

    while(std::getline(filestream, line))
    {
        // A string stream can be used for parsing strings with multiple pieces of data in it
        // String steam is separated into three sections for x, y and sigma. First we check only 3 sections exist.
        
        std::istringstream line_stream(line);
        std::string token;
        row_num++;

        REAL buffer;
        if (!(line_stream >> buffer)){
            std::cerr << "Skipping row - Error reading input data from line " << row_num << " : " << line << std::endl;
            continue;
        } 
        inputs.push_back(buffer);
        
        if (!(line_stream >> buffer)){
            std::cerr << "Skipping row - Error reading output data from line " << row_num << " : " << line << std::endl;
            inputs.pop_back();
            continue;
        }
        outputs.push_back(buffer);
        
        if (!(line_stream >> buffer)){
            std::cerr << "Skipping row - Error reading sigma data from line " << row_num << " : " << line << std::endl;
            inputs.pop_back();
            outputs.pop_back();
            continue;
        }
        if (buffer < 0){
            std::cerr << "Skipping row - Sigma value is negative where standard deviation is inherently positive in line " << row_num << " : " << line << std::endl;
            inputs.pop_back();
            outputs.pop_back();
            continue;
        }
        sigmas.push_back(buffer);
        
        if (line_stream >> token){
            std::cerr << "Unexpected data exceeding three feature x, y and sigma format in line " << row_num << " :  " << line << std::endl;
        }
    }
    filestream.close();
    assert((sigmas.size() == outputs.size()) && (sigmas.size() == inputs.size()));
    num_points = sigmas.size();
}

template void Observations<double>::loadData(const std::string&);
template void Observations<float>::loadData(const std::string&);