include(FetchContent)

cmake_minimum_required(VERSION 3.16)
project(Sampling
  VERSION 0.0.1
  LANGUAGES CXX
)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra")

find_package(Matplot++)
if(NOT Matplot++_FOUND)
    add_subdirectory(matplotplusplus)
endif()

add_subdirectory(source)

find_package(Catch2 3)
if(NOT Catch2_FOUND)
  FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.2)

  FetchContent_MakeAvailable(Catch2)
endif()

add_subdirectory(test)

target_compile_features(SamplerLib PUBLIC cxx_std_17)
set_target_properties(SamplerLib PROPERTIES CXX_STANDARD_REQUIRED ON CXX_EXTENSIONS OFF)
