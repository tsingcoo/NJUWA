cmake_minimum_required(VERSION 3.9)
project(GenAlignPyCall)

set(CMAKE_CXX_STANDARD 11)

add_executable(GenAlignPyCall pre_process.h infer_align.h infer_align2.h infer_alignh.cpp infer_alignh.h)