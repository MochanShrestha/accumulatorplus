project(accumulatorplus)

cmake_minimum_required(VERSION 3.5)

# Fix behavior of CMAKE_CXX_STANDARD when targeting macOS.
if (POLICY CMP0025)
  cmake_policy(SET CMP0025 NEW)
endif ()

# Use in case way too many errors pours out that are long template errors
#add_definitions(-Wfatal-errors)

if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(Boost)
IF (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
else()
	message(WARNING "Boost headers not found by CMake. I'm hoping it's in the default path or you'll fix it after the build is created. For Windows, get boost nuget.")
endif()

file(GLOB INCLUDES "include/*.h")
add_executable(ap_tests 
    tests/AccumulatorTests.cpp  
    tests/CircularBufferTest.cpp 
    tests/AccumulatorPlusTests.cpp
    tests/catch.hpp 
    ${INCLUDES} )
target_include_directories(ap_tests PUBLIC include)
set_target_properties(ap_tests PROPERTIES CXX_STANDARD 11)

enable_testing()
add_test(ap_tests "${CMAKE_BINARY_DIR}/ap_tests")
