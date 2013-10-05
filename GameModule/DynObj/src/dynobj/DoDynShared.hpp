// Copyright (c) 2007, Arne Steinarson
// Licensing for DynObj project - see DynObj-license.txt in this folder

#ifndef DO_DYN_SHARED_H
#define DO_DYN_SHARED_H

#if DO_USE_DYNSHARED==1

#include "pi/Atomic.h"

// %%DYNOBJ section general
// This section is auto-generated and manual changes will be lost when regenerated!!
#ifdef DO_IMPLEMENT_DODYNSHARED
    #define DO_IMPLEMENTING     // If app has not defined it already
#endif
#include "dynobj/DynObj.h"

// --- Integer type ids for defined interfaces/classes ---
#define DYNSHAREDC_TYPE_ID  0x8A1AC000

// --- Forward class declarations ---
class DynSharedC;
class RTDynShared;

// --- For each declared class, doTypeInfo template specializations ---
DO_DECL_TYPE_INFO(DynSharedC,DYNSHAREDC_TYPE_ID);
DO_DECL_TYPE_INFO(RTDynShared,RTDYNSHARED_TYPE_ID);

// %%DYNOBJ section end


/** @ingroup DynI 
 * Base implementation of DynSharedI, suitable to derive from.
 * Implements ref counting, leaves doDestroy for derived to implement 
 */
// %% DYNOBJ class(dyni) 
class DynSharedC : public DynSharedI {
public:
    DynSharedC() : m_ref_cnt(1) { }
    
	virtual DynObjType* docall doGetType( const DynI **pself ) const;
    
	// This increases the reference count of the object
	virtual int docall doAddRef( ){
        return AtomicIncrement(&m_ref_cnt);
    }

	// This decreases the reference count of the object
	virtual int docall doRelease( ){
        int cnt = AtomicDecrement(&m_ref_cnt);
        if( !cnt )
            doDestroy( );
        return cnt;
    }
    
    /*
    // This should be implemented by the derived object, to make sure
    // that the right destructor will be used.
    // NOTE: Check that ref count is zero to avoid accidently deleteing
    // object from outside.
    virtual void docall doDestroy( ) { 
        if( m_ref_cnt==0 )
            ::delete this;
        else
            SetError( "doDestroy called on DynSharedI with ref count != 0 ", DOERR_DYNSHARED_INVALID_DODESTROY );
    }
    */
    
protected:
    int m_ref_cnt;
};


/** @ingroup DynI 
 * Embeds another type and makes it ref-counted at compile time.
 * The other type can be a DynI derived or not.
 * 
 * To register:
 *   typedef ToDynShared<MyClass> MySharedClass;
 *   %% _DYNOBJ class(dyni) name(MySharedClass) bases(DynSharedI,MyClass)
 * This will work well with doGetType, also T => ToDynShared => DynSharedI
 */ 
template<class T>
class ToDynShared : public DynSharedI, T {
public:
    ToDynShared( ) : m_ref_cnt(1) { }
    
    virtual int docall doAddRef( ) { 
        return AtomicIncrement(&m_ref_cnt); 
    }
    
    virtual int docall doRelease( ) { 
        int cnt = AtomicDecrement(&m_ref_cnt);
        if( !cnt )
            doDestroy( );
            //::delete this;
        return cnt;
    }
    
    virtual void docall doDestroy( ) { 
        if( !m_ref_cnt )
            ::delete this;
        else
            SetError("doDestroy on non-zero refcounted object", DOERR_DYNSHARED_DESTROY_ON_NON_ZERO_REF );
    }
    
protected:
    int m_ref_cnt;
};


// For this case, we need a specialization of do_cast that can use
// the internal pointer below. We can cast to the owned DynObj 
// type, but from there, we cannot cast back again.
// 
// To cast back from the DynObj to RTDynShared, we would need a 
// mechanism to register custom casts on inidividual objects.

/** @ingroup DynI 
 * Embeds a DynObj instance at run-time and make it ref-counted.
 * Currently casts only work from RTDynShared to derived, not the other 
 * way around.
 */
// %%DYNOBJ class(DynI)  flags(NoTypeId,inst2reg)
class RTDynShared : public DynSharedI {
public:
    RTDynShared( DynObj *pdo=NULL ) : m_pdo(pdo), m_ref_cnt(1) { }
    
	virtual DynObjType* docall doGetType( const DynI **pself ) const;
    
    virtual void* docall doGetObj( const char *type ) {
        if( !type || (type && !strcmp(type,"DynSharedI")) ) return this;
        // Route to owned object
        return m_pdo ? m_pdo->doGetObj(type) : NULL;
    }
    
    virtual void docall doDestroy( ) { 
        if( !m_ref_cnt )
            ::delete this;
        else
            SetError("doDestroy on non-zero refcounted object", DOERR_DYNSHARED_DESTROY_ON_NON_ZERO_REF );
    }
    
    virtual int docall doAddRef( ) { 
        return AtomicIncrement(&m_ref_cnt); 
    }
    
    virtual int docall doRelease( ) { 
        int cnt = AtomicDecrement(&m_ref_cnt);
        if( !cnt ){
            // Destroy first the DynObj and the the holding object
            if(m_pdo) m_pdo->doDestroy( );
            ::delete this;
        }
        return cnt;
    }
    
    template<class U, int M, int N>
    friend struct DoCaster;
    
protected:
    DynObj *m_pdo;
    int     m_ref_cnt;
};


// %%DYNOBJ section implement
// This section is auto-generated and manual changes will be lost when regenerated!!
#ifdef DO_IMPLEMENT_DODYNSHARED

// Generate type information that auto-registers on module load

#if DO_USE_DYNSHARED==1
DynObjType g_do_vtype_DynSharedC("DynSharedC:DynSharedI",DYNSHAREDC_TYPE_ID,1,sizeof(DynSharedC));
DynObjType* DynSharedC::doGetType( const DynI **pself ) const {
   if(pself) *pself=(const DynI*)(const void*)this;
   return &g_do_vtype_DynSharedC;
}
DynObjTypeI2R<RTDynShared,DynSharedI,false> g_do_vtype_RTDynShared("RTDynShared:DynSharedI",RTDYNSHARED_TYPE_ID,1);
DynObjType* RTDynShared::doGetType( const DynI **pself ) const {
   if(pself) *pself=(const DynI*)(const void*)this;
   return &g_do_vtype_RTDynShared;
}
#endif
#endif //DO_IMPLEMENT_...
// %%DYNOBJ section end

#endif // DO_USE_DYNSHARED==1
 
#endif // DO_DYN_SHARED_H

