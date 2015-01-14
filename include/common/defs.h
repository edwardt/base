/**
 * @file mvdefs.h
 *
 * @brief Common MACROs and typedefs.
 */
#ifndef MV_DEFS_H
#define MV_DEFS_H


/* Determine platform. */
#if defined(__gnu_linux__)
#  define LINUX
#elif defined(__APPLE__) && defined(__MACH__)
#  define MACOSX
#elif defined(_WIN32)
#  define WIN32
#else
#  error "Unsupported platform"
#endif

/* Determine pointer size */
#if !defined (BITS32) && !defined (BITS64)
#ifndef __SIZEOF_POINTER__
#  error "__SIZEOF_POINTER__ undefined"
#endif
#if ( __SIZEOF_POINTER__ == 8 )
#  define BITS64
#elif ( __SIZEOF_POINTER__ == 4 )
#  define BITS32
#else
#  error "Failed to determine POINTER size"
#endif
#endif

/* DLLEXPORT */
#if defined(WIN32)
#  define STDCALL __stdcall
#  if defined(IMPORT_EXPORTS)
#    define DLLEXPORT __declspec(dllimport)
#  else
#    define DLLEXPORT __declspec(dllexport)
#  endif
#else
#  define STDCALL
#  define DLLEXPORT __attribute__ ((visibility("default")))
#endif

/* Branch prediction. */
#ifdef __GNUC__
#  define __unlikely(x) __builtin_expect(!!(x), 0)
#  define __likely(x)   __builtin_expect(!!(x), 1)
#else
#  define __unlikely(x) (x)
#  define __likely(x)   (x)
#endif

/* Define sized types. */
#ifdef BITS64
typedef char                mv_int8_t;
typedef unsigned char       mv_uint8_t;
typedef short               mv_int16_t;
typedef unsigned short      mv_uint16_t;
typedef int                 mv_int32_t;
typedef unsigned            mv_uint32_t;
typedef long                mv_int64_t;
typedef unsigned long       mv_uint64_t;
typedef float               mv_float_t;
typedef double              mv_double_t;
typedef mv_uint64_t         mv_ptr_t;
#else
typedef char                mv_int8_t;
typedef unsigned char       mv_uint8_t;
typedef short               mv_int16_t;
typedef unsigned short      mv_uint16_t;
typedef int                 mv_int32_t;
typedef unsigned            mv_uint32_t;
typedef long long           mv_int64_t;
typedef unsigned long long  mv_uint64_t;
typedef float               mv_float_t;
typedef double              mv_double_t;
typedef mv_uint32_t         mv_ptr_t;
#endif

#endif /* MV_DEFS_H */
