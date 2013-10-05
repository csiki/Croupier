//Boby Thomas - march 2006
// Adopted, March 2007, Arne Steinarson

#include <string.h>

#include "pi/DynLib.h"

//#define _GNU_SOURCE
//#include <dlfcn.h>
/*
void pi_test(){_CFE_;
	Dl_info dli;
	dladdr( (const void*)pi_test, &dli );
}
*/


#if defined(__WINDOWS__)
	#include <windows.h>
	#define PI_VALID(hndl) (hndl && hndl!=INVALID_HANDLE_VALUE)  
#elif defined(__UNIX__)
	#define PI_VALID(hndl) (hndl!=NULL)  
	#include <dlfcn.h>
#endif

#ifdef _MSC_VER 
	// MSVC complains about stricmp
	#define stricmp _stricmp
#endif



void* piLoadLib(const char *pcLibName, int iMode){_CFE_;
    if( iMode==-1 ) iMode = PI_MODE;
    
	if( !pcLibName ) return NULL;
	char lib_name[1024];
	int sl = (int)strlen(pcLibName);
	void *pv;
	if( sl>1000 ) return NULL;
	strcpy(lib_name,pcLibName);
	
#ifdef __WINDOWS__ 
	//char cwd[512];
	//GetCurrentDirectoryA(511,cwd);
	// If we have some forward slashes, convert to backslashes
	for( int p=0; p<sl; p++ )
		if( lib_name[p]=='/' )
			lib_name[p] = '\\';
	if( stricmp(lib_name+sl-4,".dll") )
		strcat(lib_name,".dll");
	SetLastError(0);
	pv = (void*)LoadLibraryExA(lib_name,NULL,iMode);
	if( pv ){
		// Windows sometimes leaves an error even though operation succeeded. 
		// (when loading from a parent directory). 
		// Since we have a handle, we can assume it's valid.
		SetLastError( 0 );
	}
    
#elif defined(__UNIX__) 
	// If we have some back-slashes, convert to forward slashes
	for( int p=0; p<sl; p++ )
		if( lib_name[p]=='\\' )
			lib_name[p] = '/';
	if( strcmp(lib_name+sl-3,".so") )
		strcat(lib_name,".so");
	pv = dlopen(lib_name,iMode);
    if( !pv && *pcLibName!='/' ){
		// # Hack, under many Unices, the current dir is not searched 
        strcpy(lib_name,"./");
        strcat(lib_name,pcLibName);
        if( strcmp(lib_name+2+sl-3,".so") )
            strcat(lib_name,".so");
        pv = dlopen(lib_name,iMode);
    }
#endif

	return PI_VALID(pv) ? pv : NULL;
}

void *piGetSymbol(void* hLib, const char *name)
{_CFE_;
#ifdef __WINDOWS__ 
	return (void*)GetProcAddress((HMODULE)hLib,name);
#elif defined(__UNIX__) // GNU compiler
	return dlsym(hLib,name);
#endif
}

bool piUnload(void *hLib)
{_CFE_;
#ifdef __WINDOWS__ 
return FreeLibrary((HMODULE)hLib) ? true : false;
#elif defined(__GNUC__) // GNU compiler
	return dlclose(hLib);
#endif
}

const char *DynLib::GetLastErr(){_CFE_;
   static char sbuf[256];
	*sbuf = 0;
#ifdef __WINDOWS__
   DWORD err = GetLastError();
   if( err ){
	   LPVOID lpMsgBuf;
   	// Tricky to get the plain text message... but here we go
	   FormatMessageA(
			 FORMAT_MESSAGE_ALLOCATE_BUFFER |
			 FORMAT_MESSAGE_FROM_SYSTEM |
			 FORMAT_MESSAGE_IGNORE_INSERTS,
			 NULL,
			 err,
			 MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			 (LPSTR) &lpMsgBuf,
			 0,
			 NULL );
   		strncpy( sbuf, (const char*)lpMsgBuf, 255 );
    	LocalFree( lpMsgBuf );
	}
#elif defined(__UNIX__)
   const char *pc = dlerror();
   if( pc )
		strncpy( sbuf, (const char*)pc, 255 );
#endif	
    return sbuf;
}



#ifdef __cplusplus

DynLib::DynLib( const char *pcLibName, int iMode ) : m_err(NULL), m_name(NULL), m_base(NULL) {_CFE_;
	m_hLib = piLoadLib( pcLibName, iMode );
#ifdef __WINDOWS__
	m_base = m_hLib;	// For Windows, handle is base address
#endif	
	if( !PI_VALID(m_hLib) ) return; // User will have to check last error
	const char *pc = strrchr(pcLibName,'/');
	if( !pc ) pc = strrchr(pcLibName,'\\');
	if( !pc ) pc = pcLibName;
	else pc++;
	m_name = new char[strlen(pc)+1];
	strcpy(m_name,pc);
}

DynLib::operator void* () { 
	return PI_VALID(m_hLib) ? m_hLib : NULL; 
}

#include "errno.h"
void* DynLib::GetBase( const char *known_symbol ){_CFE_;
	if( m_base ) return m_base;
#ifdef __UNIX__
	// Two step process, first get an address inside library, 
	// then find base address of lib
	void *pv = dlsym( m_hLib, known_symbol );
	if( pv ){
		Dl_info dli;
		if( dladdr( pv, &dli ) )
			m_base = dli.dli_fbase;
		else
			int err = errno;
	}
#endif	
	return m_base;
}

#endif // __cplusplus

