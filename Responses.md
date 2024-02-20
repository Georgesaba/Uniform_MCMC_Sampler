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

There are no member variables that are public as everything can be accessed from setter and getter member functions. The variable that stores the model function, the number of bins and the std::array containing the ParamInfo datatype is private as they can all be accessed using get and set functions and do not need to be directly accessed in the derived class. The std::map linking the param vector to the likelihood and the variable containing the pointer to the observations type variable are protected as they may need to be accessd by the derived class. The set_param_info(), constructor(), get_bins(), get_param_info(), set_bins() and get_param_likelihood member classes are all public as they may need to be accessed out of the sampler classes. The member function used to calculate the log lieklihood is public as it needs to be accessed out of the class for validation of the likelihood function but I would usually keep it protected as it only needs to be accessed by derived classes.

## Section 3.0.1

I tested the Uniform Sampler class on the data/problem_data_2D.txt data using two parameters with ranges between 0 and 5 on the function $y = ax^b$. I then compared the summarised results with the outcome of a python curve_fitting algorithm that used the scipy.optimise library. The output that we are the most interested in is the mean parameter weighted using the marginal distribution. The code I used can be seen below:
Input ---

import numpy as np
from scipy.optimize import curve_fit


def model_func(x, a, b):
    return a * x**b

data = np.loadtxt("COMP0210Assignment1/data/problem_data_2D.txt")

inputs = data[:,0]
outputs = data[:,1]

params, covariance = curve_fit(model_func, inputs, outputs)

print(params) 

Output -- [2.50415752 4.12758947]

## Section 4

## Section 6