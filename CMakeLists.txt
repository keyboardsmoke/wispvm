cmake_minimum_required(VERSION 3.10)
project(WispVM)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Qspectre-")
endif()

set(CMAKE_EXE_LINKER_FLAGS "-static")

#add_subdirectory(${CMAKE_SOURCE_DIR}/ext/catch2)
add_subdirectory(${CMAKE_SOURCE_DIR}/ext/doctest)
add_subdirectory(${CMAKE_SOURCE_DIR}/vm)
add_subdirectory(${CMAKE_SOURCE_DIR}/compiler)
add_subdirectory(${CMAKE_SOURCE_DIR}/wisp)
add_subdirectory(${CMAKE_SOURCE_DIR}/unit_test)