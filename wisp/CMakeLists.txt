cmake_minimum_required(VERSION 3.10)
project(wisp)

include_directories(${CMAKE_SOURCE_DIR})

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(ISA_HEADERS
        isa/isa.h
        isa/encoding.h
        isa/compare.h
        isa/control_flow.h
        isa/integer_math.h
        isa/load_store.h
        isa/move.h
        isa/string.h
        isa/array.h
        isa/syscall.h
        )

set(ISA_SOURCES
        isa/isa.cpp
        isa/encoding.cpp
        isa/compare.cpp
        isa/control_flow.cpp
        isa/integer_math.cpp
        isa/load_store.cpp
        isa/move.cpp
        isa/string.cpp
        isa/array.cpp
        isa/syscall.cpp
        )

set(BASE_HEADERS
        bytecode.h
        mm.h
        program.h
        value.h
        integer_value.h
        fp_value.h
        complex_value.h
        register.h
        string_formatter.h
        context.h)

set(BASE_SOURCES
        program.cpp
        value.cpp
        integer_value.cpp
        fp_value.cpp
        complex_value.cpp
        string_formatter.cpp
        register.cpp)

if (MSVC)
  source_group("Header Files\\isa" FILES ${ISA_HEADERS})
  source_group("Source Files\\isa" FILES ${ISA_SOURCES})
endif()

add_library(wisp ${BASE_SOURCES} ${ISA_SOURCES} ${BASE_HEADERS} ${ISA_HEADERS})
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