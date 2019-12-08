cmake_minimum_required(VERSION 3.10)
project(WispVM)

add_subdirectory(${CMAKE_SOURCE_DIR}/vm)
add_subdirectory(${CMAKE_SOURCE_DIR}/compiler)
add_subdirectory(${CMAKE_SOURCE_DIR}/wisp)