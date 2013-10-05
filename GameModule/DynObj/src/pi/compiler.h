
#ifndef COMPILER_H
#define COMPILER_H

#include "platform.h"

// Detect compiler

#ifdef __GNUG__
#define PI_COMPILER "g++"
#define PI_COMPILER_ __GNUG__
#define PI_COMPILER_VERSION_MINOR __GNUC_MINOR__
#define GNU_INLINE_ASM

#elif __GNUC__
#define PI_COMPILER "gcc"
#define PI_COMPILER_ __GNUG__
#define PI_COMPILER_VERSION_MINOR __GNUC_MINOR__
#define GNU_INLINE_ASM

#elif _MSC_VER
#define PI_COMPILER "VisualC"
#define PI_COMPILER_ _MSC_VER
#define MSVC_INLINE_ASM

// Fill in for other compilers
//#elif 
#else
    #error Compiler not recognized
#endif // __GNUG__

#ifndef PI_COMPILER_VERSION
#define PI_COMPILER_VERSION PI_COMPILER_
#endif

#ifndef PI_COMPILER_VERSION_MINOR
#define PI_COMPILER_VERSION_MINOR 0
#endif


#if defined(GNU_INLINE_ASM)
    #define GET_FRAME_PTR(var) \
    __asm__ __volatile__ ( \
		"mov %%ebp, %0 \n" \
		: "=a"(var)        \
    );
    
#elif defined(MSVC_INLINE_ASM)
    #define GET_FRAME_PTR(var) \
    __asm{             \
		mov var, ebp   \
    }

#endif // GNU_INLINE_ASM

#ifdef GNU_INLINE_ASM
    #define GCC_COMPATIBLE
#endif

#ifdef MSVC_INLINE_ASM
    #define MSVC_COMPATIBLE
#endif


#endif // COMPILER_H

