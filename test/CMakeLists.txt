add_executable(TestSampler source/test_sampler.cpp source/data.cpp)
target_include_directories(TestSampler PUBLIC ${CMAKE_SOURCE_DIR}/test/include)
target_link_libraries(TestSampler PUBLIC SamplerLib Catch2::Catch2WithMain)