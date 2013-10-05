
#include "compiler.h"

extern "C" {

#ifndef ATOMIC_OS_VERSION

// Inline assembly version
int AtomicAdd( int *pi, int v ){_CFE_;
	int rv;
#ifdef GNU_INLINE_ASM
    __asm__ __volatile__ (
		"lock; xaddl %%eax, (%%ecx) \n"
		: "=a"(rv)
		: "c"(pi), "a"(v)
    );

#elif defined(MSVC_INLINE_ASM)
	// Assumption: Intel C compiler has same inline syntax as MSVC (?)
	__asm {
		mov ecx, pi
		mov eax, v
		lock xadd [ecx], eax
		mov rv, eax
	}
#else
	#error No implementation of AtomicAdd
#endif
    // After xaddl (exchange...) we have the previous value (on the stack). 
    // So add the increment and return.
	return rv + v;	
}

int AtomicIncrement( int *pi ){_CFE_;
	int rv;
#ifdef GNU_INLINE_ASM
    __asm__ __volatile__ (
		"lock; xaddl %0, (%%ecx) \n"
		: "=a"(rv)
		: "c"(pi), "0" (1)
    );

#elif defined(MSVC_INLINE_ASM)
	// Assumption: Intel C compiler has same inline syntax as MSVC (?)
	__asm {
		mov ecx, pi
        mov eax, 1
		lock xadd [ecx], eax
		mov rv, eax
	}
#else
	#error No implementation of AtomicAdd
#endif
    // After xaddl (exchange...) we have the previous value (on the stack). 
    // So add the increment and return.
	return rv+1;	
}


int AtomicDecrement( int *pi ){_CFE_;
	int rv;
#ifdef GNU_INLINE_ASM
    __asm__ __volatile__ (
		"lock; xaddl %0, (%%ecx) \n"
		: "=a"(rv)
		: "c"(pi), "0" (-1)
    );

#elif defined(MSVC_INLINE_ASM)
	// Assumption: Intel C compiler has same inline syntax as MSVC (?)
	__asm {
		mov ecx, pi
        mov eax, -1
		lock xadd [ecx], eax
		mov rv, eax
	}
#else
	#error No implementation of AtomicAdd
#endif
    // After xaddl (exchange...) we have the previous value (on the stack). 
    // So add the increment and return.
	return rv-1;	
}


int AtomicExchange( int *pi, int val ){_CFE_;
	int rv;
#ifdef GNU_INLINE_ASM
    __asm__ __volatile__ (
		"lock; xchgl %0, (%%ecx) \n"
		: "=a"(rv)
		: "c"(pi), "0" (val)
    );

#elif defined(MSVC_INLINE_ASM)
	// Assumption: Intel C compiler has same inline syntax as MSVC (?)
	__asm {
		mov ecx, pi
        mov eax, val
		lock xchg [ecx], eax
		mov rv, eax
	}
#else
	#error No implementation of AtomicAdd
#endif
	return rv;	
}


#else // ATOMIC_OS_VERSION

// Use OS function instead
#ifdef _WIN32 
#include <windows.h>
#warning AtomicAdd is slow when adding values > 1

int AtomicAdd( int *pi, int v ){_CFE_;
    // This is inefficient, but it does the job
    // The primitive to add directly is not available 
    // in most Windows versions
    int rv = *pi;
    while( v<0 ){
        rv = InterlockedDecrement( (long*)pi );
        v++;
    }
    while( v>0 ){
        rv = InterlockedIncrement( (long*)pi );
        v--;
    }
    return rv;
}

int AtomicIncrement( int *pi ){_CFE_;
    return InterlockedInccrement( (long*)pi );
}

int AtomicDecrement( int *pi ){_CFE_;
    return InterlockedDecrement( (long*)pi );
}

int AtomicExchange( int *pi, int val ){_CFE_;
    return InterlockedExchange( pi, val );
}
#endif // _WIN32 

#endif // ATOMIC_OS_VERSION

} // extern "C" 

