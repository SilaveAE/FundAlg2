cmake_minimum_required(VERSION 3.31)
project(MyArray)

set(CMAKE_CXX_STANDARD 20)

enable_testing()

set(COMMON_FLAGS
        -Wall
        -Wextra
        -Wpedantic
        -Werror
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(COVERAGE_FLAGS
            --coverage
            -fprofile-arcs
            -ftest-coverage
    )
endif()


include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

add_library(my_array
        include/array.hpp
        include/container.hpp
        src/arr.cpp
)

target_include_directories(my_array
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(array_tests
    tests/testsarr.cpp
)

target_link_libraries(array_tests
    PRIVATE my_array
    PRIVATE GTest::gtest_main
)

gtest_discover_tests(array_tests
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
