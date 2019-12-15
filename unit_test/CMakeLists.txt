cmake_minimum_required(VERSION 3.16)
project(unit_test)

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/unit_test)
include_directories(${CMAKE_SOURCE_DIR}/ext/doctest)

set(UNIT_TEST_FILES
        ${CMAKE_SOURCE_DIR}/shared/platform.h
        ${CMAKE_SOURCE_DIR}/shared/types.h
        vm/simple_vm.cpp
		compiler/scanner_tests.cpp
		compiler/tokenizer_tests.cpp
        main.cpp)

add_executable(unit_test ${UNIT_TEST_FILES})
target_link_libraries(unit_test vm)
target_link_libraries(unit_test compiler)
target_link_libraries(unit_test doctest)