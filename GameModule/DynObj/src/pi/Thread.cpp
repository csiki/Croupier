//
// Copyright (C) 2007 Arne Steinarson <arst at users dot sourceforge dot net>
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any
// damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute
// it freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must
//    not claim that you wrote the original software. If you use this
//    software in a product, an acknowledgment in the product
//    documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must
//    not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source
//    distribution.
//

// No diagnostic function frames in here
#undef _CFE_
#define _CFE_ 

#include "pi/platform.h"
#include "pi/Thread.h"

// Find out what implementation to use
#if !defined(WXWIDGETS) && !defined(PTHREADS) && !defined(_WIN32)
	#warning Neither WXWIDGETS, PTHREADS or _WIN32 is defined
    #if defined(_WX_DEFS_H_)
        #define WXWIDGETS
        #warning Thread: WxWidgets detected
    #elif defined(PTHREAD_SCOPE_SYSTEM) || defined(__UNIX__)
        #define PTHREADS
        #warning Thread: PThread detected
    #elif defined(__WINDOWS__) 
        #define _WIN32
        #warning Thread: Windows detected
    #else
        #error No implementation specified or detected
    #endif
#endif
    
/*
#undef WXWIDGETS
#undef PTHREADS
//#define CRITSECT
#define WXWIDGETS
*/

#if defined(WXWIDGETS)

// wxWidgets implementation, straight forward
#include <wx/thread.h>

int piGetThreadId( ){_CFE_;
    return wxThread::GetCurrentId();
}

int piYield( ){_CFE_;
    wxThread::Yield();
	wxThread::Sleep( 10 );
	wxMilliSleep( 10 );
    
    return 0;
}


struct wxAccessThread : public wxThread {
	void DoExit( int exit_code ){
		Exit( reinterpret_cast<wxThread::ExitCode>(exit_code) );
	}
};

void piExitThread( int exit_code ){_CFE_;
	wxAccessThread *pth = ((wxAccessThread*)wxThread::This());
	//wxASSERT( pth );	// Asserts are no good here, not robust enough
	if( !pth ){
		printf( "piExitThread - Failed retrieving current thread\n" );
		printf( "Exiting program...\n" );
		exit(-1);
	}
    pth->DoExit( exit_code );
    
	printf( "piExitThreasd - Failed wxThread::Exit\n" );
	printf( "Exiting program...\n" );
	exit(-1);
}


/*
#include "CritSect.h"
PI_MUTEX_HANDLE piMutexCreate( bool recursive ){_CFE_;
    wxCritSect *pm = new wxCritSect(true);
    return pm;
}

int piMutexDestroy( PI_MUTEX_HANDLE mh){_CFE_;
    if( !mh ) return PI_ARG_ERROR;
    wxCritSect *pm = (wxCritSect*)mh;
    delete pm;
    return 0;
}

int piMutexLock( PI_MUTEX_HANDLE mh ){_CFE_;
    if( !mh ) return PI_ARG_ERROR;
    wxCritSect *pm = (wxCritSect*)mh;
    pm->Enter();
    return 0;
}

int piMutexTryLock( PI_MUTEX_HANDLE mh ){_CFE_;
    if( !mh ) return PI_ARG_ERROR;
    wxCritSect *pm = (wxCritSect*)mh;
    return pm->TryEnter() ? 0 : PI_FAILED;
}

int piMutexUnlock( PI_MUTEX_HANDLE mh ){_CFE_;
    if( !mh ) return PI_ARG_ERROR;
    wxCritSect *pm = (wxCritSect*)mh;
    pm->Leave();
    return 0;
}
*/

PI_MUTEX_HANDLE piMutexCreate( bool recursive ){_CFE_;
    wxMutex *pm = new wxMutex( recursive ? wxMUTEX_RECURSIVE : wxMUTEX_DEFAULT );
    return pm;
}

int piMutexDestroy( PI_MUTEX_HANDLE mh){_CFE_;
    if( !mh ) return PI_ARG_ERROR;
    wxMutex *pm = (wxMutex*)mh;
    delete pm;
    return 0;
}

int piMutexLock( PI_MUTEX_HANDLE mh ){_CFE_;
    if( !mh ) return PI_ARG_ERROR;
    wxMutex *pm = (wxMutex*)mh;
    if( pm->Lock()==wxMUTEX_NO_ERROR ) return 0;
    else return PI_FAILED;
}

int piMutexTryLock( PI_MUTEX_HANDLE mh ){_CFE_;
    if( !mh ) return PI_ARG_ERROR;
    wxMutex *pm = (wxMutex*)mh;
    int r = pm->TryLock();
    if( r==wxMUTEX_BUSY ) return PI_FAILED;
    else if( r==wxMUTEX_NO_ERROR ) return 0;
    else return PI_ERROR;
}

int piMutexUnlock( PI_MUTEX_HANDLE mh ){_CFE_;
    if( !mh ) return PI_ARG_ERROR;
    wxMutex *pm = (wxMutex*)mh;
    if( pm->Unlock()==wxMUTEX_NO_ERROR ) return 0;
    else return PI_FAILED;
}


#elif defined(PTHREADS)

// Implementation for PThreads
#include <pthread.h>	// pthreads

int piGetThreadId( ){_CFE_;
    return pthread_self();
}

int piYield( ){_CFE_;
    return sched_yield();
}

void piExitThread( int exit_code ){_CFE_;
    // # I'm not sure what the void* is all about here
    pthread_exit( reinterpret_cast<void*>(exit_code) );
}



PI_MUTEX_HANDLE piMutexCreate( bool recursive ){_CFE_;
    pthread_mutexattr_t attrs;
    if( pthread_mutexattr_init(&attrs) ) return 0;
    if( pthread_mutexattr_settype(&attrs, recursive?PTHREAD_MUTEX_RECURSIVE:PTHREAD_MUTEX_ERRORCHECK) ) 
        return 0;

    pthread_mutex_t *pm = new pthread_mutex_t;
    if( !pm ) return 0;
    
    int r = pthread_mutex_init(pm,&attrs);
    pthread_mutexattr_destroy(&attrs);
    if( r ){
        delete pm;
        return 0;
    }
    return pm;
}

int piMutexDestroy( PI_MUTEX_HANDLE mh){_CFE_;
    if( !mh ) return PI_ARG_ERROR;
    return pthread_mutex_destroy((pthread_mutex_t*)mh) ? PI_ERROR : 0;
}

int piMutexLock( PI_MUTEX_HANDLE mh ){_CFE_;
    if( !mh ) return PI_ARG_ERROR;
    return pthread_mutex_lock((pthread_mutex_t*)mh) ? PI_ERROR : 0;
}

int piMutexTryLock( PI_MUTEX_HANDLE mh ){_CFE_;
    if( !mh ) return PI_ARG_ERROR;
    return pthread_mutex_trylock((pthread_mutex_t*)mh) ? PI_FAILED : 0;
}

int piMutexUnlock( PI_MUTEX_HANDLE mh ){_CFE_;
    if( !mh ) return PI_ARG_ERROR;
    return pthread_mutex_unlock((pthread_mutex_t*)mh) ? PI_ERROR : 0;
}


#elif defined(_WIN32)

#ifndef _WIN32_WINNT
#   define _WIN32_WINNT 0x400
#endif
#include <windows.h>

int piGetThreadId( ){_CFE_;
    return GetCurrentThreadId();
}

int piYield( ){_CFE_;
    return SwitchToThread() ? 0 : 1;
}

void piExitThread( int exit_code ){_CFE_;
    // # I'm not sure what the void* is all about here
    ExitThread( exit_code );
}


PI_MUTEX_HANDLE piMutexCreate( bool recursive ) {_CFE_;
    if( !recursive ) return NULL;
    LPCRITICAL_SECTION pcs = (LPCRITICAL_SECTION)new char[sizeof(CRITICAL_SECTION)];
    InitializeCriticalSection(pcs);
    return pcs;
}

int piMutexDestroy( PI_MUTEX_HANDLE mh){_CFE_;
    if( !mh ) return PI_ARG_ERROR;
    DeleteCriticalSection( (LPCRITICAL_SECTION)mh );
    delete [] (char*)mh;
    return 0;
}

int piMutexLock( PI_MUTEX_HANDLE mh ){_CFE_;
    if( !mh ) return PI_ARG_ERROR;
    EnterCriticalSection( (LPCRITICAL_SECTION)mh );
    return 0;
}

int piMutexTryLock( PI_MUTEX_HANDLE mh ){_CFE_;
    if( !mh ) return PI_ARG_ERROR;
    return TryEnterCriticalSection( (LPCRITICAL_SECTION)mh ) ? 0 : PI_FAILED;
}

int piMutexUnlock( PI_MUTEX_HANDLE mh ){_CFE_;
    if( !mh ) return PI_ARG_ERROR;
    LeaveCriticalSection( (LPCRITICAL_SECTION)mh );
    return 0;
}

#else
    #error No implementation specificed!
#endif

