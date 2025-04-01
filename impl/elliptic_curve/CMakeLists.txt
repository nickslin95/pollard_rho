add_library(elliptic_curve
    ec_point.cpp
    field.cpp
)

target_link_libraries(elliptic_curve PUBLIC extended_euclidean)
target_include_directories(elliptic_curve PUBLIC ${CMAKE_SOURCE_DIR}/impl)

add_executable(elliptic_curve_test test.cpp)
target_link_libraries(elliptic_curve_test PRIVATE elliptic_curve gtest gtest_main)