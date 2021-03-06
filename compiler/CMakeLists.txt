cmake_minimum_required(VERSION 3.10)
project(compiler)

include_directories(${CMAKE_SOURCE_DIR})

set(COMPILER_FILES
        ${CMAKE_SOURCE_DIR}/shared/platform.h
        ${CMAKE_SOURCE_DIR}/shared/types.h
        ast.h
        ast.cpp
        ast_types.h
        decl.h
        decl.cpp
        statement.h
        statement.cpp
        compiler.h
        compiler.cpp
        char.h
        char.cpp
        lexer.h
        lexer.cpp
        parser.h
        parser.cpp
        token.h
        token.cpp)

add_library(compiler STATIC ${COMPILER_FILES})

set_target_properties(compiler PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

if(MSVC)
  target_compile_options(compiler PRIVATE /W4 /WX)
else()
  target_compile_options(compiler PRIVATE -Wall -Wextra -pedantic -Werror)
endif()