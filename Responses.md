# Responses

Use this file for responding to questions in the assignment. 

## Section 1.1

1) Explain why it makes sense to store the observation data in std::vector instead of std::array?

It makes sense to store the observation data in std::vector instead of std::array because std::vector has a dynamic size with the ability to remove and add elements with member functions such as .push_back(), .pop_back(), .resize(), .clear() and .erase(). This is necessary because when the data is loaded it sequentially adds to the member variables inputs, outputs and sigma. This is needed so that the class can load files with variable sizes.

2) Explain why lines 27 & 28 are needed in Observations.cpp. How could we avoid this?

Lines 27 and 28, now 75 and 76, refer to explicit instantiations of the loadData method template for Observations class using the double and float data types. Since it is a member function of a class it needs to be explicitly instantiated for the relevant datatypes it will work with so that the compiler can generate function definitions for the template functions. Without the explicit instantiation, the linker has difficulty finding definitions of the loadData method for specific datatypes causing linking errors as there only is a class template available instead of a template class which is created with the explicit instantiation. To avoid this the full method logic can be placed in the header file where it will be instantiated automatically with the used type.

## Section 1.2

Why is sample a pure virtual function?

Sample is a pure virtual function because it is a place holder for various sampling methods that would be member functions of classes derived from the base Sampler class. Therefore the sample member function in the base Sampler class needs to be pure virtual as it must be completely overwritten but is necessary for every derived class to have.

What container type have you chosen to use to represent and pass the parameters (~p) to the model /
likelihood function, and why?

I have used an std::array container to represent and pass the parameters to the model as when a derived class is initialised the number of parameters is known so doesn't need the dynamic size that the std::vector container offers. The std::array takes up less memory so is more ideal in this case. I have used std::map that has a std::array of the sample point acting as a key to the corresponding likelihood value. An array makes sense over a tuple because it is easy to make its size modular and it contains a consistant data type. 

What variables have you made public, private, or protected and why?

There are no member variables that are public as everything can be accessed from setter and getter member functions. The variable that stores the model function, the number of bins and the std::array containing the ParamInfo datatype is private as they can all be accessed using get and set functions and do not need to be directly accessed in the derived class. The std::map linking the param vector to the likelihood and the variable containing the pointer to the observations type variable are protected as they may need to be accessd by the derived class. The set_param_info(), constructor(), get_bins(), get_param_info(), set_bins() and get_param_likelihood member classes are all public as they may need to be accessed out of the sampler classes. The member function used to calculate the log lieklihood is public as it needs to be accessed out of the class for validation of the likelihood function but I would usually keep it protected as it only needs to be accessed by derived classes. I have a member variable called extra settings which is private and can be accessed using a protected setter functio. This houses a map of extra parameters other derived sampling classes may have. This becomes useful for plotting later on.

## Section 3.0.1

I tested the Uniform Sampler class on the data/problem_data_2D.txt data using two parameters with ranges between 0 and 5 on the function $y = ax^b$. I then compared the summarised results with the outcome of a python curve_fitting algorithm that used the scipy.optimise library. The output that we are the most interested in is the mean parameter weighted using the marginal distribution. The code I used can be seen below:
Input ---

import numpy as np <br>
from scipy.optimize import curve_fit <br>


def model_func(x, a, b):<br>
    return a * x**b

data = np.loadtxt("COMP0210Assignment1/data/problem_data_2D.txt") <br>

inputs = data[:,0] <br>
outputs = data[:,1] <br>

params, covariance = curve_fit(model_func, inputs, outputs) <br>

print(params) 

Output -- [2.50415752 4.12758947]

## Section 4

The following runs will be to fit the equation $y=ax^b$ to the data in data/problem_data_2D.txt. Refer to the read me to understand the syntax for the file name housing the created plots.

#### Run 1:

For 10 bins being sampled with parameters a and b both having ranges between 0 and 5.

Parameter a : <br>
Standard Deviation - 0.0920166 <br>
Mean - 2.73245 <br>
Parameter at Marginal Distribution Peak - 2.75 <br>

Parameter b : <br>
Standard Deviation - 0.184192 <br>
Mean - 4.71476 <br>
Parameter at Marginal Distribution Peak - 4.75 <br>

#### Run 2:

For 50 bins being sampled with parameters and between ranges 0 and 5.

Parameter a : <br>
Standard Deviation - 0.049599 <br>
Mean - 2.50834 <br>
Parameter at Marginal Distribution Peak - 2.55 <br>

Parameter b : <br>
Standard Deviation - 0.130017 <br>
Mean - 4.14033 <br>
Parameter at Marginal Distribution Peak - 4.15 <br>

#### Run 3:

For 100 bins being sampled with parameters a and b both having ranges between 0 and 5.

Parameter a : <br>
Standard Deviation - 0.0386534 <br>
Mean - 2.50513 <br>
Parameter at Marginal Distribution Peak - 2.525 <br>

Parameter b : <br>
Standard Deviation - 0.114997 <br>
Mean - 4.13374 <br>
Parameter at Marginal Distribution Peak - 4.125 <br>

#### Run 4:

For 1000 bins being sampled with parameters a and b both having ranges between 0 and 5.

Parameter a : <br>
Standard Deviation - 0.0386467 <br>
Mean - 2.50513 <br>
Parameter at Marginal Distribution Peak - 2.5025 <br>

Parameter b : <br>
Standard Deviation - 0.114989 <br>
Mean - 4.13373 <br>
Parameter at Marginal Distribution Peak - 4.1275 <br>

#### Run 5:

For 10000 bins being sampled with parameters a and b both having ranges between 0 and 5. Note: This took a few minutes as it meant that 100,000,000 points were sampled. Given that the parameter means and standard deviations had hardly changed between 100 and 1000 the drastic increase to 10,000 bins is inefficient and not worth the computational resources. The main difference is how the bin midpoint is much closer to the mean which is to be expected. As we use the mean value to determine the paramter anyway and since this value doesn't vary much this run is not worth it. As each sampled point is recorded in the chain there is also an increase in memory overhead as 100,000,000 points need to be stored.
By the time the program finished running around 6gb of memory was being used by the application.

Parameter a : <br>
Standard Deviation - 0.0386467 <br>
Mean - 2.50513 <br>
Parameter at Marginal Distribution Peak - 2.50475 <br>

Parameter b : <br>
Standard Deviation - 0.114989 <br>
Mean - 4.13373 <br>
Parameter at Marginal Distribution Peak - 4.12875 <br>

#### Run 6

For 100 bins sampled with parameter a between 2 and 3 and parameter b between 3.5 and 4.5. These ranges are chosen from the results of the previous runs so that the parameter maxima and minima encompass the marginal distributions more tightly. A benefit of these runs is that we can reduce the bin size gaining more precision in the parameter at the marginal distribution peak whilst keeping the same computational load. This run can be seen below.

Parameter a : <br>
Standard Deviation - 0.038552 <br>
Mean - 2.50504 <br>
Parameter at Marginal Distribution Peak - 2.505 <br>

Parameter b : <br>
Standard Deviation - 0.114271 <br>
Mean - 4.13329 <br>
Parameter at Marginal Distribution Peak - 4.125 <br>

#### Run 7:

For 1000 bins sampled with parameter a between 2 and 3 and parameter b between 3.5 and 4.5. Same concept as above but even more granular.

Parameter a : <br>
Standard Deviation - 0.0385518 <br>
Mean - 2.50504 <br>
Parameter at Marginal Distribution Peak - 2.5045 <br>

Parameter b : <br>
Standard Deviation - 0.11427 <br>
Mean - 4.13329 <br>
Parameter at Marginal Distribution Peak - 4.1285 <br>

#### Run 8:

For 1000 bins sampled with parameter a between 2 and 3 and parameter b between 3.5 and 4.7. Same concept as above but even more granular.

Parameter a : <br>
Standard Deviation - 0.0386464 <br>
Mean - 2.50513 <br>
Parameter at Marginal Distribution Peak - 2.5045 <br>

Parameter b : <br>
Standard Deviation - 0.114986 <br>
Mean - 4.13373 <br>
Parameter at Marginal Distribution Peak - 4.1282 <br>


#### Extra Notes:

When the ranges are between 0 and 5 the mean parameter values get more and more accurate converging to the actual solution of [2.5042, 4.1276] obtained from the python curve fitter program when the bin number is increased up to 1000. When 10000 bins are used the mean stays the same with only the parameters with the highest marginal probabilities getting closer to the mean and "actual" solution as the bin size is much smaller. When the bounds of the distribution are changed ,as the mean is reflective of the entire distribution, the tails being cut off shifts the mean slightly however the bins are more granular. The mean of both parameters stay pretty consistence although as bin boundaries shift there are slight fluctuations. It is interesting to note how in Run 4 the peak of parameter b marginal distribution is closer to that of the python found value as compared to run 5 where 10 times the number of bins are used although the accuracy of the python value isnt guaranteed. In conclusion changing the parameter ranges only changes the mean if the distribution of one of the parameters is cut off however it has an impact on how the bins are determined. As the marginal distribution is dependent on how the other parameters are binned it is difficult to quantify the exact effect. More than 100 bins seems to be ineffective when looking at parameter means and even more so when 1000 bins are exceeded.

What does remain consistent is that the more bins there are the better the gaussian fits represent the data.

## Section 6

The following runs come from the Sample4D application and consist of the data from the path data/problem_data_4D.txt being fit to the equation $f(x) = ax^3 + bx^2 + cx + d$. The README.md can be used to determine which plots belong to which runs. This application will use a combination of the MCMC Metropolis Hasting Samper and the Uniform Sampler based on the size of the parameter space in relation to the desired amount of points to be sampled. The step size is always 0.01.

#### Run 1:

Ranges of all params between -3 and 3. 200,000 sample points and 100 bins. 

Metropolis Hastings Sampler Initiated <br>
Parameter a : <br>
Standard Deviation - 0.423076 <br>
Mean - -0.895843 <br>
Parameter at Marginal Distribution Peak - -0.75 <br>

Parameter b : <br>
Standard Deviation - 0.616433 <br>
Mean - 1.84729 <br>
Parameter at Marginal Distribution Peak - 1.71 <br>

Parameter c : <br>
Standard Deviation - 0.266852 <br>
Mean - 0.057443 <br>
Parameter at Marginal Distribution Peak - 0.15 <br>

Parameter d : <br>
Standard Deviation - 0.0419465 <br>
Mean - 0.995582 <br>
Parameter at Marginal Distribution Peak - 0.99 <br>

The fitted line here is very accurate and the histograms match their respective gaussain distributions although due to the nature of this sampling algorithm the distribution is not perfectly normal as this method involves targeted optimisation instead of sampling the entire space and as the marginal distribution is obtained from how many times each point is sampled there is bound to be slight skew. 

#### Run 2:

Ranges of all params between -3 and 3. 200,000 sample points and 20 bins

Uniform Sampler Initiated <br>
Parameter a : <br>
Standard Deviation - 0.265133 <br>
Mean - -0.696735 <br>
Parameter at Marginal Distribution Peak - -0.75 <br>

Parameter b : <br>
Standard Deviation - 0.405392 <br>
Mean - 1.91777 <br>
Parameter at Marginal Distribution Peak - 1.95 <br>

Parameter c : <br>
Standard Deviation - 0.151402 <br>
Mean - -0.171041 <br>
Parameter at Marginal Distribution Peak - -0.15 <br>

Parameter d : <br>
Standard Deviation - 0.000140601 <br>
Mean - 1.05 <br>
Parameter at Marginal Distribution Peak - 1.05 <br>

Although the histograms and gaussian fits for each parameter are very poor as 20 bins is not enough to sample each parameter, the fitted line appears to be roughly accurate.

#### Run 3:

As the distribution of paramater b seems to be cut off from the right I will change its range to (-3,5) from here on out. The next run will be 1,000,000 sample points and 1000 bins. This will use the MHS sampler as it would need 1,000,000,000,000 points to sample using a uniform sampling technique. As the acceptance criteria of the MHS method allows a new point to be accepted even if the log likelihood of the old point is higher than that of the new given that the difference is more than the log of a random number between 0 and 1 picked from a uniform distribution, increasing the number of points to be sampled introduces noise but also means that both tails of the distribution are explored more evenly.

Metropolis Hastings Sampler Initiated <br>
Parameter a : <br>
Standard Deviation - 0.592986 <br>
Mean - -0.933536 <br>
Parameter at Marginal Distribution Peak - -0.873 <br>

Parameter b : <br>
Standard Deviation - 0.882231 <br>
Mean - 1.89672 <br>
Parameter at Marginal Distribution Peak - 1.588 <br>

Parameter c : <br>
Standard Deviation - 0.373519 <br>
Mean - 0.0437748 <br>
Parameter at Marginal Distribution Peak - 0.105 <br>

Parameter d : <br>
Standard Deviation - 0.041382 <br>
Mean - 0.99598 <br>
Parameter at Marginal Distribution Peak - 1.005 <br>

#### Run 4:

Now I will run with 1000 bins and 200,000 sample points.

Metropolis Hastings Sampler Initiated
Parameter a : 
Standard Deviation - 0.552839
Mean - -1.06463
Parameter at Marginal Distribution Peak - -1.107

Parameter b : 
Standard Deviation - 0.824493
Mean - 2.09961
Parameter at Marginal Distribution Peak - 1.932

Parameter c : 
Standard Deviation - 0.353154
Mean - -0.0440812
Parameter at Marginal Distribution Peak - -0.207

Parameter d : 
Standard Deviation - 0.0417209
Mean - 1.00417
Parameter at Marginal Distribution Peak - 0.999

#### Run 5:

Now I will run with 100 bins and 1,000,000 sample points.

Metropolis Hastings Sampler Initiated <br>
Parameter a : <br>
Standard Deviation - 0.59316 <br>
Mean - -0.933597 <br>
Parameter at Marginal Distribution Peak - -0.75 <br>

Parameter b : <br>
Standard Deviation - 0.882406 <br>
Mean - 1.897 <br>
Parameter at Marginal Distribution Peak - 1.6 <br>

Parameter c : <br>
Standard Deviation - 0.373872 <br>
Mean - 0.0438234 <br>
Parameter at Marginal Distribution Peak - 0.03 <br>

Parameter d : <br>
Standard Deviation - 0.0449266 <br>
Mean - 0.996106 <br>
Parameter at Marginal Distribution Peak - 0.99 <br>

#### Run 6:

Now I will run with 50 bins and 10,000,000 sample points. At this point the UniformSampler's fit describes the data quite well however the histograms still do not follow the gaussain fit.

Uniform Sampler Initiated <br>
Parameter a : <br>
Standard Deviation - 0.489939 <br>
Mean - -1.13989 <br>
Parameter at Marginal Distribution Peak - -1.02 <br>

Parameter b : <br>
Standard Deviation - 0.694006 <br>
Mean - 2.23399 <br>
Parameter at Marginal Distribution Peak - 2.52 <br>

Parameter c : <br>
Standard Deviation - 0.270593 <br>
Mean - -0.11399 <br>
Parameter at Marginal Distribution Peak - -0.18 <br>

Parameter d : <br>
Standard Deviation - 0.0264012 <br>
Mean - 1.01447 <br>
Parameter at Marginal Distribution Peak - 1.02 <br>

#### Run 7:

Now I will run with 100 bins and 100,000,000. Note: This is effectively the complexity as the Sample2D example with 10,000 bins. This takes a lot of RAM and time to run so is not advisable to run but I just ran to compare directly with the MHS sampler over the same number of bins. Here the histograms follow the gaussian fit and data is well fit by the parameters. The standard deviation appears to now be higher with more points and the mean parameter values mathces the other results. In general these distributions and fits look worse than those obtained with 1,000,000 points over 100 bins with the Metropolis Hastings Sampler.

Uniform Sampler Initiated <br>
Parameter a : <br>
Standard Deviation - 0.569246 <br>
Mean - -0.99901 <br>
Parameter at Marginal Distribution Peak - -0.93 <br>

Parameter b : <br>
Standard Deviation - 0.847408 <br>
Mean - 1.99845 <br>
Parameter at Marginal Distribution Peak - 1.92 <br>

Parameter c : <br>
Standard Deviation - 0.359968 <br>
Mean - 0.000680935 <br>
Parameter at Marginal Distribution Peak - 0.09 <br>

Parameter d : <br>
Standard Deviation - 0.0400485 <br>
Mean - 0.999928 <br>
Parameter at Marginal Distribution Peak - 0.99 <br>

#### Run 8:

Now I will run 20 bins with 1000 sample points.

Metropolis Hastings Sampler Initiated <br>
Parameter a : <br>
Standard Deviation - 0.363737 <br>
Mean - 0.405045 <br>
Parameter at Marginal Distribution Peak - 0.15 <br>

Parameter b : <br>
Standard Deviation - 0.326268 <br>
Mean - 0.362038 <br>
Parameter at Marginal Distribution Peak - 0.4 <br>

Parameter c : <br>
Standard Deviation - 0.554462 <br>
Mean - 0.125425 <br>
Parameter at Marginal Distribution Peak - 0.45 <br>

Parameter d : <br>
Standard Deviation - 0.175777 <br>
Mean - 1.03022 <br>
Parameter at Marginal Distribution Peak - 1.05 <br>

## Run 9:

Now I will run 20 bins with 100,000 sample points:

Metropolis Hastings Sampler Initiated <br>
Parameter a : <br>
Standard Deviation - 0.511058 <br>
Mean - -1.06578 <br>
Parameter at Marginal Distribution Peak - -1.05 <br>

Parameter b : <br>
Standard Deviation - 0.750535 <br>
Mean - 2.11015 <br>
Parameter at Marginal Distribution Peak - 2.4 <br>

Parameter c : <br>
Standard Deviation - 0.323119 <br>
Mean - -0.0581889 <br>
Parameter at Marginal Distribution Peak - -0.15 <br>

Parameter d : <br>
Standard Deviation - 0.0225338 <br>
Mean - 1.04915 <br>
Parameter at Marginal Distribution Peak - 1.05 <br>

#### Conclusions:

The key difference between the Uniform Sampling and MCMC Metropolis Hastings Sampling techniques is that the Uniform Sampling technique discretises the parameter space into a certain number of bins and samples the whole space only through the midpoints. The MCMC Metropolis Hastings Sampling technique however operates on a continuous space and only records the bin that is currently being occupied to add to the marginal distribution. Therefore with a small amount of bins the MCMC sampler still has the continuous likelihood information availible to it causing it to hone in on a bin with more accuracy although the statistics will be lacking. The uniform sampling technique however will only sample the midpoints of the bins so at small bin numbers such as 20 it will have a very coarse grained parameter space with a potentially misleading distribution. At 20 bins with 1000 sample points the MCMC method had extremely inaccurate parameters and needed 100,000 sample points to have a similar amount of accuracy as the uniformly sampled distribution however the fit was still inferior as can be seen in the curve fit plots. There actually appears to be an offset in the fit which could be due to the correct points being sampled but then being assosciated with a bin midpoint value that is too far away from the actual value due to the large bin size. The uniformly sampled distributions with 50 and 100 bins were much more accurate with distributions that are fit the normal distribution more but the number of samples required was extremely high. 6.24 million for 50 and 100 million for 100 bins. The MCMC method however with 100 bins only needed 200,000 points to get a good fit for the parameters so a distribution with a good mean however the distributions have thinner tails than the corresponding gaussian distribution. 1,000,000 points with 100 bins resulted in a distribution that followed the gaussian fit well and that matched the data well however the median recorded is still far from the mean due to the bin width however the uniform sampling technique still has a cleaner distribution with a smaller standard deviation.  When the number of bins is increased to 1000 the histogram becomes more noisy possible as 1 million sample points is inefficient to have enough samples in each bin to build up a proper distribution however the gaussian fit still represents the distribution, the fit is very good however the parameter means remain the same. It should be noted that a larger amount of bins such as 1000 and a lower number of sample points is the worst combination as the mean starts becoming more inaccurate although these values themselves arent too bad. 

In conclusion the MCMC method is vastly superior as it allows you to get similar results and distributions to the Uniform Sampling technique at a fraction of the computational cost. Sampling 100 parameters with a similar representation of the paramter space likelihood can be done with 100 times less the amounts of parameters although the Uniform Sampler will always be more regular. This disparity gets larger the more bins that are used as the comlexity is proportional to $b^{n_p}$ where $b$ is the number of bins and $n_p$ is the number of params. When 100,000,000 points are sampled with the MCMC sampler using 1000 bins the distribution becomes a perfect fit for the gaussian function with no noise and the parameters appear to be perfectly fitted with a peak value extremely close to the mean. At this point even if those points were to be uniformly sampled there would be little room to improve althout 1,000,000,000,000 sample points would be required which is 10,000 times more. The disparity between the two samplers increases the more points there are. The 6,0000,0000 points MHS sample plots also provides a vastly superior fit to the 50 bin Uniform Sample that uses the same amount of points.
