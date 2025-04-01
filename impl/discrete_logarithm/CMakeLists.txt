add_library(discrete_logarithm
    dl_finder.cpp
)

target_link_libraries(discrete_logarithm PUBLIC elliptic_curve extended_euclidean)
target_include_directories(discrete_logarithm PUBLIC ${CMAKE_SOURCE_DIR}/impl)

add_executable(discrete_logarithm_test test.cpp)
target_link_libraries(discrete_logarithm_test PRIVATE discrete_logarithm gtest gtest_main)