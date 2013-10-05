// Copyright (c) 2007, Arne Steinarson
// Licensing for DynObj project - see DynObj-license.txt in this folder


#ifndef DYNOBJ_H
#define DYNOBJ_H


// These are directives to preprocessing tool that auto-generates some of the required code	
// %%DYNOBJ	options generate(implement)
	
// Defines and config options are in here
#include "DoSetup.h"

// Type system
#include "DoType.h"


// Error logging
#include "DoError.h"


// Preprocessor symbols controlling how DynObj.h and related files:
//
// DO_IMPLEMENTING    - from a C++ source file that implements a DynObj
// DO_MAIN            - from the main program (that loads other DynObjs and has the DoRunTimeI instance)
// DO_MODULE          - Set when compiling a library/loadable module (that does not own DoRunTimeI)
// DO_IMPLEMENTED_TYPES - Define before compiling DynObj.cpp to set to a list of implemented
//                        type names /type IDs. This how types are made 'public'. Example
//
//                            "string,0xB314DA4,Car:VehicleI,0xBA618F23"
//
//                        Another method is to use doAddImplemented or global instances of
//                        class DoImplDecl (later this file).
//                        

#ifdef DO_MODULE
	#ifdef DO_MAIN 
		#error Both DO_MODULE and DO_MAIN are defined (only one should be)
	#endif
#elif defined(DO_MAIN)
	// Fine, do nothing
#else
	// Proceed until the defs are needed, give warning there
#endif


// Doxygen stuff
/** @defgroup DynI DynI derived classes */
/** @defgroup VObj VObj interfaces and base classes (not derived from DynI) */
/** @defgroup ImplClass Other classes */
/** @defgroup DynI derived classes */
/** @defgroup Utils Global functions, enums */
/** @defgroup Library Library interface and setup */
/** @defgroup DynTemplate Central DynI templates */
/** @defgroup DynDefs DynObj defines */


// Object IDs: 
// They can be generated automatically from the "pdoh" tool (Parse DynObj Header). 
// Layout of ID:
// Bit 0..7   - Set to 0 for now
// Bit 8..30  - Custom type ID
// Bit 31     - Set for user-type without VTable 
// Custom IDs start at BASE_CUSTOM_TYPE_ID (0x10000)
// The next one is 0x10100, 0x10200, and so on

#define TYPE_ID_MASK          0xFFFFF000        // Bits 12..31

// Base type IDs: - They should probably be positive
#define SIDEBASE_TYPE_ID	  0xFFFFF000		// All side bas classes have this type ID (cannot be addressed individually from outside)
#define OUTERMOST_TYPE_ID	  0xFFFFD000		// When casting to the outermost object (the container object) (string OUTERMOST)
#define COMPOUND_TYPE_ID	  0xFFFFE000		// When casting to a compound object (the container object) (string COMPOUND)
#define INVALID_TYPE_ID       0xFFFFF000        // Just same as unknown for now

#define UNKNOWN_TYPE_ID	      0					// Hmmm...

#define VOBJ_TYPE_ID          0x00001000
#define DYNI_TYPE_ID          0x00002000
#define DYNOBJ_TYPE_ID        0x00003000
#define DYNSHAREDI_TYPE_ID    0x00004000

#define DYNDATA_TYPE_ID       0x00006000
#define RTDYNSHARED_TYPE_ID   0x00008000

#define DORUNTIMEI_TYPE_ID    0x00010000
#define DORTINTERNALI_TYPE_ID 0x00011000
#define DOMODULEC_TYPE_ID     0x00012000
#define DORT_TYPE_ID          DORUNTIMEI_TYPE_ID

#define DYNSTR_TYPE_ID        0x00013000

#define DYNARRCHAR_TYPE_ID      0x00020000
#define DYNARRSHORT_TYPE_ID     0x00021000
#define DYNARRINT_TYPE_ID       0x00022000
#define DYNARRINT64_TYPE_ID     0x00023000
#define DYNARRPTR_TYPE_ID       0x00024000
#define DYNARRPTR_VOBJ_TYPE_ID  0x00025000
#define DYNARRPTR_DYNI_TYPE_ID  0x00026000

#define NOTIFIERI_TYPE_ID     0x00030000
#define NOTIFIABLEI_TYPE_ID   0x00031000
#define NOTDATAI_TYPE_ID      0x00032000
#define BASE_CUSTOM_TYPE_ID   0x00100000

#define STEP_TYPE_ID          0x00001000    // How much to increment for each type


#if DO_NO_TYPEINFO!=1
/** Translate C++ type to type string and type ID.
 * This provides a way to get type ID and type name for any known (registered) class. 
 * Implement for each class that is exposed. See DO_DECL_TYPE_INFO further down.
*/
template<class T>
struct doTypeInfo {
	static int Id( ){ return UNKNOWN_TYPE_ID; }
	static const char* Name( ){ return NULL; }
	//enum { TypeId=INVALID_TYPE_ID };
};

/// @cond internal
//  Peels off pointer
template<class T>
struct doTypeInfo<T*> {
public:
	static int Id( ){ return doTypeInfo<T>::Id(); }
	static const char* Name( ){ return doTypeInfo<T>::Name(); }
	//enum { TypeId=doTypeInfo<T>::TypeId };
};

//  Peels off ref
template<class T>
struct doTypeInfo<T&> {
public:
	static int Id( ){ return doTypeInfo<T>::Id(); }
	static const char* Name( ){ return doTypeInfo<T>::Name(); }
	//enum { TypeId=doTypeInfo<T>::TypeId };
};

// Peels off const
template<class T>
struct doTypeInfo<const T> {
public:
	static int Id( ){ return doTypeInfo<T>::Id(); }
	static const char* Name( ){ return doTypeInfo<T>::Name(); }
	//enum { TypeId=doTypeInfo<T>::TypeId };
};
#endif //DO_NO_TYPEINFO
/// @endcond internal        //squRegisterMemberFn( sd.v, sd.newClass.GetObjectHandle(), sqbVObj_GetObj, _T("GetObj") );



#if DO_FAT_VOBJ==1
// Need this proto below
void* doGetObj( void* pobj, const char *type, DynObjType *self_type, doCastAlgo algo=doCastCross, DynObjType **found_type=NULL );
void* doGetObj( void* pobj, int type_id, DynObjType *self_type, doCastAlgo algo=doCastCross, DynObjType **found_type=NULL );
void* doGetObj( DynI* pdi, const char *type, doCastAlgo algo=doCastCross, DynObjType **found_type=NULL );
void* doGetObj( DynI* pdi, int type_id, doCastAlgo algo=doCastCross, DynObjType **found_type=NULL );
DynObjType* doGetType( const void* pobj );
DynObjType* doGetType( const DynI* pdi );
#endif

/** @ingroup VObj 
 * This is the 'stipulated' base class for objects with virtual functions.
 * 
 * In this class the virtual functions (number of them, signatures) are not known.
 * It only promises that it has a VTable, and we can do type lookups from that.
 * VObjs can be used when we have C++ classes that don't derive from DynI.
 * Objects of type VObj are not created, but one can get pointers to them through
 * casts/queries (doGetVObj). 
 */
// %%DYNOBJ class(vobj)                <- Tag for header file preprocessing  
class VObj {
public:
    // With fat VObj we get a lot of non-virtual convenience functions for the base class. 
    // NOTE: Requires static linking (to DynObj.cpp)
    #if DO_FAT_VOBJ==1
    /** @name Non-virtuals 
     *  Functions included with DO_FAT_VOBJ define */
    //@{ 
    
   /** Returns type structure for this object. 
    * Does a global lookup using object VPTR.
    */
	DynObjType* GetType( ) const;
	
    /** Test if object is of a certain type ID 
     * IsA returns true if the requested type can be returned without 
     * adjusting the 'this' pointer (must be main base class).
     */
	bool IsA( int type_id ) const       { return IsA(type_id,NULL); }
    
    /** Test if object 'can be' of a certain type ID 
     * CanBeA returns true if the requested type is available as any base
     * class of the object (main base or side base).
     */
	bool CanBeA( int type_id ) const    { return CanBeA(type_id,NULL); }
    
    /** Same as IsA but using type string */ 
    // Lookup is not done on custom addon types.
	bool IsA( const char *type ) const  { return IsA(type,NULL); }
	//bool CanBeA( const char *type )     { return CanBeA(type,NULL); }

    /** Raw cast, lookup internal type by type ID. */
    void* GetObj( int type_id, doCastAlgo algo=doCastCross ) { return ::doGetObj(this,type_id,NULL,algo); }

    /** Raw cast, lookup internal type by type string. */
    void* GetObj( const char *type, doCastAlgo algo=doCastCross ) { return ::doGetObj(this,type,NULL,algo); }

    /** Returns current error string for object or NULL if no error
     * @param perr_code a location to store the error code, default is NULL
     * @return The error string or NULL if no error. 
     */
    const char* GetError( int* perr_code=NULL ) const;
    
    /** This will clear the error state for the object if set */
    void ClearError( );
    
    /** Set the internal error state for the object */
    bool SetError( const char *err_str, int err_code=-1 ) const;

	bool IsA( int type_id, DynObjType *ptype ) const;
	bool CanBeA( int type_id, DynObjType *ptype ) const;
	bool IsA( const char *type, DynObjType *ptype ) const;
	//bool CanBeA( const char *type, DynObjType *ptype  );
	const char *GetTypeName( ) const { 
		DynObjType *pdt = ::doGetType(this);
		return pdt ? pdt->_type : NULL; 
	}

    /** Check if object exists and has no errors 
     * Check if 'this' is non-NULL and that object has no error 
     */
	bool IsOk( ) const { return this && !GetError(); } 
    //@}
    #endif // DO_FAT_VOBJ
};


/** @ingroup DynI 
 * The effective root class of the DynObj library.
 *
 * This is the base type representing objects, interfaces and sub-objects 
 * created by the DynObj package. In itself it is an interface. It knows its
 * own type and can be queried for custom owned objects.
 *
 * For compatibility with MSVC++, the member function calling convention 'docall' 
 * must be put between the return type and the function name:
 *   virtual int ->docall<- MyFunc( int i );
 */
// %%DYNOBJ class(dyni)             <- Tag for header file preprocessing 
class DynI : public VObj {
public:
    /** @name Virtuals
     *  Interface members */
    //@{ 
    
   /** Returns type of this object.
    * Each type that wants to make itself known should override this method.
    * DynI knows its own type so we get the type without any lookup. 
    * It is faster and works without registration. 
    *
    * @param pself allows fetching the actual 'this' used inside doGetType
    * (from a side base, this will be different to the 'this' of the caller).
    * @return A DynObjType pointer
    */
	virtual DynObjType* docall doGetType( const DynI **pself=NULL ) const;
	
   /** 'Smart cast' (parallel to QueryInterface) used to retrieve another
     * interface or sub-object that the object supports. 
     * The default implementation of doGetObj traverses the registered main and side
     * base classes for the object and returns any match found there. This is the same
     * search as done when querying for types using type IDs.
     *
     * A derived version can also check the argument type string and return a pointer
     * to an owned object that is not a base class (custom types). The rule is that the 
     * returned object should have the same life span as this one, they are part of the 
     * same compound object. 
     * 
	 * When overriding this method, and not returning a custom object directly, one should 
     * call the immediate base class version. Then, after traversing the inheritance chain, 
     * the call will end up in the base version DynI::doGetObj. There, the call will be 
     * made to check for any registered base class types.
     *
     * For versions with type safety, use do_cast or do_cast_str.
     * 
     * @return A pointer to the found interface/internal object.
    */
	virtual void* docall doGetObj( const char *type );		// Works for all objects, even data structures

    /** Returns current error string for object or NULL if no error.
     * Object errors are used to handle errors that are not detected through return 
     * codes, i.e. exceptions. The default implementation uses DoRunTime for this. Errors 
     * are unique to the object but stored outside of it. No memory is used for this until 
     * an error is set.
     * @param perr_code a location to store the error code, default is NULL
     * @return The error string or NULL if no error. 
     */
    virtual const char* docall doGetError( int* perr_code=NULL ) const;

    /** This will clear the error state for the object if set */
    virtual void docall docall doClearError( );
    //@}

    #if DO_FAT_VOBJ==1
    /** @name Inlines
     *  Inline convenience functions */
    //@{ 
    
	// Convenience functions for accesing DynI type info
	// Non virtual, inline, generated on both sides, therefore OK 
	DynObjType *GetType()         const { return ::doGetType(this); }
	bool        IsOk( )       	  const { return this && !doGetError(); } 
	const char *GetTypeName( )    const { DynObjType *pdt=DI_GET_TYPE(this); return pdt ? pdt->_type : NULL; }
	int         GetTypeId( )      const { DynObjType *pdt=DI_GET_TYPE(this); return pdt ? pdt->_type_id : -1; }
	int         GetTypeVersion( ) const { DynObjType *pdt=DI_GET_TYPE(this); return pdt ? pdt->_version : -1; }
	int         GetObjectSize( )  const { DynObjType *pdt=DI_GET_TYPE(this); return pdt ? pdt->_size : -1; }

	bool IsA( int type_id )            const { return VObj::IsA( type_id, DI_GET_TYPE(this) ); }
	bool CanBeA( int type_id )         const { return VObj::CanBeA( type_id, DI_GET_TYPE(this) ); }
	bool IsA( const char *type )       const { return VObj::IsA( type, DI_GET_TYPE(this) ); }
	//bool CanBeA( const char *type )     { return VObj::CanBeA( type, DI_GET_TYPE(this) ); }
    
    /** Raw cast, lookup internal type by type ID. */
    void* GetObj( int type_id, doCastAlgo algo=doCastCross ) { return ::doGetObj(this,type_id,algo); }

    /** Raw cast, lookup internal type by type string. */
    void* GetObj( const char *type, doCastAlgo algo=doCastCross ) { return ::doGetObj(this,type,algo); }

    //@}
	
    /** @name Others
     *  Other member functions */
    //@{ 

	// This is only intended for user of a DynObj to be able to use ordinary
	// delete on the object and route the call back to the creator. 
	// NOTE: When an application defines its own operator new and/or redefines
	// the new/delete keywords (to support memory tracking), this may not work.
	// To be safe its best to call doDestroy directly from the app.
    #if DO_ENABLE_OPERATOR_DELETE==1 
	void operator delete( void *pv ){ 
        SetError( "DynObj::operator delete on a DynI", DOERR_DELETE_ON_DYNI );
	}
    #endif // DO_ENABLE_OPERATOR_DELETE
    //@}
    #endif // DO_FAT_VOBJ
};

// Used as a NULL object 
#define DynINull (*(DynI*)(0))

/** @ingroup DynI 
 * This class represents objects that can be created and destroyed. It is the 
 * type of new objects created (from type names/type IDs). To destroy a DynObj, 
 * the function doDestroy is used. 
 */
// %%DYNOBJ class(dyni)                   <- Tag for header file preprocessing 
class DynObj : public DynI {
public:
    /** @name Virtuals
     *  Interface members */
    //@{ 
    
	// All DynI derived types overrides this
	virtual DynObjType* docall doGetType( const DynI **pself=0 ) const;
	
   /** doDestroy is invoked by the owner to delete it. 
    * An implementation should both call its own most derived C++ destructor and 
    * the object memory. A typical implementation would be:
	*   MyClass::doDestroy(){ ::delete this; }
    */
	virtual void    docall doDestroy( ) { };
	
protected:    
   /** doDtor provides access to the object object destructor.
    * Currently not used. The intention with this member is to provide a way to 
    * invoke an object destructor without freeing its memory. It would allow for 
    * creating objects as sub-objects of other main objects. The way to implement 
    * it would be:  MyClass::doDtor(){ this->~MyClass(); } 
    */
	virtual void    docall doDtor( ){ }
    //@}
public:
    
    /** @name Others
     *  Other functions */
    //@{ 

    #if DO_FAT_VOBJ==1
   /** This destructor will notify the object tracker if activated. */
    ~DynObj();
    #endif // DO_FAT_VOBJ
    
    #if DO_ENABLE_OPERATOR_DELETE==1
	/** Route operator delete back to doDestroy. 
	 * NOTE: When an application defines its own operator new and/or redefines
	 * the new/delete keywords (to support memory tracking), this may not work.
	 * To be safe its best to call doDestroy directly.
     */
	void operator delete( void *pv ){ 
		if( !pv ) return;
        if( pv!=this )
            SetError( "DynObj::operator delete on wrong object", DOERR_DELETE_ON_WRONG_OBJECT );
        else
            doDestroy();
	}
    #endif // DO_ENABLE_OPERATOR_DELETE
    //@}
};

/** @ingroup DynI 
 * This is the base class for a ref-counted object. Derive from DynSharedI and 
 * implement ref-counting (support for atomic ref handling in pi/Atomic.h). 
 *
 * It adds ref counting methods to the object: doAddRef, doRelease. On object
 * creation, it is assumed that the internal ref counter is set to 1 (NOTE: not 0).
 *
 * DynSharedI derives from DynObj (more fundamental) since a ref-counted object 
 * depends on a mechanism to delete itself (which DynObj provides). 
 *
 * The DynSharedC is a default implementation that implements the ref counting
 * methods. 
 *
 * There are some other helpful classes: 
 *
 * -  ToDynShared<T> when converting a DynObj (or really any other object)
 *    to a shared object at compile time. do_cast<SomeType*>(p) will work for 
 *    querying types.
 *    
 * -  RTDynShared does the same at run-time, given a DynObj*. do_cast<SomeType*>(p) 
 *    will work (partially) for querying types.
 */
// %%DYNOBJ class(dyni)                         <- Tag for header file preprocessing 
class DynSharedI : public DynObj {
public:
	// All DynI derived types overrides this
	virtual DynObjType* docall doGetType( const DynI **pself=0 ) const;
	
	/** Increases the reference count of the object 
     * @return the updated ref count value
    */
	virtual int docall doAddRef( ) = 0;
	
	/** Decreares the reference count of the object and destroy if zero.
     * @return the updated ref count value
     */
	virtual int docall doRelease( ) = 0;
};


#if DO_NO_TYPEINFO!=1

/** @addtogroup Utils*/
//@{ 

/** Macro to declare type registration info.
 * Can use preprocessor pdoh instead to automate this. */
#define DO_DECL_TYPE_INFO(CLASS,ID) \
template<>                          \
struct doTypeInfo<CLASS> { \
public:                             \
    static int Id( ){ return ID; }  \
    static const char* Name( ){ return #CLASS; } \
};

/*
#define DO_DECL_TYPE_INFO_ALIAS(CLASS,REALCLASS)     \
template<>                                           \
struct doTypeInfo<CLASS> {                           \
public:                                              \
    static int Id( ){ doTypeInfo<REALCLASS>::Id(); } \
    static const char* Name( ){ return doTypeInfo<REALCLASS>::Name(); } \
};                             
//	enum { TypeId=doTypeInfo<REALCLASS>::TypeId };    \
*/

#else   // DO_NO_TYPEINFO
#define DO_DECL_TYPE_INFO(CLASS,ID)
#define DO_DECL_TYPE_INFO_ALIAS(CLASS,REALCLASS)
#endif  // DO_NO_TYPEINFO


// doTypeInfo for VObj, DynI and DynObj and DynSharedI
/// @cond internal
DO_DECL_TYPE_INFO(VObj,VOBJ_TYPE_ID);
DO_DECL_TYPE_INFO(DynI,DYNI_TYPE_ID);
DO_DECL_TYPE_INFO(DynObj,DYNOBJ_TYPE_ID);
DO_DECL_TYPE_INFO(DynSharedI,DYNSHAREDI_TYPE_ID);
/// @endcond   // internal


/** Retrieve the type of the object. 
  * @param pobj Object 
  * @return DynObjType of object or NULL if not found. */
DynObjType* doGetType( const void* pobj );

/** Retrieve the type of the object. 
  * @param pdi Object 
  * @return DynObjType of object or NULL if not found. */
DynObjType* doGetType( const DynI* pdi );


/** Get sub-object or interface. 
  * @param pobj Query object 
  * @param type string 
  * @param self_type Type of pobj or NULL if not known 
  * @param algo Algoithm to use in cast
  * @param found_type Pointer-to-pointer: store found type here if non-NULL
  * @return Pointer to interface/object */
void* doGetObj( void* pobj, const char *type, DynObjType *self_type, doCastAlgo algo, DynObjType **found_type );

/** Get sub-object or interface. 
  * @param pobj Query object 
  * @param type_id Integer type ID
  * @param self_type Type of pobj or NULL if not known 
  * @param algo Algoithm to use in cast
  * @param found_type Pointer-to-pointer: store found type here if non-NULL
  * @return Pointer to interface/object */
void* doGetObj( void* pobj, int type_id, DynObjType *self_type, doCastAlgo algo, DynObjType **found_type );

/** Version of doGetType (with type string) for DynI */
void* doGetObj( DynI* pdi, const char *type, doCastAlgo algo, DynObjType **found_type );

/** Version of doGetType (with type ID) for DynI */
void* doGetObj( DynI* pdi, int type_id, doCastAlgo algo, DynObjType **found_type );


/** Find DynObjType by type string. 
  * @param type Type name string
  * @return Pointer to DynObjType or NULL if not found */
DynObjType *doFindType( const char *type );

/** Find DynObjType by type ID. 
  * @param type Type name string
  * @return Pointer to DynObjType or NULL if not found */
DynObjType *doFindType( int type_id );


/** Ends startup phase and checks that initializion went OK. 
  * Registers all known types with DoRunTimeI. Verifies that all types 
  * were correctly resolved. 
  * Usually called only by main program.
  * @return true on success */
bool doInit( );


/** Set up a type from an instance and register it. 
  * This should be done 
  * - For the first instance of each type created
  * - If using VTable correction, it must be done for each object of a type 
  *   that needs correction (if it has virtual methods to protect from derived /
  *   compound overriding [for example DynI]).
  * NOTE: This is done automatically when creating objects through DoRunTime. 
  * When creating instantiating objects in other ways, one must us DoConstruct
  * manually
  * @param obj The object to register 
  * @param self_type Type of the object or NULL if not known 
  * @param is_outer set to true when registering an object from outside
  * (when it is not a side base class) 
  * @return true if successful.
  */
bool doConstruct( const VObj *obj, DynObjType *self_type, bool is_outer=true );


/** Call doRelease or doDestroy on the object. 
  * Used to end ownership of a DynObj which can derive either
  * directly from DynObj or from DynSharedI. 
  * @param pdo Object to release or destroy
  */ 
void doReleaseDestroy( DynObj *pdo );

/** Check if an object is a static/global instance.
  * A static instance will have equal or longer lifetime compared to 
  * the calling module.  
  * @param pdo Object to release or destroy
  * @param type if of object if known
  * @return true if valid object and it is static 
  */
bool doIsStatic( const VObj *pvo, DynObjType *type=NULL );
bool doIsStatic( const DynI *pdi );

/** Returns a pointer to the shared instance of doRunTime. Valid if we're compiling 
  * with DO_USE_RUNTIME, and after the pointer has been setup.
  */ 
class DoRunTimeI;
DoRunTimeI &doGetRunTime();

// Add implemented type for exporting (a type for which the user can give 
// its name to the library and receive a new instance)
bool doAddImplemented( const char *type, int type_id );
const char *doGetImplemented( );

// Use global variable to automatically register exported type on module load
class doUserTypeDecl {
public: 
	doUserTypeDecl( const char *type, int type_id ) {
		doAddImplemented(type,type_id);
	}
};
//@} // Utils



//
//	Functionality implemented by DynObj libraries.
//

/** @addtogroup Library*/
//@{ 

// Used in library init functions below
class DoRunTimeI;
struct DynObjLibImpl;


//
// The object creation function should be named 'CreateDynObj'.
//
// The object destroy function should be named 'DestroyDynObj'.
//
// Library init function is named 'InitDynLib' and returns a pointer to a 
// DynObjLibImpl structure (if library is compiled fromDynObj.cpp 
// this is not necessary, unless custom initialization must be done).
//
// Library exit function named 'ExitDynLib' (not mandatory).
// - not implemented - The set base creation function should be named 'SetBaseCreate'.
// All these functions are exported with 'extern "C" SYM_EXPORT'. 

/** Protoptype for library functions that create objects .
  * This function is usually named 'CreateDynObj'.
  * The create function is free to create an object even if only one of
  * type_id or type is defined.
  * @param type The type string naming type of object to be created
  * @param type_id The type ID of the object to be created
  * @param di_init A DynI object to be passed to constructor of new object
  * @param obj_size Size of memory to allocate for object. Currently not used and always
  * set to 0 which means don't care.
  * @return The returned object is derived from DynObj (directly or indirectly)
  */
typedef DynObj* (*CreateDynObjFn)( const char *type, int type_id, const DynI* pdi_init, int object_size );


/** Protoptype for library init functions. 
  * Library init function is named 'InitDynLib' and returns a pointer to a 
  * DynObjLibImpl structure. If library is linked with DynObj.cpp 
  * this is not necessary it can return NULL. 
  * @param pdort The pointer to DoRunTimeI. Allows the library to connect with 
  * shared resources.
  * @return A pointer to a DynObjLibImpl structure that describes types
  * implemented by the plugin library and the compiler used to generate it.
  */
typedef DynObjLibImpl* (*InitDynLibFn)( DoRunTimeI* pdort );

/** Prototype for function called at library unload. 
  * If is_query is true, the library will not be closed now, given that the 
  * function returns false.
  * @param is_query is set to true if the library can 'object' to shutdown.
  * @return true if library is ready to shut down (it may be shutdown anyway).
  */
typedef bool (*ExitDynLibFn)(bool is_query);

/** Protoptype for library functions that destroys objects. 
  * The default way to delete is through doDestroy, so this is not necessary to
  * implement. When destroying objects not derived from DynObj/DynSharedI
  * it is required.
  * @param pv is a pointer to object to destroy
  * @param pdt is a pointer to the type of the object
  * @return true on succesful destroy.
  */
typedef bool (*DestroyDynObjFn)( VObj *pv, DynObjType *pdt );

class DoModuleC;

/** Holds information about a DynObj library. 
  * Returned on library initialization. 
  */
struct DynObjLibImpl {
    DynObjLibImpl(const char *cs, const char *impl, int dt, int pf);
  
    StructHeader _sh;           /**< Verifies that this is a struct of the right type */
    
    const char *_lib_name;      /**< Name of library */
    const char *_lib_descr;     /**< Description of library */
    const char *_lib_author;    /**< Author of library */
    
    const char *_call_str;      /**< Calling convention */
    const char *_implements;    /**< Interfaces & classes implemented */
    int   _do_traits;           /**< Describing object layout properties */
    int   _platform;            /**< Platform where library was built */
    int       _lib_major_ver;   /**< Library major version */
    int       _lib_minor_ver;   /**< Library minor version */
    
    DynObjType **&_types;       /**< Pointer to types used/implemented in library (NULL terminated array) */
    DoModuleC  *_module;        /**< ModuleI for this library */
    
    const char *_compiler;      /**< Library compiler */
    int        _comp_major_ver; /**< Major version of library compiler */
    int        _comp_minor_ver; /**< Minor version of library compiler */
    int        _lib_flags;      /**< Library flags */
    int        _dynobj_major_ver; /**< Major version of DynObj framework */
    int        _dynobj_minor_ver; /**< Minor version of DynObj framework */
};

#define DOLI_USES_RUNTIME       1
#define DOLI_OBJECT_TRACKING    2

//@} // Library


// Macros to generate and extract platform info above
#define DO_PLATFORM_FLAGS ( PLATFORM_FLAGS | (PFM_INT_SIZE<<16) | (PFM_PTR_SIZE<<18) | (PFM_WCHART_SIZE<<20) )


// The compiler will likely set sizeof(VObj) to 0 or 1, however, we want 
// the VPTR to be part of the size
#define VOBJ_SIZE sizeof(void*)

// %%DYNOBJ section implement
// This section is auto-generated and manual changes will be lost when regenerated!!
#ifdef DO_IMPLEMENT_DYNOBJ

// Generate type information that auto-registers on module load
DynObjType g_do_vtype_VObj("VObj",VOBJ_TYPE_ID,1,VOBJ_SIZE);
DynObjType g_do_vtype_DynI("DynI:VObj",DYNI_TYPE_ID,1,sizeof(DynI));
DynObjType* DynI::doGetType( const DynI **pself ) const {
   if(pself) *pself=(const DynI*)(const void*)this;
   return &g_do_vtype_DynI;
}
DynObjType g_do_vtype_DynObj("DynObj:DynI",DYNOBJ_TYPE_ID,1,sizeof(DynObj));
DynObjType* DynObj::doGetType( const DynI **pself ) const {
   if(pself) *pself=(const DynI*)(const void*)this;
   return &g_do_vtype_DynObj;
}
DynObjType g_do_vtype_DynSharedI("DynSharedI:DynObj",DYNSHAREDI_TYPE_ID,1,sizeof(DynSharedI));
DynObjType* DynSharedI::doGetType( const DynI **pself ) const {
   if(pself) *pself=(const DynI*)(const void*)this;
   return &g_do_vtype_DynSharedI;
}
#endif //DO_IMPLEMENT_...
// %%DYNOBJ section end

#endif // DYNOBJ_H


