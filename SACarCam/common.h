#pragma once
#define WIN32_LEAN_AND_MEAN

#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#pragma warning(disable: 4244)	// int to float
#pragma warning(disable: 4800)	// int to bool
#pragma warning(disable: 4305)	// double to float
#pragma warning(disable: 4838)  // narrowing conversion
#pragma warning(disable: 4996)  // POSIX names

#define PI 3.1415927f
#define TWOPI (PI*2)
#define HALFPI (PI/2)
#define DEGTORAD(x) ((x) * PI / 180.0f)
#define RADTODEG(x) ((x) * 180.0f / PI)

#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <math.h>
#include <new>
#include <string>

typedef uint8_t uint8;
typedef int8_t int8;
typedef uint16_t uint16;
typedef int16_t int16;
typedef uint32_t uint32;
typedef int32_t int32;
typedef uintptr_t uintptr;
typedef uint64_t uint64;
typedef int64_t int64;
// hardcode ucs-2
typedef uint16_t wchar;

typedef uintptr_t addr;
#define nil nullptr

inline float RecipSqrt(float x, float y) { return x / sqrtf(y); }
inline float RecipSqrt(float x) { return RecipSqrt(1.0f, x); }

#define clamp(v, low, high) ((v)<(low) ? (low) : (v)>(high) ? (high) : (v))
