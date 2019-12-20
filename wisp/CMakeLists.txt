cmake_minimum_required(VERSION 3.10)
project(wisp)

include_directories(${CMAKE_SOURCE_DIR})

set(WISP_FILES
        ${CMAKE_SOURCE_DIR}/shared/platform.h
        ${CMAKE_SOURCE_DIR}/shared/types.h
        ${CMAKE_SOURCE_DIR}/shared/random.h
        ${CMAKE_SOURCE_DIR}/shared/decoder.h
        bytecode.h
        program.h
        program.cpp
        integer_value.h
        integer_value.cpp
        fp_value.h
        fp_value.cpp
        register.h
        register.cpp
        context.h
        isa/isa.h
        isa/isa.cpp)

add_library(wisp ${WISP_FILES})
target_link_libraries(wisp vm)
target_link_libraries(wisp compiler)

set_target_properties(wisp PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

if(MSVC)
  target_compile_options(wisp PRIVATE /W4 /WX)
else()
  target_compile_options(wisp PRIVATE -Wall -Wextra -pedantic -Werror)
endif()