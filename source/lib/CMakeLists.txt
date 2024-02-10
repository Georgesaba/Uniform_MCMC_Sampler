add_library(SamplerLib Observations.cpp)
target_link_libraries(SamplerLib PUBLIC matplot)
target_include_directories(SamplerLib PUBLIC ${CMAKE_SOURCE_DIR}/include)