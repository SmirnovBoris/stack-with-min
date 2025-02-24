cmake_minimum_required(VERSION 3.20)

# Set the project name
project(stack_with_min)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(stack_with_min INTERFACE)
target_include_directories(stack_with_min INTERFACE ${PROJECT_SOURCE_DIR}/include)


enable_testing()

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
)
FetchContent_MakeAvailable(googletest)

file(GLOB test_SRC "tests/*.cpp" "tests/*.h")
add_executable(test_stack_with_min ${test_SRC})

target_link_libraries(test_stack_with_min gtest gtest_main stack_with_min)

add_test(NAME test_stack_with_min COMMAND test_stack_with_min)
