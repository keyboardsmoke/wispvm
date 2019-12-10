cmake_minimum_required(VERSION 3.10)
project(vm)

include_directories(${CMAKE_SOURCE_DIR})

set(VM_FILES
        ${CMAKE_SOURCE_DIR}/shared/platform.h
        ${CMAKE_SOURCE_DIR}/shared/types.h
        ${CMAKE_SOURCE_DIR}/shared/random.h
        ${CMAKE_SOURCE_DIR}/shared/decoder.h
        ${CMAKE_SOURCE_DIR}/shared/hash/crc32.h
        register.h
        register.cpp
        register_math.h
        stack.h
        stack.cpp
        state.h
        state.cpp
        instruction.h
        instruction.cpp
        vm.h
        vm.cpp)

add_library(vm STATIC ${VM_FILES})

set_target_properties(vm PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)