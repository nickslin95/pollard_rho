include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.tar.gz
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_subdirectory(extended_euclidean)
add_subdirectory(elliptic_curve)
add_subdirectory(discrete_logarithm)
add_subdirectory(long_arithmetic)