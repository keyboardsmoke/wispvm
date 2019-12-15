cmake_minimum_required(VERSION 3.10)
project(vm)

include_directories(${CMAKE_SOURCE_DIR})

set(VM_FILES
        ${CMAKE_SOURCE_DIR}/shared/platform.h
        ${CMAKE_SOURCE_DIR}/shared/types.h
        modules/memory.h
        modules/memory.cpp
        isa.h
        context.h
        vm.h
        vm.cpp)

add_library(vm STATIC ${VM_FILES})

set_target_properties(vm PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

if(MSVC)
  target_compile_options(vm PRIVATE /W4 /WX)
else()
  target_compile_options(vm PRIVATE -Wall -Wextra -pedantic -Werror)
endif()