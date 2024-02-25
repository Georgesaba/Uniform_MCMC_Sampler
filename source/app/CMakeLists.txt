add_executable(Sample2D UniformSamplingApp.cpp)
target_include_directories(Sample2D PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(Sample2D PUBLIC SamplerLib)

add_executable(Sample4D MixedSamplingApp.cpp)
target_include_directories(Sample4D PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(Sample4D PUBLIC SamplerLib)