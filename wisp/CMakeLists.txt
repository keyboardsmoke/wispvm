cmake_minimum_required(VERSION 3.10)
project(wisp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

include_directories(${CMAKE_SOURCE_DIR})

set(WISP_FILES
        ${CMAKE_SOURCE_DIR}/shared/platform.h
        ${CMAKE_SOURCE_DIR}/shared/types.h
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