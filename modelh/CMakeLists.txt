cmake_minimum_required(VERSION 3.8)
project(modelh)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp LeaveOneOut.h Commons.h Original.h LOOforKeyword.h PriorEM.h)
add_executable(modelh ${SOURCE_FILES})