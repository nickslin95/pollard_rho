add_library(extended_euclidean
    extended_euclidean.cpp
)

target_link_libraries(extended_euclidean PUBLIC long_arithmetic)
target_include_directories(extended_euclidean PUBLIC ${CMAKE_SOURCE_DIR}/impl)

add_executable(extended_euclidean_test test.cpp) 
target_link_libraries(extended_euclidean_test PRIVATE extended_euclidean gtest gtest_main)