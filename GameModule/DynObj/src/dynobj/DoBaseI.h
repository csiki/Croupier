// Copyright (c) 2007, Arne Steinarson
// Licensing for DynObj project - see DynObj-license.txt in this folder

#ifndef DOBASE_I
#define DOBASE_I

//%%DYNOBJ options(vtabcorr)


// %%DYNOBJ section general
// This section is auto-generated and manual changes will be lost when regenerated!!
#ifdef DO_IMPLEMENT_DOBASEI
    #define DO_IMPLEMENTING     // If app has not defined it already
#endif
#include "dynobj/DynObj.h"

// --- Integer type ids for defined interfaces/classes ---
#define MUTEXI_TYPE_ID      0xD98ED000
#define MALLOCI_TYPE_ID     0x9F46B000
#define TEXTIOI_TYPE_ID     0xB5C14000
#define STATEI_TYPE_ID      0xE0025000
#define PATHI_TYPE_ID       0xBC8E3000
#define NAMEDREFI_TYPE_ID   0xAABF9000

// --- Forward class declarations ---
class NotDataI;
class NotifierI;
class NotifiableI;
class MutexI;
class MallocI;
class TextIoI;
class StateI;
class PathI;
class NamedRefI;
class DoModuleC;
class DoRunTimeI;

// --- For each declared class, doTypeInfo template specializations ---
DO_DECL_TYPE_INFO(NotDataI,NOTDATAI_TYPE_ID);
DO_DECL_TYPE_INFO(NotifierI,NOTIFIERI_TYPE_ID);
DO_DECL_TYPE_INFO(NotifiableI,NOTIFIABLEI_TYPE_ID);
DO_DECL_TYPE_INFO(MutexI,MUTEXI_TYPE_ID);
DO_DECL_TYPE_INFO(MallocI,MALLOCI_TYPE_ID);
DO_DECL_TYPE_INFO(TextIoI,TEXTIOI_TYPE_ID);
DO_DECL_TYPE_INFO(StateI,STATEI_TYPE_ID);
DO_DECL_TYPE_INFO(PathI,PATHI_TYPE_ID);
DO_DECL_TYPE_INFO(NamedRefI,NAMEDREFI_TYPE_ID);
DO_DECL_TYPE_INFO(DoModuleC,DOMODULEC_TYPE_ID);
DO_DECL_TYPE_INFO(DoRunTimeI,DORUNTIMEI_TYPE_ID);

// %%DYNOBJ section end


// These are practical to have here, even if we don't own them
// If needed we know they should be implemented somewhere
// Import NotifierI and NotifiableI classes
#include "utils/NotifiableI.hpp"
// %%DYNOBJ class(vobj) name(NotDataI) bases(VObj)  flags(notypeid)
// %%DYNOBJ class(vobj) name(NotifierI) bases(VObj)  flags(notypeid)
// %%DYNOBJ class(vobj) name(NotifiableI) bases(VObj)  flags(notypeid)


/** @ingroup VObj
 * Interface to a mutex object
 * Assumption: a recursive mutex since it can be created on most platforms 
 * @see MutexLocker
 */
// %%DYNOBJ class(vobj) 
class MutexI {
public:
    /** @name Virtuals
     *  Interface members */
    //@{ 
	virtual bool docall Lock() = 0;
	virtual bool docall Unlock() = 0;
	virtual bool docall TryLock() { return false; }
    //@}
};

// Utility class
/** @ingroup Utils
 * Utility class to hold a mutex in a local variable.
 */
class MutexLocker {
public:
	MutexLocker(MutexI *pmi) : m_pmi(pmi) { if(m_pmi) m_pmi->Lock(); }
	~MutexLocker() { if(m_pmi) m_pmi->Unlock(); }
protected:
	MutexI *m_pmi;
};


/** @ingroup VObj
 * Interface to a memory allocator
 */
// %%DYNOBJ class(vobj) 
class MallocI {
public:
    /** @name Virtuals
     *  Interface members */
    //@{ 
	virtual void* docall malloc( int size ) = 0;
	virtual void  docall free( void *pv ) = 0;
    //@}
};

class DynStr;

/** @ingroup VObj
 * Interface to a memory allocator
 */
// %%DYNOBJ class(vobj) 
class TextIoI {
public:
    /** @name Virtuals
     *  Interface members */
    //@{ 
	virtual int docall puts(const char *ps) = 0;
	virtual int docall puterr(const char *pe) = 0;
	virtual int docall gets(DynStr& ds, bool sync=true) = 0;
    //@}
};


/** @ingroup VObj
 * Interface for object with a state that can be stored as text
 */
// %%DYNOBJ class(vobj) 
class StateI {
public:
    /** @name Virtuals
     *  Interface members */
    //@{ 
	/** Fill in a current state (often XML) */
	virtual bool docall GetState( DynStr &ds ) = 0;
	/** Set state from previous GetState */
	virtual bool docall SetState( const char *state ) = 0;
	/** Called when all items/objects have received their state */
	virtual void docall LateInit( ) = 0;
    //@}
};

/** @ingroup VObj
 * Interface for object with that uses a 'current path'
 */
// %%DYNOBJ class(vobj) 
class PathI {
public:
    /** @name Virtuals
     *  Interface members */
    //@{ 
	virtual bool docall GetPath( DynStr &ds ) = 0;
	virtual bool docall SetPath( const char *path ) = 0;
    //@}
};


/** @ingroup DynI
 * This interface allows for retrieving named 'related objects' to an object. 
 * A Tree browser may have a "contents" related object ,...etc. 
 * The sibling object is usually an object with its own life span.
 * If the object implementing NamedRefI also implements NotifiableI
 * there is no need for the Remove function, since we get destructor 
 * notifications.*/
// %%DYNOBJ class(dyni) flags(inst2reg)
class NamedRefI : public DynI {
public:
    /** @name Virtuals
     *  Interface members */
    //@{ 
	virtual DynObjType* docall doGetType( const DynI **pself=0 ) const;
	
    /** Lookup a named object.
      * @param name is the name which the object is known by
      * @param type_id is the type of the object or 0 if don't care
      * @return An interface pointer or NULL if not found */
	virtual DynI* docall GetByName( const char *name, int type_id ){ return NULL; }
    
    /** Lookup a named object using index.
      * @param index lookup the n:th object of this type
      * @param ds store name of object here
      * @param type_id is the type of the object or 0 if don't care
      * @return An interface pointer or NULL if not found */
	virtual DynI* docall GetByIndex( int index, DynStr &ds, int type_id=0 ){ return NULL; }
    
    /** Store a named object.
      * An implementation is free to return false if the object is not 
      * accepted (i.e. it does not implement NotifierI)
      * @param name is the name by which the object can later be looked up 
      * @param pdi the object to be known with this name
      * @return true on success */
	virtual bool  docall Set( const char *name, DynI *pdi ){ return false; }
    
    /** Manually remove a named object.
      * @param pdi the object to be removed
      * @return true on success */
	virtual bool  docall Remove( DynI *pdi ){ return false; }
    //@}

    /** @name Inlines
     *  Inline convenience functions */
    // Get without a type ID 
	DynI* Get( const char *name ){ return GetByName(name,0); }
    //DynI *GetByName( const char *name, int type_id=0 ){ return Get(name,type_id); }
    //DynI *GetByIndex( int index, DynStr &ds, int type_id=0 ){ return Get(index,ds,type_id); }
};


/** @ingroup DynI
 * A DynObj module (internal use). Represents one currently loaded library */
// %%DYNOBJ class(Dyni) flags(NoTypeId)
class DoModuleC : public DynI {
public:
    DoModuleC() : m_pv1(0), m_pv2(0) { }
	virtual DynObjType* docall doGetType( const DynI **pself=0 ) const;
	
    /** @name Virtuals
     *  Interface members */
    //@{ 
	virtual DynObjType* docall FindType( void** vtbl ){ return NULL; }
	virtual DynObjType* docall FindType( const char *type ){ return NULL; }
	virtual DynObjType* docall FindType( int type_id ){ return NULL; }
    //@}
    void *m_pv1, *m_pv2;
};


#ifdef DO_USE_RUNTIME

/** @ingroup DynI
 * Global DoRunTime class. Gives access to shared resources, a named object
 * pool and types in other loaded libraries */
// %%DYNOBJ class(dyni) flags(notypeid) 
class DoRunTimeI : public DynI {
public:
	// Base DynObj functions
	virtual DynObjType* docall doGetType( const DynI **pself=0 ) const;
	
    /** @name Virtuals
     *  Interface members */
    //@{ 
	// Access shared functionality
	virtual MutexI*  docall  GetMutex() const { return NULL; }
	virtual MallocI* docall  GetMalloc() const{ return NULL; }
	virtual TextIoI* docall  GetTextIo() const{ return NULL; }
	
	// Object creation, set lib to NULL to create with first library that knows the type.
	virtual DynObj*  docall  Create( const char *lib, const char *type, int type_id, const DynI* pdi_init=NULL, int obj_size=0 ){ return NULL; }
	
	// A collection of named global objects handled here
    // This is delegated to NamedRefI

	// Set/Get last error (per thread, but that is transparent to caller)
	virtual int  docall GetLastErrCode( int thid=-1 ){ return -1; }
	virtual bool docall GetLastErrStr( DynStr& ds, int thid=-1 ){ return false; }
	virtual bool docall SetLastError( int err_code, const char *err_msg ){ return false; }
	virtual int  docall GetThreadId( ){ return -1; }

	// DynObj module registration 	
	virtual bool docall RegisterModule( DoModuleC *pmi ){ return false; }
	virtual bool docall UnRegisterModule( DoModuleC *pmi ){ return false; }

	// Manage types based on VTable pointer
	virtual bool        docall RegisterType( void **vtbl, DynObjType *pdt ){ return false; }
	virtual bool        docall UnRegisterType( void **vtbl ){ return false; }
	virtual DynObjType* docall GetTypeOf( void **vtbl ){ return NULL; }

    // Handle per object errors
    virtual bool docall SetObjectError( const VObj *pvo, const char *err_str, int err_code ){ return false; }
    virtual bool docall ClearObjectError( const VObj *pvo ){ return false; }
    virtual const char* docall GetObjectError( const VObj *pvo, int *perr_code ){ return NULL; }
    //@}

    /** @name Inlines
     *  Inline convenience functions */
    //@{ 
    
	// Assuming that DoRunTimeI implements NamedRefI (which is very convenient)
	bool  ObjectPublish( DynI *pdi, const char *name ){ 
		NamedRefI *pni = (NamedRefI*)doGetObj("NamedRefI");
		if( !pni ) return false;
		return pni->Set(name,pdi);
	}
	DynI* ObjectLookup( const char *name, int type_id=0 ){ 
		NamedRefI *pni = (NamedRefI*)doGetObj("NamedRefI");
		if( !pni ) return NULL;
		return pni->GetByName(name,type_id);
	}
    //@}
};

#endif // DO_USE_RUNTIME 

// %%DYNOBJ section implement
// This section is auto-generated and manual changes will be lost when regenerated!!
#ifdef DO_IMPLEMENT_DOBASEI

// Generate type information that auto-registers on module load
DynObjType g_do_vtype_NotDataI("NotDataI:VObj",NOTDATAI_TYPE_ID,1,sizeof(NotDataI));
DynObjType g_do_vtype_NotifierI("NotifierI:VObj",NOTIFIERI_TYPE_ID,1,sizeof(NotifierI));
DynObjType g_do_vtype_NotifiableI("NotifiableI:VObj",NOTIFIABLEI_TYPE_ID,1,sizeof(NotifiableI));
DynObjType g_do_vtype_MutexI("MutexI:VObj",MUTEXI_TYPE_ID,1,sizeof(MutexI));
DynObjType g_do_vtype_MallocI("MallocI:VObj",MALLOCI_TYPE_ID,1,sizeof(MallocI));
DynObjType g_do_vtype_TextIoI("TextIoI:VObj",TEXTIOI_TYPE_ID,1,sizeof(TextIoI));
DynObjType g_do_vtype_StateI("StateI:VObj",STATEI_TYPE_ID,1,sizeof(StateI));
DynObjType g_do_vtype_PathI("PathI:VObj",PATHI_TYPE_ID,1,sizeof(PathI));
DynObjTypeI2R<NamedRefI,DynI,false> g_do_vtype_NamedRefI("NamedRefI:DynI",NAMEDREFI_TYPE_ID,1);
DynObjType* NamedRefI::doGetType( const DynI **pself ) const {
   if(pself) *pself=(const DynI*)(const void*)this;
   return &g_do_vtype_NamedRefI;
}
DynObjType g_do_vtype_DoModuleC("DoModuleC:DynI",DOMODULEC_TYPE_ID,1,sizeof(DoModuleC));
DynObjType* DoModuleC::doGetType( const DynI **pself ) const {
   if(pself) *pself=(const DynI*)(const void*)this;
   return &g_do_vtype_DoModuleC;
}

#ifdef DO_USE_RUNTIME
DynObjType g_do_vtype_DoRunTimeI("DoRunTimeI:DynI",DORUNTIMEI_TYPE_ID,1,sizeof(DoRunTimeI));
DynObjType* DoRunTimeI::doGetType( const DynI **pself ) const {
   if(pself) *pself=(const DynI*)(const void*)this;
   return &g_do_vtype_DoRunTimeI;
}
#endif
#endif //DO_IMPLEMENT_...
// %%DYNOBJ section end

#endif //DOBASE_I


