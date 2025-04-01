add_library(long_arithmetic
    long_int.cpp
)

target_include_directories(long_arithmetic PUBLIC ${CMAKE_SOURCE_DIR}/impl /usr/local/include)
target_link_libraries(long_arithmetic PRIVATE /usr/local/lib/libgmp.dylib)

add_executable(long_arithmetic_test test.cpp)
target_link_libraries(long_arithmetic_test PRIVATE long_arithmetic gtest gtest_main /usr/local/lib/libgmp.dylib)