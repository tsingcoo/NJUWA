cmake_minimum_required(VERSION 3.8)
project(model1)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp common.h original.h extendmethod.h sEM.h LOO.h iEM.h soriginal.h sEMwithLOO.h LOOforKeyword.h ext.h PriorEM.h)
add_executable(model1 ${SOURCE_FILES})