cmake_minimum_required(VERSION 3.5)



set( CMAKE_CXX_COMPILER "/usr/bin/g++" )
set( CMAKE_C_COMPILER "/usr/bin/gcc" )

project (SADAD)
include(CTest)


set(SOURCES
    sadad_coding_challenge.cpp
)

set(CMakeDemo_TEST_HEADER
    catch.hpp
)

add_executable(SADAD  ${CMakeDemo_TEST_HEADER} ${SOURCES})
#add_executable(SADAD  g++ --std=c++20 sadad_coding_challenge.cpp -o challenge && ./challenge)

enable_testing()

add_test(NAME SADAD  COMMAND SADAD)

target_include_directories(SADAD
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
)