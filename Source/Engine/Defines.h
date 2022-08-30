#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define FATAL_PLATFORM_WINDOWS 1
#ifndef _WIN64
#error "64-bit required."
#endif

#elif defined(__linux__) || defined(__gnu_linux__)
#define FATAL_PLATFORM_LINUX 1
#if defined(__ANDROID__)
#define FATAL_PLATFORM_ANDROID 1
#endif
#endif

#ifdef FATAL_EXPORT
#ifdef _MSC_VER
#define FATAL_API __declspec(dllexport)
#else
#define FATAL_API __attribute__((visibility("default")))
#endif
#else
#ifdef _MSC_VER
#define FATAL_API __declspec(dllimport)
#else
#define FATAL_API
#endif
#endif
