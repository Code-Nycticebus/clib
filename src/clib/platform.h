#ifndef __CLIB_PLATFORM_H__
#define __CLIB_PLATFORM_H__

/* System */
#if defined(__linux__)
#define LINUX 1
#define CLIB_SYSTEM LINUX
#elif defined(_WIN32) || defined(_WIN64)
#define WINDOWS 2
#define CLIB_SYSTEM WINDOWS
#else
#error "Platform currently not supported"
#endif
/* !System */

/* Architecture */
#if defined(__x86_64__) || defined(_M_X64)
#define x86_64 1
#define CLIB_ARCHITECTURE x86_64
#elif defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)
#define x86_32 2
#define CLIB_ARCHITECTURE x86_32
#elif defined(__ARM_ARCH_2__)
#define ARM2 3
#define CLIB_ARCHITECTURE ARM2
#elif defined(__ARM_ARCH_3__) || defined(__ARM_ARCH_3M__)
#define ARM3 4
#define CLIB_ARCHITECTURE ARM3
#elif defined(__ARM_ARCH_4T__) || defined(__TARGET_ARM_4T)
#define ARM4T 5
#define CLIB_ARCHITECTURE ARM4T
#elif defined(__ARM_ARCH_5_) || defined(__ARM_ARCH_5E_)
#define ARM5 6
#define CLIB_ARCHITECTURE ARM5
#elif defined(__ARM_ARCH_6T2_) || defined(__ARM_ARCH_6T2_)
#define ARM6T2 7
#define CLIB_ARCHITECTURE ARM6T2
#elif defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) ||                   \
    defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__) ||                    \
    defined(__ARM_ARCH_6ZK__)
#define ARM6 8
#define CLIB_ARCHITECTURE ARM6
#elif defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) ||                   \
    defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) ||                    \
    defined(__ARM_ARCH_7S__)
#define ARM7 9
#define CLIB_ARCHITECTURE ARM7
#elif defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) ||                  \
    defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
#define ARM7A 10
#define CLIB_ARCHITECTURE ARM7A
#elif defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) ||                  \
    defined(__ARM_ARCH_7S__)
#define ARM7R 11
#define CLIB_ARCHITECTURE ARM7R
#elif defined(__ARM_ARCH_7M__)
#define ARM7M 12
#define CLIB_ARCHITECTURE ARM7M
#elif defined(__ARM_ARCH_7S__)
#define ARM7S 13
#define CLIB_ARCHITECTURE ARM7S
#elif defined(__aarch64__) || defined(_M_ARM64)
#define ARM64 14
#define CLIB_ARCHITECTURE ARM64
#elif defined(mips) || defined(__mips__) || defined(__mips)
#define MIPS 15
#define CLIB_ARCHITECTURE MIPS
#elif defined(__sh__)
#define SUPERH 16
#define CLIB_ARCHITECTURE SUPERH
#elif defined(__powerpc) || defined(__powerpc__) || defined(__powerpc64__) ||  \
    defined(__POWERPC__) || defined(__ppc__) || defined(__PPC__) ||            \
    defined(_ARCH_PPC)
#define POWERPC 17
#define CLIB_ARCHITECTURE POWERPC
#elif defined(__PPC64__) || defined(__ppc64__) || defined(_ARCH_PPC64)
#define POWERPC64 18
#define CLIB_ARCHITECTURE POWERPC64
#elif defined(__sparc__) || defined(__sparc)
#define SPARC 19
#define CLIB_ARCHITECTURE SPARC
#elif defined(__m68k__)
#define M68K 20
#define CLIB_ARCHITECTURE M68K
#else
#define ARCHITECTURE_UNKNOWN 0
#define CLIB_ARCHITECTURE ARCHITECTURE_UNKNOWN
#endif
/* !Architecture */

/* Compiler */
#if defined(__GNUC__) && !defined(__clang__)
#define GCC 1
#define CLIB_COMPILER GCC
#elif defined(__clang__)
#define CLANG 2
#define CLIB_COMPILER CLANG
#elif defined(_MSC_VER)
#define MSVC 3
#define CLIB_COMPILER MSVC
#elif defined(__MINGW32__)
#define MINGW32 4
#define CLIB_COMPILER MINGW32
#elif defined(__MINGW64__)
#define MINGW64 5
#define CLIB_COMPILER MINGW64
#elif defined(__INTEL_COMPILER)
#define INTEL_COMPILER 6
#define CLIB_COMPILER INTEL_COMPILER
#else
#define COMPILER_UNKOWN 0
#define CLIB_COMPILER COMPILER_UNKOWN
#endif
/* !Compiler */

/* CPU */
#if defined(__LP64__) || defined(_LP64)
#define CLIB_64BIT
#else
#define CLIB_32BIT
#endif
/* !CPU */

/* Byte-Order */
#define ENDIAN_UNKOWN 0x00
#define ENDIAN_LITTLE 0x01
#define ENDIAN_BIG 0x80

#if defined(__BYTE_ORDER__) && defined(__ORDER_BIG_ENDIAN__) &&                \
    defined(__ORDER_LITTLE_ENDIAN__)
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define CLIB_BYTE_ORDER ENDIAN_BIG
#elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define CLIB_BYTE_ORDER ENDIAN_LITTLE
#endif
#else
#define CLIB_BYTE_ORDER ENDIAN_UNKOWN
#endif
/* !Byte-Order */

#ifdef __cross__
#define CLIB_CROSS_COMPILATION
#endif

#endif /*  !__CLIB_PLATFORM_H__ */
