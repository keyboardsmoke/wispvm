cmake_minimum_required(VERSION 3.10)
project(compiler)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

include_directories(${CMAKE_SOURCE_DIR})

set(COMPILER_FILES
        ${CMAKE_SOURCE_DIR}/shared/platform.h
        ${CMAKE_SOURCE_DIR}/shared/types.h
        lexer.h
        lexer.cpp
        compiler.h
        compiler.cpp)

add_library(compiler STATIC ${COMPILER_FILES})