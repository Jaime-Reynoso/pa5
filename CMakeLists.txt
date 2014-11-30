cmake_minimum_required(VERSION 2.8.4)
project(pa5)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    bookorders.c
    bookorders.h
    uthash.h)

add_executable(pa5 ${SOURCE_FILES})