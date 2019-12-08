cmake_minimum_required(VERSION 3.10)
project(wisp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

include_directories(${CMAKE_SOURCE_DIR})

set(WISP_FILES
        ${CMAKE_SOURCE_DIR}/shared/platform.h
        ${CMAKE_SOURCE_DIR}/shared/types.h
        main.cpp)

add_executable(wisp ${WISP_FILES})
target_link_libraries(wisp vm)
target_link_libraries(wisp compiler)