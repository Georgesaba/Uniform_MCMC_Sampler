# Sampling Method for Inference

## Build Instructions

To build the project containing both the Sample2D and Sample4D applications it is necessary to have cmake installed. The minimum verson required for this project is 3.16. Inside the same directory as this read me file run cmake -B build to configure the project and create the build directory. To compile run cmake --build build. Now you should have three applications. Sample2D, Sample4D and TestSampler. Sample2D and Sample4D are the command line applications and TestSampler just contains unit tests.


## Sample2D

This application is built to fit data to an exponential. The function that is being fitted is $f(x) = ax^b$. The sampling technique used here is Uniform Sampling where the entire discretised parameter space is sampled with marginal distributions being computed for each parameter. This application should use data located at the following local path: data/problem_data_2D.txt. 

To run this application you need to use the command line flags -n and -f to specify the number of bins and flags respectively. These two flags are the only essential flags for this application. The -h flag can be used find brief run instructions and the defaults of the optional parameters. Below is an example


#################################################################################################################
This program uses uniform sampling to fit data to the equation y = ax^b with default parameter ranges from 0 to 5. 
The data should come in the format of a txt file with columns inputs (x), outputs (y) or error (σ).

Brief instructions can be found below.
Usage: Sample2D -f <file_path> -n <number_of_bins>
Options:
  -h                Show this help message
  -f <path>         Path to the data file
  -n <bins>         Number of bins for sampling
  -ar <upper,lower> Range for parameter a             (optional: default = 0,5)
  -br <upper,lower> Range for parameter b             (optional: default = 0,5)
  -p  <plot>        Plot condition (Y/N)              (optional: default = Y)
  -g <rigidity>     Strictness when Reading Data File (optional: default = false)
#################################################################################################################

-ar and -br are the flags for the range of parameters a and b respectively. There are also flags -p and -g which are the plot conditions and rigidity settings respectively.

The plot condition determines whether the distributions and fitted data is plotted by the application and the rigidity setting determines how harsh the error handling is when files are being read. A false rigidity setting means that lines with missing or faulty data get skipped with error messages printed that highlight the error but the file still ends up being read. A true setting means that the program halts as soon as a data irregularity is spotted with the details of the problem line printed.

#### Examples:

In the same directory as this read me the below command will sample 100 bins across the full two parameter space from a specified file. It is the bare minimum of input required. The space will be sampled between the range(0,5) for each parameter.

It should be noted that the order does not matter when running the flags. Make sure there is a space between each flag and its value. Use of an invalid flag will terminate the program. 

#################################################################################################################
./build/bin/Sample2D -f data/problem_data_2D.txt -n 100
#################################################################################################################


Extra settings such as a range(2,3) for parameter a and range(4,5.5) for parameter b, switching plotting off and enabling the rigidity setting can be triggered as below.

#################################################################################################################
./build/bin/Sample2D -f data/problem_data_2D.txt -n 100 -ar 2,3 -br 4,5.5 -p false -g Y
#################################################################################################################

## Sample4D