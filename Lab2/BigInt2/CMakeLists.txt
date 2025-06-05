cmake_minimum_required(VERSION 3.28)
project(BigInt2)

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

add_library(bigint
        include/bigint.hpp
        src/bigint.cpp
)

target_include_directories(bigint
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(bigint_tests
        tests/test.cpp
)

target_link_libraries(bigint_tests
        PRIVATE bigint
        PRIVATE GTest::gtest_main
)

gtest_discover_tests(bigint_tests
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
