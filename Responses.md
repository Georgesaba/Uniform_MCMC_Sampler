# Responses

Use this file for responding to questions in the assignment. 

## Section 1.1

1) Explain why it makes sense to store the observation data in std::vector instead of std::array?

It makes sense to store the observation data in std::vector instead of std::array because std::vector has a dynamic size with the ability to remove and add elements with member functions such as .push_back(), .pop_back(), .resize(), .clear() and .erase(). This is necessary because when the data is loaded it sequentially adds to the member variables inputs, outputs and sigma. This is needed so that the class can load files with variable sizes.

2) Explain why lines 27 & 28 are needed in Observations.cpp. How could we avoid this?

Lines 27 and 28 refer to explicit instantiations of the loadData method template for Observations class using the double and float data types. Since it is a member function of a class it needs to be explicitly instantiated for the relevant datatypes it will work with so that the compiler can generate function definitions for the template functions. Without the explicit instantiation, the linker has difficulty finding definitions of the loadData method for specific datatypes causing linking errors as there only is a class template available instead of a template class which is created with the explicit instantiation. To avoid this the full method logic can be placed in the header file where it will be instantiated automatically with the used type.

## Section 1.2

## Section 4

## Section 6