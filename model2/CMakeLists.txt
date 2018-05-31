cmake_minimum_required(VERSION 3.8)
project(model2)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp EMwithLOO.h common.h Original.h EMwithLOOlex.h LOOforKeyword.h PriorEM.h)
add_executable(model2 ${SOURCE_FILES})