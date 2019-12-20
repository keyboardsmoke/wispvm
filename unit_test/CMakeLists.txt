cmake_minimum_required(VERSION 3.16)
project(unit_test)

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/unit_test)
include_directories(${CMAKE_SOURCE_DIR}/ext/doctest)

set(VM_UNIT_TESTS
        vm/simple_vm.cpp
        vm/memory_permissions.cpp)

set(COMPILER_UNIT_TESTS
        compiler/tokenizer_tests.cpp
        compiler/lexer_tests.cpp)

set(WISP_UNIT_TESTS
        wisp/bytecode_tests.cpp
        wisp/int_value_type_tests.cpp)

if (MSVC)
  source_group("Source Files\\vm" FILES ${VM_UNIT_TESTS})
  source_group("Source Files\\compiler" FILES ${COMPILER_UNIT_TESTS})
  source_group("Source Files\\wisp" FILES ${WISP_UNIT_TESTS})
endif()
        
add_executable(unit_test main.cpp ${VM_UNIT_TESTS} ${COMPILER_UNIT_TESTS} ${WISP_UNIT_TESTS})
target_link_libraries(unit_test vm)
target_link_libraries(unit_test compiler)
target_link_libraries(unit_test doctest)
target_link_libraries(unit_test wisp)

set_target_properties(unit_test PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)