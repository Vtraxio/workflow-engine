#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long long i64;

typedef float f32;
typedef double f64;

static_assert(sizeof(u8) == 1, "Expected u8 to be 1 byte.");
static_assert(sizeof(u16) == 2, "Expected u16 to be 2 bytes.");
static_assert(sizeof(u32) == 4, "Expected u32 to be 4 bytes.");
static_assert(sizeof(u64) == 8, "Expected u64 to be 8 bytes.");
static_assert(sizeof(i8) == 1, "Expected i8 to be 1 byte.");
static_assert(sizeof(i16) == 2, "Expected i16 to be 2 bytes.");
static_assert(sizeof(i32) == 4, "Expected i32 to be 4 bytes.");
static_assert(sizeof(i64) == 8, "Expected i64 to be 8 bytes.");
static_assert(sizeof(f32) == 4, "Expected f32 to be 4 bytes.");
static_assert(sizeof(f64) == 8, "Expected f64 to be 8 bytes.");

#if defined(NDEBUG)
#define BUILD_RELEASE 1
#else
#define BUILD_DEBUG 1
#endif

#define ARRAY_COUNT(x) (sizeof(x) / sizeof((x)[0]))

#define KILOBYTES(x) ((x) * 1024ULL)
#define MEGABYTES(x) (KILOBYTES(x) * 1024ULL)
#define GIGABYTES(x) (MEGABYTES(x) * 1024ULL)

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define CLAMP(x, lo, hi) (MIN(MAX((x), (lo)), (hi)))

#define SWAP(type, a, b)                                                       \
  do {                                                                         \
    type tmp = a;                                                              \
    (a) = b;                                                                   \
    (b) = tmp;                                                                 \
  } while (0)

#define UNUSED(x) (void)(x)

#define FORCE_INLINE inline __attribute__((always_inline))
#define NO_INLINE __attribute__((noinline))
#define THREAD_LOCAL __thread
#define DEBUG_BREAK() __builtin_trap()
#define UNREACHABLE() __builtin_unreachable()

#include <stdio.h>

#define LOG_INFO(...)                                                          \
  do {                                                                         \
    printf("[INFO] ");                                                         \
    printf(__VA_ARGS__);                                                       \
    printf("\n");                                                              \
  } while (0)
#define LOG_WARN(...)                                                          \
  do {                                                                         \
    printf("[WARN] ");                                                         \
    printf(__VA_ARGS__);                                                       \
    printf("\n");                                                              \
  } while (0)
#define LOG_ERROR(...)                                                         \
  do {                                                                         \
    printf("[ERROR] ");                                                        \
    printf(__VA_ARGS__);                                                       \
    printf("\n");                                                              \
  } while (0)

#if BUILD_DEBUG
#define ASSERT(x)                                                              \
  do {                                                                         \
    if (!(x)) {                                                                \
      LOG_ERROR("Assertion failed: %s (%s:%d)", #x, __FILE__, __LINE__);       \
      DEBUG_BREAK();                                                           \
    }                                                                          \
  } while (0)
#else
#define ASSERT(x) ((void)0)
#endif