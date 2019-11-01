cmake_minimum_required(VERSION 3.15)
project(mergesort)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(mergesort
        Buffer.cpp
        Buffer.h
        File.cpp
        File.h
        globals.cpp
        globals.h
        NaturalMergeSort.cpp
        Probability.cpp
        Probability.h
        ReadBuffer.cpp
        ReadBuffer.h
        WriteBuffer.cpp
        WriteBuffer.h)
