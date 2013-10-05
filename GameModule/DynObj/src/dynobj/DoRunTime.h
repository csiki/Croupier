// Copyright (c) 2007, Arne Steinarson
// Licensing for DynObj project - see DynObj-license.txt in this folder

#ifndef DORUNTIME_H
#define DORUNTIME_H

#ifndef DO_USE_RUNTIME 
    #warning DO_USE_RUNTIME  is not defined. Setting it from DoRunTime.h 
    #define DO_USE_RUNTIME 1
#endif

// %%DYNOBJ section general
// This section is auto-generated and manual changes will be lost when regenerated!!
#ifdef DO_IMPLEMENT_DORUNTIME
    #define DO_IMPLEMENTING     // If app has not defined it already
#endif
#include "dynobj/DynObj.h"

// --- Integer type ids for defined interfaces/classes ---
#define DORUNTIMEC_TYPE_ID 0xDCCF5000

// --- Forward class declarations ---
class DoRunTimeC;

// --- For each declared class, doTypeInfo template specializations ---
DO_DECL_TYPE_INFO(DoRunTimeC,DORUNTIMEC_TYPE_ID);

// %%DYNOBJ section end


#ifndef DO_MAIN
	#define DO_MAIN		// DoRunTime is always in the main module
#endif

#include "DoBaseI.h"
#include "DoError.h"

// Access global DoRunTimeI through this function

#include "DynObj.h"
#include "utils/ExpArr.hpp"
#include "utils/HashTable.hpp"
#include "utils/CharBuf.h"
#include "DoBase.hpp"
#include "DortInt.h"

#include "DynObjLib.h"
#include "DoNotifiable.hpp"
#include "NamedRef.h"

//class DynObjLib;
struct DynObjError;
struct DOError;
class DynStr;
class DoVtblType;



/** @ingroup ImplClass
 * The global instance of DoRunTime.
 * Holds shared types and resources available tyo main program and 
 * the loaded libraries.
 */
// %%DYNOBJ class(dyni) bases(DoRunTimeI,NotifierI,NamedRefI,DortInternalI)
class DoRunTimeC : public DoRunTimeI, public Notifier, public NamedRef, 
                   public DortInternalI {
public:
	DoRunTimeC();
	~DoRunTimeC(){ Dtor(); }
	void Dtor();

	/** Get the one and only instance */
	static DoRunTimeC &GetInstance();

    /** @name Virtuals
     *  Interface members */
    //@{ 

	// Base DynObj functions
	virtual DynObjType* docall doGetType( const DynI **pself=0 ) const;
	virtual void*       docall doGetObj( const char *type );
	virtual void        docall doDestroy( ){ delete this; }

	//@{ 
    /** Access shared functionality */
	virtual MutexI*  docall  GetMutex() const { return m_pmut; }
	virtual MallocI* docall  GetMalloc() const { return m_pmalloc; }
	virtual TextIoI* docall  GetTextIo() const { return m_ptio; }
    //@}
	
	/** Object creation 
      * @param lib Library name or NULL if automatic search
      * @param type Type name (of created class)
      * @param type_id Type ID (of created class)
      * @param pdi_init Argument to constructor of new object
      * @param obj_size Not used - Size of memory to allocate 
      * @return Pointer to new DynObj */
	virtual DynObj*    docall  Create( const char *lib, const char *type, int type_id, const DynI* pdi_init=NULL, int obj_size=0 );

	//@{ 
    /** Global error handling - per thread */
	virtual int         docall GetLastErrCode( int thid=-1 );
	virtual bool        docall GetLastErrStr( DynStr& ds, int thid=-1 );
	virtual bool        docall SetLastError( int err_code, const char *err_msg );
	virtual int         docall GetThreadId( );
    //@}

	//@{ 
    /** Module registration - internal use */
	virtual bool        docall RegisterModule( DoModuleC *pmi );
	virtual bool        docall UnRegisterModule( DoModuleC *pmi );
    //@}
	
	//@{ 
    /** Manage types based on VPTR (VTable pointer) */
	virtual bool        docall RegisterType( void **vtbl, DynObjType *pdt );
	virtual bool        docall UnRegisterType( void **vtbl );
	virtual DynObjType* docall GetTypeOf( void **vtbl );
    //@}
	
	//@{ 
    /** Handle per object errors */
    virtual bool docall SetObjectError( const VObj *pvo, const char *err_str, int err_code );
    virtual bool docall ClearObjectError( const VObj *pvo );
    virtual const char* docall GetObjectError( const VObj *pvo, int *perr_code );
    //@}

    //@}    // Virtuals
    
    /** @name Others 
     *  Utrility functions */
    //@{ 
    
	// Takes ownership of the library
    /** Add/remove libraries. These functions take ownership of the libraries
      * (until library is removed). The libraries are unloaded at application 
      * shutdown */
	bool AddDOLib( DynObjLib *pdol );
	bool RemoveDOLib( const DynObjLib *pdol );
	/** Returns NULL on success, otherwise error from library loader */
	const char *LoadLib( const char *lib_name );
	/** Returns an owned library (for temporary use) */
	DynObjLib* GetLib( const char *lib );
	
	// Does not take ownership for these objects
    /** Change the exposed memory allocator. Must do before memory 
      * allocation begins! */
	void SetMalloc( MallocI *pmi ){ m_pmalloc=pmi; }
    
    /** Set Text IoI currently used. This is where logging and printing
      * goes to. */
	void SetTextIo( TextIoI *ptio ){ m_ptio=ptio; }

    /** Returns true if DoRunTime is alive and well and not shutting down */
    bool IsOk();
    
    /** Template wrapper of create above.
      * It uses a clever algorithm to guess which library to allocate from, 
      * so a search is avoided in most cases. */
    template<class T>
	T* Create( doTypeWrapper<T>, const char *lib, const DynI* pdi_init=NULL, int obj_size=0 ) {
        if( !IsOk() || m_tmp_disable<0 ) return NULL;
        
        // Check library is OK.
        static int st_lib_cache_ix = -1;
        if( st_lib_cache_ix==-1 ){
            // First time around for this type, alloc an index
            st_lib_cache_ix = m_lib_cache_ix++;
            m_type_libs.Push(NULL);
        }
        const char *type = doTypeInfo<T>::Name();
        int type_id = doTypeInfo<T>::Id();
        
        T *pt;
        
        // Attempt to use last library it worked for
        DynObjLib *pdol = m_type_libs[st_lib_cache_ix];
        if( pdol && (!lib || !strcmp(pdol->GetLibName(),lib)) ){
            pt = (T*)pdol->Create(type,type_id,pdi_init,obj_size);
            if( pt ) return pt;
            // Fall through, try another lib
        }
        
        MutexLocker lock(m_pmut);
        pt = (T*)Create( lib, type, type_id, pdi_init, obj_size );
        if( pt )
            // Cache the library for this type
            m_type_libs[st_lib_cache_ix] = m_lib_last;
            
        return pt;
    }

    //@}    // Others
    virtual void docall OnObjectDestroy( DynObj *pdo );
    virtual void docall TempDisableCreate( bool disable );

protected:
    
	MutexI	 *m_pmut;		// No point in changing this one during execution
	MallocI  *m_pmalloc;
	TextIoI  *m_ptio;	
	
    DynObjLib *m_lib_last;
    int       m_lib_cache_ix;
	ExpArrP<DynObjLib*>     m_type_libs;
	ExpArrP<DynObjLib*>     m_libs;
    
	ExpArrP<DoModuleC*>     m_modules;
    
    // All registered types, associated with VTable pointers
	HashTable<void**,DynObjType*> m_types;
    
    // Global errors
	ExpArrP<DOError*>    m_errors;
    
    // Per object error handling
	HashTable<const VObj*,DOError*> m_obj_errors;
    ExpArrPOwn<DOError*> m_oe_allocated;
    ExpArrP<DOError*> m_oe_avail;
    
    int m_tmp_disable;
};

/// @cond internal

// A global error, per thread
struct DOError {
	DOError(int thid, int err_code, const char *err_str) : 
		m_thid(thid), m_err_code(err_code), m_err_str(err_str) { }
	bool operator == (int thid) { return m_thid==thid; }
	
	int     m_thid;
	int     m_err_code;
	CharBuf m_err_str;
};


// Solve different ways of handling args in do_new
// Parallel implementation in DynObjLib.h 
template<class T,class A, bool dyni_arg>
struct DoCreator;

template<class T,class A>
struct DoCreator<T,A,false> {
    static T *Create( A arg, const char *lib ){
        // Wrap arg
        DynData dd(arg);
        return DoRunTimeC::GetInstance().Create( doTypeWrapper<T>(), lib, &dd, 0 );
    }
};

template<class T,class A>
struct DoCreator<T,A*,true> {
    static T *Create( A* parg, const char *lib ){
        // Arg is already DynI
        return DoRunTimeC::GetInstance().Create( doTypeWrapper<T>(), lib, parg, 0 );
    }
};

template<class T,class A>
struct DoCreator<T,A&,true> {
    static T *Create( A& arg, const char *lib ){
        // Arg is already DynI
        return DoRunTimeC::GetInstance().Create( doTypeWrapper<T>(), lib, &arg, 0 );
    }
};

/// @endcond internal


/** @ingroup DynTemplate
 * Create a new DynObj based on C++ type.
 * Can take arbitrary type of argument and pass along to constructor of new object 
 * after wrapping it up in a DynData object. 
 * @param arg Constructor argument of arbitrary type 
 * @param lib_name Name of library to use or NULL for default (this is usually 
 * efficient)
 */
template <class T, class A>
T *do_new( A arg, const char *lib_name ){
//T *do_new( A arg, const char *lib_name=DOH_LIB_FIRST ){
    return DoCreator<T,A,DoIsTypeA<A,DynI>::v>::Create( arg, lib_name );
}

/** @ingroup DynTemplate
 * Create a new DynObj based on C++ type.
 * No argument version. New object will receive a NULL pointer in constructor.
 */
template <class T>
T *do_new( ){
    return DoCreator<T,DynI*,true>::Create( (DynI*)NULL, DOH_LIB_FIRST );
}


/*
// Creates a new object using the instance of DoRunTime
template <class T>
T *do_new( const DynI* pdi_init=NULL, const char *lib_name=DOH_LIB_FIRST ){
    // This is internal C++ linking
	return DoRunTimeC::GetInstance().Create( doTypeWrapper<T>(), lib_name, pdi_init, 0 );
}
*/

// %%DYNOBJ section implement
// This section is auto-generated and manual changes will be lost when regenerated!!
#ifdef DO_IMPLEMENT_DORUNTIME

// Generate type information that auto-registers on module load
DynObjType g_do_vtype_DoRunTimeC("DoRunTimeC:DoRunTimeI",DORUNTIMEC_TYPE_ID,1,sizeof(DoRunTimeC));
SideBaseDecl g_do_vtype_sbd_DoRunTimeC_1("DoRunTimeC","NotifierI",sizeof(NotifierI),sidebase_offset<DoRunTimeC,NotifierI>(),NULL,-1,NULL);
SideBaseDecl g_do_vtype_sbd_DoRunTimeC_2("DoRunTimeC","NamedRefI",sizeof(NamedRefI),sidebase_offset<DoRunTimeC,NamedRefI>(),NULL,-1,NULL);
SideBaseDecl g_do_vtype_sbd_DoRunTimeC_3("DoRunTimeC","DortInternalI",sizeof(DortInternalI),sidebase_offset<DoRunTimeC,DortInternalI>(),NULL,-1,NULL);
DynObjType* DoRunTimeC::doGetType( const DynI **pself ) const {
   if(pself) *pself=(const DynI*)(const void*)this;
   return &g_do_vtype_DoRunTimeC;
}
#endif //DO_IMPLEMENT_...
// %%DYNOBJ section end

#endif // DORUNTIME_H


