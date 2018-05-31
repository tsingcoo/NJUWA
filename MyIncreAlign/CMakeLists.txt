cmake_minimum_required(VERSION 3.8)
project(myIncreAlign)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp incre_model1.cpp incre_model1.h incre_model2.cpp incre_model2.h incre_modelh.cpp incre_modelh.h)
add_executable(myIncreAlign ${SOURCE_FILES})