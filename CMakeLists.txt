project(accumulatorplus)

cmake_minimum_required(VERSION 3.10)

# Fix behavior of CMAKE_CXX_STANDARD when targeting macOS.
if (POLICY CMP0025)
  cmake_policy(SET CMP0025 NEW)
endif ()

#add_definitions(-Wfatal-errors)

set(CMAKE_CXX_STANDARD 14)

find_package(Boost)
IF (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
endif()

file(GLOB INCLUDES "include/*.h")
add_executable(tests 
    tests/AccumulatorTests.cpp  
    tests/CircularBufferTest.cpp 
    tests/AccumulatorPlusTests.cpp
    tests/catch.hpp 
    ${INCLUDES} )
target_include_directories(tests PUBLIC include)
