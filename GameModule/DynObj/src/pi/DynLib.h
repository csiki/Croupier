
#ifndef PILIB_H
#define PILIB_H

#include "platform.h"
#if defined(__WINDOWS__)
	#define PI_MODE	0
#elif defined(__UNIX__)
	//#define _GNU_SOURCE
	#include <dlfcn.h>
	//#define PI_MODE     (RTLD_LAZY|RTLD_LOCAL)	// Resolve symbols now, keep them local
	//#define PI_MODE   (RTLD_LAZY|RTLD_GLOBAL)	// Resolve symbols when needed, make global
	//#define PI_MODE   (RTLD_NOW|RTLD_GLOBAL)	// Resolve symbols now, make global
    //#define OI_MODE   (RTLD_NOW|RTLD_GLOBAL|RTLD_DEEPBIND)
    #define PI_MODE   	(RTLD_NOW|RTLD_LOCAL|RTLD_DEEPBIND)
#else
	#error Unknown platform
#endif

// C interface
void *piLoadLib(const char *pcLibName, int iMode=-1);
void *piGetSymbol(void* hLib, const char *name);
bool piUnload(void *hLib);


// C++ wrapper
#ifdef __cplusplus

class DynLib{
public:
	DynLib( const char *pcLibName, int iMode=-1 );
	~DynLib( ){ if( m_hLib ) piUnload(m_hLib); delete [] m_name; }
	
	void *GetSymbol( const char *name ){ return piGetSymbol(m_hLib,name); }
	
	// Return handle to library
	operator void* ();
	const char *GetName(){ return m_name; }
	const char *GetLastErr();
	
	void* GetBase( const char *known_symbol );
	
protected:
	void* m_hLib;	
	const char *m_err;
	char *m_name;
	void* m_base;	// Base address of library in mem
};

#endif // __cplusplus

#endif // PILIB_H


