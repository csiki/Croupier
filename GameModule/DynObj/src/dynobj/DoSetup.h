// Copyright (c) 2007, Arne Steinarson
// Licensing for DynObj project - see DynObj-license.txt in this folder


#ifndef DOSETUP_H
#define DOSETUP_H

// Detect platform
#include "pi/platform.h"

// Definition of symbol visibility 
#include "pi/visibility.h"

// Version of DynObj.cpp and friends
#define DYNOBJ_MAJOR_VERSION 0
#define DYNOBJ_MINOR_VERSION 51

// Doxygen stuff 
/** @addtogroup DynDefs
    @{ */
#if DOXYGEN_BUILD
    // To force documentation of these symbols
    #undef DO_USE_RUNTIME
    #undef DO_USE_DYNSHARED
    #undef DO_FAT_VOBJ
    #undef docalld
#endif 


// Filter out things required when compiling DynObj.cpp
#ifdef DO_IMPLEMENT_DYNOBJ
    // Setup library name and version 
    // Only required when compiling DynObj.cpp and when compiling a library
    #ifdef DO_MODULE
        /** Defines the name of the library being compiled. The name is platform 
            independent and coincides with stripped name of a DLL/SO, i.e the name 
            "DBase" corresponds to "DBase.dll" and libDBase.so */
        #ifndef DO_LIB_NAME
            #define DO_LIB_NAME "YourLibNameHere"
            #error Library name not defined in module setting!
        #endif
        #ifndef DO_LIB_AUTHOR
            #define DO_LIB_AUTHOR "Name/Organization Here"
            #error Library author not defined in module setting!
        #endif
        #ifndef DO_LIB_DESCR
            #define DO_LIB_DESCR "No description"
        #endif 
        #ifndef DO_LIB_MAJOR_VERSION
            #ifndef MSVC_COMPATIBLE
                #warning Library version undefined. Defining major/minor version to 0.50
            #else
                #pragma message("Library version undefined. Defining major/minor version to 0.50")
            #endif
            /** Defines the library major version */
            #define DO_LIB_MAJOR_VERSION 0
            /** Defines the library minor version */
            #define DO_LIB_MINOR_VERSION 50
        #endif
    #else
        // For the main app, these have no meaning
        #define DO_LIB_NAME "Main App"
        #ifndef DO_LIB_AUTHOR
            #define DO_LIB_AUTHOR "---"
        #endif
        #ifndef DO_LIB_DESCR
            #define DO_LIB_DESCR "This is the main app"
        #endif 
        #define DO_LIB_MAJOR_VERSION 1
        #define DO_LIB_MINOR_VERSION 0
    #endif 
#endif


/** If we should use the class DoRunTimeI or not. When using several DynObj 
  * libraries at the same time, DoRunTime provides useful functionality. It has
  * global type database (making sure types can be resolved across library boundaries), 
  * it has a shared named object pool, it keeps track of per-object errors. For 
  * a library, using DoRunTimeI does not consume resources, since since the instance 
  * will be with the main application. Default: 1 for both app & module */
#ifndef DO_USE_RUNTIME
    #define DO_USE_RUNTIME 1
#endif


/** If we should use the class DynSharedI or not. It implements ref-counted 
  * ownership and adds little overhead. It depends on pi/Atomic.cpp).
  * Default: 1 */
#ifndef DO_USE_DYNSHARED
    #define DO_USE_DYNSHARED 1
#endif

/** Whether we should use weak references in DynObjHolder or not.
  * We get dependency on utils/Notifiable.cpp and pi/RWLock.cpp. From a main 
  * app with DO_USE_RUNTIME, it is automatically defined.
  * Default: 1 if we use DoRunTime and if we are main, else 0 */
#ifndef DO_USE_NOTIFIER
    #if DO_USE_RUNTIME==1 && defined(DO_MAIN)
        #define DO_USE_NOTIFIER 1
    #else
        #define DO_USE_NOTIFIER 0
    #endif
#endif

/** When using several base classes, virtual functions of the bases
  * are 'swallowed' by the compound class if it defines them. In C++ 
  * no selective function overriding is available, and a side-base class 
  * cannot prevent a derived class from overriding.
  *
  * With VTable correction, this can be undone at run-time, efficiently
  * letting base classes maintain 'local virtual' methods.
  * It requires allocating and installing a new VTable for the object
  * and this can break compiler specific usage of VTables (for this particular
  * class) (this would usually be ordinary RTTI and potentially exception handling).
  *
  * It enables more efficient type lookups, since we can be sure to get the 
  * type of the local sub-object, not the type of the full object.
  *
  * It only affects side base classes when using multiple inheritance.
  * VTables of other classes are untouched. 
  *
  * Default: 0 - not used */
#ifndef DO_ENABLE_VTABLE_CORR
    #define DO_ENABLE_VTABLE_CORR 0
#endif


/** This produces a VObj class that must link with DynObj.cpp and provides a 
  * number of practical non-virtual VObj functions. 
  * Default: 1 */
#ifndef DO_FAT_VOBJ
    #define DO_FAT_VOBJ 1
#endif


/** The doTypeInfo template enables going from C++ type to a type name and
  * type ID. This macro can turn off that ability, to save some memory
  * and reduce global symbol usage. However, then do_cast<Type> and DynI::Cast<Type>
  * will not work.
  * Default: 0 */
#ifndef DO_NO_TYPEINFO
    #define DO_NO_TYPEINFO 0
#endif


/** Function call convention. How function parameters are passed and
  * how the stack is cleaned up after a function call. We define the
  * macro docall to be used in each virtual function declaration.
  *
  * On Unices, __decl is the default and does not have to be specified
  * so docall is set to an empty string. 
  *
  * On Windows, it is best to use __cdecl (parameters pushed right-to-left,
  * caller clears the stack).
  *
  * Alternatives are __stdcall, __fastcall, __thiscall
  * __stdcall is used for script language calls on Win32
  * __fastcall is very compiler dependent .
  *
  * Default: __cdecl on Windows, empty on Unices */
#ifndef docall
	#ifdef __WINDOWS__
		#define docall __cdecl 
        #define DO_CALLSTR "__cdecl"
	#elif defined(__LINUX__) || defined(__UNIX__) 
		// On Unices, this can be skipped (__cdecl by default)
		#define docall 
        #define DO_CALLSTR ""
	#endif
#endif
#ifndef DO_CALLSTR
    #error No DO_CALLSTR defined!
#endif



/** This enables invoking member functions using the VTable directly,
  * as opposed to using it indirectly through the compiler. Methods are called 
  * by looking them up in the VTable using the method index. Currently only used 
  * for casts based on string when calling DynI::doGetType on side bases. It 
  * causes dependency on SpinLock.cpp. 
  * Default: 0 */
#ifndef DO_ENABLE_VTABLE_DISPATCH
    #define DO_ENABLE_VTABLE_DISPATCH  0
#endif


/** The macro DI_GET_TYPE() retrieves the type of a DynI. In the case of the
  * instance being a side base (in a multiple base class object), the method
  * DynI::doGetType depends on the VTable being 'corrected' (in doConstruct), 
  * otherwise, the compound object type is (incorrectly) returned. The call 
  * DynI::doGetType simply returns a global variable, where as ::doGetType(pdi) 
  * has to do a lookup in a hash table. */
#ifndef DI_GET_TYPE
    #if DO_ENABLE_VTABLE_CORR==1
        // VTables are corrected, so DynI::doGetType can be trusted
        #define DI_GET_TYPE(pdi) (pdi)->doGetType(NULL)
        /** Means we're using virtual member function to get type */
        #define DI_GET_TYPE_VIRTUAL
    #else    
        // Not sure if DynI::doGetType works for side bases, call global version
        #define DI_GET_TYPE(pdi) ::doGetType(pdi)
        /** Means we're using the global function to get type */
        #define DI_GET_TYPE_GLOBAL
    #endif
#endif 

/** This option enables a DynObjLib to keep track of the number of objects
  * it has created and know if there are still instances around on library
  * unload. It requires DO_FAT_VOBJ since it depends on a trigger in the 
  * DynObj destructor. When the main application activates it at run-time, 
  * it has some cost in terms of memory and performance. Implementation 
  * depends on DoRunTime. 
  * Default: 1 if both DO_FAT_VOBJ and DO_USE_RUNTIME, else 0 */
#ifndef DO_ENABLE_OBJECT_TRACKING
    #if DO_FAT_VOBJ==1 && DO_USE_RUNTIME==1
        #define DO_ENABLE_OBJECT_TRACKING 1
    #else
        #define DO_ENABLE_OBJECT_TRACKING 0
    #endif
#endif

/** One can implement an operator delete inside DynObj to route this call
  * doDestroy. Similarly, one can do the same to give an error for DynI.
  * This works when there is no other customized operator delete at work.
  * But when using a custom memory allocator, this scheme might not work
  * anymore.
  * It is best to delete DynObjs using doDestroy directly.
  * Default: 0. */
#define DO_ENABLE_OPERATOR_DELETE 0



/** Handle virtual destructors in VTables. A DynObj instance does not rely on use 
  * of virtual destructors. If a class with a virtual destructor has to be exposed 
  * as a DynObj, then, to another module, chances are that two different compilers use 
  * the entrie(s) in the VTable in slightly different ways. 
  *
  * Therefore it is best not to invoke virtual destructors on DynObj:s. 
  * Calling doDestroy gives the job of destroying the object to the module which 
  * it. It knows how to use the destructors.
  * 
  * Also since some compilers (g++, others?) use two VTable slots for a virtual destructor 
  * and most other compiler only one, we have to compensate for that. The macro below can 
  * be pasted into an exposed class to make sure the VTables will have the same size 
  * regardless of compiler.
  * 
  * Add other compilers here. */
#if defined(__GNUC__) || \
    defined(__xlc__) || defined(__xlC__)
    #define DO_USE_DTOR_PADDING
#endif

#ifdef DO_USE_DTOR_PADDING
    #define DO_VTABLE_DTOR_PADDING  virtual void docall do_dtor_padding() { }
#else
    #define DO_VTABLE_DTOR_PADDING 
#endif


// Switchable assert
#ifndef DO_ASSERT_MSG
    #ifdef DO_USE_PIASSERT 
        // Adds dependency on pi/Assert.cpp, pi/ErrorLog.cpp 
        #include "pi/Assert.h"
        #define DO_ASSERT_MSG(v,msg) piAssert(v,msg)
    #else
        #ifdef DO_DEBUG
            #include <assert.h>
            #define DO_ASSERT_MSG(v,msg)  { if(!v){ DO_PUTS(msg); assert(0); } }
        #else
            #define DO_ASSERT_MSG(v,msg)  { if(!v){ DO_PUTS(msg); } }
        #endif 
    #endif
#endif 

/** @} */


#endif // DOSETUP_H
