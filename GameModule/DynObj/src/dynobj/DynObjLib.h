// Copyright (c) 2007, Arne Steinarson
// Licensing for DynObj project - see DynObj-license.txt in this folder

#ifndef DYNOBJLIB_H
#define DYNOBJLIB_H

#include "DynObj.h"
#include "pi/DynLib.h"
#include "utils/CharBuf.h"
#if DO_ENABLE_OBJECT_TRACKING==1
    #include "utils/ExpArr.hpp"
#endif

#include "DORT.h"


// Utility class: Load, init, access and close a DynObj library
/** @ingroup ImplClass
 * Represents a loaded DynObj library.
 * Use to automatically load a plugin class library.
 */
class DynObjLib {
public:

    /** Library status. Library can be used only when status is Initialized */
    enum {
        NotLoaded = 0,  // DOLIB_NONE
        LoadFailed,     // DOLIB_LOAD_FAILED
        Loaded,         // DOLIB_LOADED
        Incompatible,   // DOLIB_INCOMPATIBLE
        InitFailed,     // DOLIB_INIT_FAILED
        CannotInit,     // DOLIB_NO_INIT
        Initialized     // DOLIB_INIT
    };

    /** Constructor that takes library name/path as arg
      * @param lib name of library. Do not append .so or .dll to the name.
      * @param do_call_init Initialize the library (true by default) If false
      * some more checks can be made on the library before initialized.
      */
   DynObjLib( const char *lib, bool do_call_init=true, int load_flags=-1 );
   
   // A constructor for the main program, which is not really a library. But we want it 
   // to look that way to DoRunTime
   DynObjLib( const char *name, CreateDynObjFn pcfn, 
   			  const char *implements, const char *callstr );
   ~DynObjLib( );

   /** Call to close and unload the library. 
     * @param is_query If set, it is like a signal to the library that shutdown 
     * is approaching. If clear, it is shutdown
     * @return true on success
     */
   bool ExitDynLib( bool is_query );
   int GetState() { return m_state; }
   
   /** Turn on/off support for tracking object lifetime in this library. */
   bool EnableObjectTracking( bool enable );

   /** Check that library has loaded OK. 
     * @return true on success     */
   bool IsOk() { return m_state==Initialized; }
   
	/** Returns error from library loader */
	const char *GetError( );

   /** Read calling convention used by the library.
     * @return calling convention as a string. */
   const char *GetCallStr(){ return m_callstr; }
   
   /** Read platform of source library.
     * @return integer with platform specific value. */
   const int GetPlatform(){ return m_platform; }	// Return pointer to platform integer
   
   /** Read VTable properties from source compiler.
     * @return integer with VTable properties.
     */
   const int GetDoTraits(){ return m_do_traits; }	// Return pointer to traits integer
   
   /** Loookup arbitrary symbol from the library.
     * @return pointed to symbol.
     */
   void *FindSymbol( const char *symb_name );
   
   /** Get library name. */
   const char *GetLibName();
   
   /** Get library file-name. */
   const char *GetLibFileName();
   
   /** Get library base address. */
   void* GetBase( );
   
    
   /** Template wrapper for Create DynObj. 
     * @param pdi_init is the argument to the object constructor.
     * @return The new object or NULL if failed.
     */
    template<class T>
	DynObj* Create( const DynI* pdi_init=NULL ){
        return Create( doTypeInfo<T>::Name(), doTypeInfo<T>::Id(), pdi_init );
    }
    
   /** Create a DynObj from the library. 
     * @param type is the name of the string.
     * @param type_id is the integer type ID of the type.
     * @param pdi_init is the argument to the object constructor.
     * @param object_size is currently not used, use 0.
     * @return The new object or NULL if failed.
     */
	DynObj* Create( const char *type, int type_id, const DynI* pdi_init=NULL, int object_size=0 );
    
   /** Destroy a VObj or DynObj. 
     * If the object was created by this library it is destroyed. 
     * @param pvo is the object to destroy.
     * @return true if object belonged to this library and was destroyed.
     */
	bool    Destroy( VObj *pvo );
	
	/** Query if library implements type specified by string. 
      * Can be called before the library is initialized.
      * @param type type string
      * @return >0-Yes, 0-Unknown <0:No 
      */
	int        Implements( const char *type );
	/** Query if library implements type specified by type ID. 
      * Can be called before the library is initialized.
      * @param type type ID
      * @return >0-Yes, 0-Unknown <0:No 
      */
	int        Implements( int type_id );
	/** Read the function calling convention used by the library.
      * It is best to use one of __cdecl or __stdcall.
      * Can be called before the library is initialized.
      * @return string with calling convention (__cdecl, __stdcall, ...) 
      */
	const char *GetCallConv( );

	bool operator==(const char *name) { return m_name==name; }
protected:				
	// Initialize the library.
    void Ctor( const char *lib, bool do_call_init );
	bool InitDynLib( );
	bool InitFromLibStruct( DynObjLibImpl* impl );
	void OnObjectDestroy( DynObj *pdo, DynObjType *pdt );
    
	DynLib *m_pdlib;
	CharBuf	m_name;		// To override name of library (for pseudo lib: Main module)
	int	m_state;
    
	CreateDynObjFn  m_pcfn;		// Pointer to CreateDynObjFn for this library 
	//CreateDynObjFn  m_pbcr;		// Pointer to base class creation function for this library 
	InitDynLibFn	 m_pidlfn;	// Pointer to DoInitLib
	ExitDynLibFn	 m_pedlfn;	// Pointer to DoExitLib
	DestroyDynObjFn m_pdofn;
	
	DynObjLibImpl *m_pdol_impl;
	const char  *m_implements;	// Keep as a pointer to pointer, for main module
	const char  *m_callstr;		// Calling convention used by lib. Check against DO_CALLSTR
	int		    m_platform;		// A pointer to g_do_platform imported or NULL if not found
	int         m_do_traits;	// A pointer to g_do_traits which stores VTable compatibility info of
	const char *m_last_err;		// From library loader

#if DO_ENABLE_OBJECT_TRACKING==1
    int   m_ot_cnt;
    int   m_ot_size;
    int   m_ot_size_total;
    bool  m_ot_enable;
    ExpArrP<DynObj*> m_objs;
#endif
	
    friend class DoRunTimeC;
};


#define DOH_LIB_FIRST ((const char*)0)

// Templates for DynObj creation & temporary holding 

#ifdef DO_USE_RUNTIME 
#ifdef DO_MAIN

// Included from main program. Just need protos. These are implemented in DoRunTime.h
template <class T, class A>
T *do_new( A arg, const char *lib_name=DOH_LIB_FIRST );
template <class T>
T *do_new( );

#else // DO_MODULE

#include "DoBase.hpp"
#include "DORT.h"

// Make local version of do_new, still using DoRunTime
template<class T,class A, bool dyni_arg>
struct DoCreator;

template<class T,class A>
struct DoCreator<T,A,false> {
    static T *Create( A arg, const char *lib ){
        // Wrap arg
        DynData dd(arg);
        return (T*)DORT.Create( lib, doTypeInfo<T>::Name(), doTypeInfo<T>::Id(), &dd, 0 );
    }
};

template<class T,class A>
struct DoCreator<T,A*,true> {
    static T *Create( A* parg, const char *lib ){
        // Arg is already DynI
        return (T*)DORT.Create( lib, doTypeInfo<T>::Name(), doTypeInfo<T>::Id(), parg, 0 );
    }
};

template<class T,class A>
struct DoCreator<T,A&,true> {
    static T *Create( A& arg, const char *lib ){
        // Arg is already DynI
        return (T*)DORT.Create( lib, doTypeInfo<T>::Name(), doTypeInfo<T>::Id(), &arg, 0 );
    }
};

// Take any argument, select way to create object
template <class T, class A>
T *do_new( A arg, const char *lib_name=DOH_LIB_FIRST ){
    return DoCreator<T,A,DoIsTypeA<A,DynI>::v>::Create( arg, lib_name );
}

template <class T>
T *do_new( ){
    return DoCreator<T,DynI*,true>::Create( (DynI*)NULL, DOH_LIB_FIRST );
}
#endif // DO_MAIN + DO_MODULE

#endif // DO_USE_RUNTIME



#endif // DYNOBJLIB_H


