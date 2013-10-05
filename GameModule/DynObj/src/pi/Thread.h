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

#ifndef PI_THREAD_H
#define PI_THREAD_H

// 'Rudimentary' platform independent thread interface

#define PI_FAILED     1     // Not an error, but operation could not be performed (as in piMutexTryLock)
#define PI_SUCCESS    0
#define PI_ARG_ERROR -1
#define PI_ERROR     -10


int piGetThreadId( );
int piYield( );
void piExitThread( int exit_code );    // Exit current thread of execution

// Create a mutex
typedef void* PI_MUTEX_HANDLE;

// Returns NULL on failure
// NOTE: Win32 only handles re-entrant sync objects, so it fails
// when recursive is false. For cross platform operation, set to true.
PI_MUTEX_HANDLE piMutexCreate( bool recursive=true );

// Below functions return PI_... as defined above
int piMutexDestroy( PI_MUTEX_HANDLE mh);
int piMutexLock( PI_MUTEX_HANDLE mh );
int piMutexTryLock( PI_MUTEX_HANDLE mh );
int piMutexUnlock( PI_MUTEX_HANDLE mh );


#endif // PI_THREAD_H

