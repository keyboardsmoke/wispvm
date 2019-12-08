cmake_minimum_required(VERSION 3.10)
project(vm)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

include_directories(${CMAKE_SOURCE_DIR})

set(VM_FILES
        ${CMAKE_SOURCE_DIR}/shared/platform.h
        ${CMAKE_SOURCE_DIR}/shared/types.h
        ${CMAKE_SOURCE_DIR}/shared/random.h
        ${CMAKE_SOURCE_DIR}/shared/decoder.h
        ${CMAKE_SOURCE_DIR}/shared/hash/crc32.h
        value.h
        value.cpp
        register.h
        register.cpp
        state.h
        state.cpp
        instruction.h
        instruction.cpp
        vm.h
        vm.cpp)

add_library(vm STATIC ${VM_FILES})