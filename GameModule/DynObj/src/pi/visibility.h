
#ifndef VISIBILITY_H
#define VISIBILITY_H

// This gives a way to make global symbols visible or hidden
// On Win32, when building DLLs symbols are by default hidden 
// and must be made visible with __declspace(dllexport). The
// visibility attribute in GCC provides the same functionality.
//
// DynObjs themselves don't use very many global symbols, since
// they rely on vtable position for linking. It is really only 
// the 'CreateDynObjFn' that must be exported from a DynObj library. 
#if defined(WIN32) || defined(_WIN32) 
  #define SYM_LOCAL 
  #define SYM_EXPORT __declspec(dllexport)
  #define SYM_IMPORT __declspec(dllimport)
#else
  #if(__GNUC__ >= 4) || (__GNUC__ == 3 && __GNUC_MINOR__ >= 4)
    #define SYM_LOCAL __attribute__ ((visibility("hidden")))
    #define SYM_EXPORT __attribute__ ((visibility("default")))
    #define SYM_IMPORT __attribute__ ((visibility("default")))
  #else
    #define SYM_IMPORT
    #define SYM_EXPORT
    #define SYM_IMPORT
  #endif
#endif
	
#endif // VISIBILITY_H
