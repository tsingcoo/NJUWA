cmake_minimum_required(VERSION 3.8)
project(GenAlign)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp pre_process.cpp pre_process.h distance.cpp distance.h infer_align.cpp infer_align.h vocab.cpp vocab.h two_vocab_infer_align.cpp two_vocab_infer_align.h two_vocab_infer_align.cpp two_vocab_infer_align.h two_index_infer_align.cpp two_index_infer_align.h two_index_infer_align.cpp two_index_infer_align.h infer_align2.cpp infer_align2.h infer_alignh.cpp infer_alignh.h pycall.cpp)
add_executable(GenAlign ${SOURCE_FILES})