#pragma once

#define WISP_ARCH_X64 0
#define WISP_ARCH_X86 1
#define WISP_ARCH_ARM 2
#define WISP_ARCH_ARM64 3

#define WISP_OS_WINDOWS 0
#define WISP_OS_LINUX 1

#define WISP_COMPILER_VS 0
#define WISP_COMPILER_GCC 1
#define WISP_COMPILER_CLANG 2

#if defined(_M_X64) || defined(__x86_64__)
#define WISP_ARCH FRAMEWORK_ARCH_X64
#define WISP_WORD_SIZE 64
#elif defined(_M_IX86) || defined(__i386__)
#define WISP_ARCH FRAMEWORK_ARCH_X86
#define WISP_WORD_SIZE 32
#elif defined(_M_ARM) || defined(__arm__)
#define WISP_ARCH FRAMEWORK_ARCH_ARM
#define WISP_WORD_SIZE 32
#elif defined(_M_ARM64) || defined(__aarch64__)
#define WISP_ARCH FRAMEWORK_ARCH_ARM64
#define WISP_WORD_SIZE 64
#else
#error Invalid architecture
#endif

#if defined(__clang__)
#define WISP_COMPILER WISP_COMPILER_CLANG
#elif defined(__GNUC__)
#define WISP_COMPILER WISP_COMPILER_GCC
#elif defined(_MSC_VER)
#define WISP_COMPILER WISP_COMPILER_VS
#endif

#if defined(_WIN32) || defined(_WIN64)
#define WISP_OS WISP_OS_WINDOWS
#elif defined(__linux__)
#define WISP_OS WISP_OS_LINUX
#else
#error Invalid operating system
#endif