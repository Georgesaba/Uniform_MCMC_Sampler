# Sampling Method for Inference

## Build Instructions

To build the project containing both the Sample2D and Sample4D applications it is necessary to have cmake installed. The minimum verson required for this project is 3.16. Inside the same directory as this read me file run cmake -B build to configure the project and create the build directory. To compile run cmake --build build. Now you should have three applications. Sample2D, Sample4D and TestSampler. Sample2D and Sample4D are the command line applications and TestSampler just contains unit tests.


Note: Both applications make use of the rigidity setting. This setting depends on how reliable you consider the rest of the data you reference to be if there are errors in some rows of data. I included this setting as in practical applications all data have issues and otherwise reliable data that had thousands of rows might not want to be rendered useless through a few bad lines. However if there is an issue that carries through to the rest of the dataset then rigidity is best left on.


## Sample2D

This application is built to fit data to a function with two parameters. The function that is being fitted is $f(x) = ax^b$. The sampling technique used here is Uniform Sampling where the entire discretised parameter space is sampled with marginal distributions being computed for each parameter. This application should use data located at the following local path: data/problem_data_2D.txt however can be fed other files if that data believes to fit the same function type. It is best advises to store the data in the data folder however will still function if at the same file level as this README. If located elsewhere an absolute path is needed. 

To run this application you need to use the command line flags -n and -f to specify the number of bins and flags respectively. These two flags are the only essential flags for this application. The -h flag can be used find brief run instructions and the defaults of the optional parameters. Below is an example:


######################################################################################################## \n
This program uses uniform sampling to fit data to the equation y = ax^b with default parameter ranges from 0 to 5. \n
The data should come in the format of a txt file with columns inputs (x), outputs (y) or error (σ). \n

Brief instructions can be found below. \n
Usage: Sample2D -f <file_path> -n <number_of_bins> \n
Options: \n
  -h                Show this help message \n
  -f  <path>        Path to the data file \n
  -n  <bins>        Number of bins for sampling \n
  -ar <upper,lower> Range for parameter a             (optional: default = 0,5) \n
  -br <upper,lower> Range for parameter b             (optional: default = 0,5) \n
  -p  <plot>        Plot condition (Y/N)              (optional: default = Y) \n
  -g  <rigidity>    Strictness when Reading Data File (optional: default = false)  \n
########################################################################################################

-ar and -br are the flags for the range of parameters a and b respectively. There are also flags -p and -g which are the plot conditions and rigidity settings respectively.

The plot condition determines whether the distributions and fitted data is plotted by the application and the rigidity setting determines how harsh the error handling is when files are being read. A false rigidity setting means that lines with missing or faulty data get skipped with error messages printed that highlight the error but the file still ends up being read. A true setting means that the program halts as soon as a data irregularity is spotted with the details of the problem line printed.

#### Examples:

In the same directory as this read me the below command will sample 100 bins across the full two parameter space using data from a specified file. It is the bare minimum of input required. The space will be sampled between the range(0,5) for each parameter if the range is not speciified.

It should be noted that the order does not matter when running the flags. Make sure there is a space between each flag and its value. Use of an invalid flag will terminate the program. 

########################################################################################################
`./build/bin/Sample2D -f data/problem_data_2D.txt -n 100`
########################################################################################################

Extra settings such as a range(2,3) for parameter a and range(4,5.5) for parameter b, switching plotting off and enabling the rigidity setting can be triggered as below.

########################################################################################################
`./build/bin/Sample2D -f data/problem_data_2D.txt -n 100 -ar 2,3 -br 4,5.5 -p N -g True`
########################################################################################################

Below a possible output of the application can be seen. The mean values of the distributions indicate the most probable values for the parameters to take and the standard deviation gives the error of the respective parameter. Parameter at Marginal Distribution Peak tells us the bin that the peak of the distribution occupied. With plotting enabled plots of the data fitted to the function $f(x) = ax^b$ using the parameter means and of the individual marginal distributions of each parameter fitted to gaussian with the mean and standard deviation from the summary can be found. These are located in the plots/Sample2D folder. The subfolder CurveFit is for best fit lines and MarginalDistribution is for the distributions of each parameter. 

The CurveFit files have the format `fit_{a}_{param a low}_{param a high}_{b}_{param b low}_{param b high}_{number of bins}_y=ax^b.png`.

The MarginalDistribution files have the format `dist_{param name}_{param low}_{param high}_{number of bins}_y=ax^b.png`.

######################################################################################################## \n
Parameter a : \n
Standard Deviation - 0.0386534 \n
Mean - 2.50513 \n
Parameter at Marginal Distribution Peak - 2.525 \n

Parameter b : \n
Standard Deviation - 0.114997 \n
Mean - 4.13374 \n
Parameter at Marginal Distribution Peak - 4.125 \n
######################################################################################################## \n

## Sample4D

This application is built to fit data to a cubic functions wth 4 parameters. It can be described by $f(x) = ax^3 + bx^2 + cx + d$. This application uses both the Uniform Sampling Markov Chain Monte Carlo methods based on the size of the parameter space in relation to the maximum number of sampled points in relation to the user. If the user inputted maximum number of sample points is less than the number of bins raised to the power of the number of parameters the Markov Chanin Monte Carlo method is used otherwise the Uniform Sampling method is used. The marginal distribution of the MCMC method is based off how many times each bin is sampled. This application should use the data stored in the path data/problem_data_4D.txt however can be fed other files if that data believes to fit the same function type. It is best advises to store the data in the data folder however will still function if at the same file level as this README. If located elsewhere an absolute path is needed.

To run this program three command line flags are absolutely essential: -f for the filename, -n for the number of bins and -s for the maximum number of points to sample. Extra additional flags include -ar, -br, -cr, -dr which are the ranges for all the parameters. There are also flags -p and -g which are the plot conditions and rigidity settings respectively. The plot condition determines whether the distributions and fitted data is plotted by the application and the rigidity setting determines how harsh the error handling is when files are being read. A false rigidity setting means that lines with missing or faulty data get skipped with error messages printed that highlight the error but the file still ends up being read. A true setting means that the program halts as soon as a data irregularity is spotted with the details of the problem line printed.


Using the -h flag or invalid command line arguments being passed will yield a help message such as the one shown below:

########################################################################################################
This program uses uniform sampling to fit data to the equation y = ax^3 + bx^2 + cx + d with default parameter ranges from 0 to 5.\n
The step size used for the Metropolis Hastings Sampler is 0.01. \n
The data should come in the format of a txt file with columns inputs (x), outputs (y) or error (σ). \n

Brief instructions can be found below.\n
Usage: Sample4D -f <file_path> -n <number_of_bins> -s <number of samples>\n
Options:\n
  -h                       Show this help message \n
  -f  <path>               Path to the data file \n
  -n  <bins>               Number of bins for sampling \n
  -s  <number_samples>     Number of Samples to take \n
  -ar <upper,lower>        Range for parameter a                       (optional: default = -3,3) \n
  -br <upper,lower>        Range for parameter b                       (optional: default = -3,3) \n
  -cr <upper,lower>        Range for parameter c                       (optional: default = -3,3) \n
  -dr <upper,lower>        Range for parameter d                       (optional: default = -3,3) \n
  -p  <plot>               Plot condition (Y/N)                        (optional: default = Y) \n
  -g  <rigidity>           Strictness when Reading Data File (Bool)    (optional: default = false) \n
########################################################################################################

#### Examples:

The below command will use the MCMC Metropolis Hastings Sampling method to sample 200,000 points with each parameter range discretised into 100 bins. As a uniform approach would need to sample too many points so would be incredibly inefficient the MCMC method is used. The parameter space will automatically have range(-3,3) for each parameter if not specified otherwise. It should be noted that the order does not matter when running the flags. Make sure there is a space between each flag and its value. Use of an invalid flag will terminate the program. This should print "Metropolis Hastings Sampler Initiated" showing which sampler is being used.

########################################################################################################
`./build/bin/Sample4D -f data/problem_data_4D.txt -n 100 -s 200000`
########################################################################################################

The below command will instead sample using the Uniform Sampler as $20^4$ is less than 200,000. It should print "Uniform Sampler Initiated".

########################################################################################################
`./build/bin/Sample4D -f data/problem_data_4D.txt -n 20 -s 200000`
########################################################################################################


Extra settings such as setting ranges of (-1.2,0.5); (1.5,2.1); (-0.3,0.3); (0.8, 1.2) for parameters a, b, c and d respectively or turning plotting off and enabling rigidity for reading data can be implemented as shown below. Functionality to change the step-size is not included however is possible with the libraries this application uses.

########################################################################################################
`./build/bin/Sample4D -f data/problem_data_4D.txt -n 100 -s 200000 -ar -1.2,0.5 -br 1.5,2.1 -cr -0.3,0.3 -dr 0.8,1.2 -p N -g true`
########################################################################################################


Below a possible output of the application can be seen. The mean values of the distributions indicate the most probable values for the parameters to take and the standard deviation gives the error of the respective parameter. Parameter at Marginal Distribution Peak tells us the bin that the peak of the distribution occupied. With plotting enabled plots of the data fitted to the function $f(x) = ax^3 + bx^2 + cx + d$ using the parameter means and of the individual marginal distributions of each parameter fitted to gaussian with the mean and standard deviation from the summary can be found. These are located in the plots/Sample4D folder. The subfolder MHS is for results obtained using the MCMC MHS sampler and the subfolder Uniform is for results obtained using uniform sampling. The subfolder CurveFit is for best fit lines and MarginalDistribution is for the distributions of each parameter.

The Uniform/CurveFit files have the format `fit_a_{param a low}_{param a high}_b_{param b low}_{param b high}_c_{param c low}_{param c high}_d_{param d low}_{param d high}_{number of bins}.png`.

The Uniform/MarginalDistribution files should have the format `dist_{param name}_{param low}_{param high}_{number of bins}_cubic.png`.

The MHS/CurveFit files have the format `fit_a_{param a low}_{param a high}_b_{param b low}_{param b high}_c_{param c low}_{param c high}_d_{param d low}_{param d high}_{number of bins}_N_sample_{number of sample points}_step_size_0.01_cubic.png`.

The MHS/MarginalDistribution files have the format `dist_{param name}_N_sample_{number of sample points}_step_size_0.01_{param low}_{param high}_{number of bins}_cubic.png`.

########################################################################################################
Parameter a : \n
Standard Deviation - 0.423076 \n
Mean - -0.895843 \n
Parameter at Marginal Distribution Peak - -0.75 \n

Parameter b : \n
Standard Deviation - 0.616433 \n
Mean - 1.84729 \n
Parameter at Marginal Distribution Peak - 1.71  \n

Parameter c : \n
Standard Deviation - 0.266852 \n
Mean - 0.057443 \n
Parameter at Marginal Distribution Peak - 0.15 \n

Parameter d : \n
Standard Deviation - 0.0419465 \n
Mean - 0.995582 \n
Parameter at Marginal Distribution Peak - 0.99 \n
########################################################################################################