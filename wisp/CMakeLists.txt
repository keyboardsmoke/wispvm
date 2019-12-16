cmake_minimum_required(VERSION 3.10)
project(wisp)

include_directories(${CMAKE_SOURCE_DIR})

set(WISP_FILES
        ${CMAKE_SOURCE_DIR}/shared/platform.h
        ${CMAKE_SOURCE_DIR}/shared/types.h
        ${CMAKE_SOURCE_DIR}/shared/random.h
        ${CMAKE_SOURCE_DIR}/shared/decoder.h
        program.h
        program.cpp
        register_math.h
        register.h
        register.cpp
        context.h
        isa/compare.h
        isa/compare.cpp
        isa/control_flow.h
        isa/control_flow.cpp
        isa/fp.h
        isa/fp.cpp
        isa/int.h
        isa/int.cpp
        isa/memory.h
        isa/memory.cpp
        isa/string.h
        isa/string.cpp
        isa/isa.h
        isa/isa.cpp
        main.cpp)

add_executable(wisp ${WISP_FILES})
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