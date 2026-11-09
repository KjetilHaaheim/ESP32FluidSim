cmake_minimum_required(VERSION 3.16)
project(fluid2d LANGUAGES CXX)

add_library(fluid2d STATIC
    fluid_solver.cpp
)

target_include_directories(fluid2d PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_compile_features(fluid2d PUBLIC cxx_std_11)
