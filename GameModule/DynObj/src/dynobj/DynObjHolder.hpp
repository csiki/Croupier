// Copyright (c) 2007, Arne Steinarson
// Licensing for DynObj project - see DynObj-license.txt in this folder


#ifndef DYNOBJ_HOLDER_HPP
#define DYNOBJ_HOLDER_HPP

///////////////////////////////////////////////////////////////////////
// DynObjHolder - temporary hold DynObj and release in destructor
//

#include "DoBase.hpp"
#ifndef DOH_SKIP_NEW_ABILITY
    #include "DoNew.h"
#endif

// Deduction
#if DO_USE_RUNTIME==1 && defined(DO_MAIN)
    // DoRunTimeC uses Notifier
    #ifndef DO_USE_NOTIFIER
        #define DO_USE_NOTIFIER 1
    #endif
#endif

#if DO_USE_NOTIFIER==1
    #include "DoNotifiable.hpp"
#endif

/** @ingroup DynTemplate
 * Different modes to setup and hold an interface/class pointer. 
 * - hmOwn: Owns the object and releases it with doDestroy
 * - hmShared: Has reference to a ref-counted object [release with doRelease()]
 * - hmStatic: Held object is static/global and 'immortal'
 * - hmWeakRef: Hold a weak reference to the object
 * - hmAutoOwn: Setup directive: Try hmShared first and then hmOwn
 * - hmAutoNoOwn: Setup directive: Try hmStatic first and then hmWeakRef
 * - hmAuto: Try in this order: hmStatic, hmWeakRef, hmShared, hmOwn */
enum HoldMode { hmOwn=1, hmShared, hmStatic, hmWeakRef, hmAutoOwn, hmAutoNoOwn, hmAuto };
    

 /** @ingroup DynTemplate
 * Holds a DynI (or derived type) and releases/destroyes it when holder 
 * goes out of scope. It is practical for handling local DynObj:s. By default 
 * it takes over a DynObj in the constructor. Alternatively it can create a 
 * new object from a library. 
 * @param T The type of object to hold */
template <class T>
class DynObjHolder {
public:
#if DO_USE_RUNTIME==1 && !defined(DOH_SKIP_NEW_ABILITY)
    /** No argument constructor. Will attempt to create a DynObj using DoRunTimeI
      * and hold on to that until the holder goes out of scope. Then the DynObj is 
      properly released/destroyed.      */
    DynObjHolder( ) : m_pdo(0) { 
        m_pdo = do_new<T>( );
        m_mode = hmAutoOwn;    // We created it so we own it
    }
    
    /** Two argument constructor.
      * A new DynObj is created from specificed library with given argument to
      * new DynObj constructor. The object is held by this class (the holder) until 
      * we go out of scope, when the DynObj is properly released/destroyed.
      * @param lib_name Name of the library to use (set to NULL for automatic 
      * library search)
      * @param arg An argument to pass to the new DynObj. Can be arbitrary type. */
    template<class A>
    DynObjHolder( const char *lib_name, A arg ) { 
        m_pdo = do_new<T>( arg, lib_name );
        m_mode = hmAutoOwn;    // We created it so we own it
    }
#endif // DO_USE_RUNTIME==1 && !DOH_SKIP_NEW_ABILITY

    /** Different two argument constructor. 
      * @param pdo is a pointer to an existing DynObj to take over.
      * @param mode determines how the object is released */
    DynObjHolder( T *pdo, HoldMode mode ) : m_pdo(NULL), m_mode(mode) {
        Assign( pdo, mode );
    }
    
    /** Assign a new DynObj to holder.
      * For a given mode, it will check that the object being held supports
      * the requested way of holding. If it fails, an error is logged gloablly.
      * @param pdo is a pointer to an existing DynObj to take over.
      * @param mode determines how the object is release. */
    DynObjHolder& Assign( T *pdo, HoldMode mode ){
    	// Detect case where we don't have to re-assign
    	if( pdo && (m_pdo==pdo 
#if DO_USE_NOTIFIER==1
    	    || (m_mode==hmWeakRef && m_pwr && m_pwr->Get()==pdo) 
#endif            
    	    ))
    		return *this;
    		
        Release( );
        if( !pdo ){
        	// Work with old mode
            if( m_mode!=hmWeakRef ) m_pdo = NULL;
#if DO_USE_NOTIFIER==1
            else if(m_pwr) m_pwr->Reset(NULL);
#endif            
            // Work with new mode
            if( m_mode==hmWeakRef && mode!=hmWeakRef ){
#if DO_USE_NOTIFIER==1
            	delete m_pwr;
            	m_pwr = NULL;
#endif
            }
            m_mode = mode;	// Set the mode, it's a hint for the future
            return *this;
        }

        // With auto-mode, sort it either to AutoOwn or AutoNoOwn
        if( mode==hmAuto ){
        	if( doIsStatic(pdo) )
        		mode = hmStatic;
        	else if( pdo->CanBeA(NOTIFIERI_TYPE_ID) )
        		mode = hmWeakRef;
        	else 
        		mode = hmAutoOwn;
        }
        
        HoldMode old_mode = m_mode;
#if DO_USE_NOTIFIER==1
        // Store in case of delete / reuse
        WeakRef<T> *old_pwr = old_mode==hmWeakRef ? m_pwr : NULL; 
#endif
        // Assume this will work for now
        m_mode = mode;
        m_pdo = pdo;
        if( mode==hmOwn || mode==hmAutoOwn || mode==hmShared ){
            // Need DynObj derived type
            if( !pdo->CanBeA(DYNOBJ_TYPE_ID) ){
                DO_LOG_ERR1( DOERR_CLASS_DYNOBJHOLDER, "DynObjHolder<T>, Request to store owning reference, but no DynObj interface available", pdo );
                goto Assign_Return_Failed;
            }
            if( mode==hmShared || (mode==hmAutoOwn && pdo->CanBeA(DYNSHAREDI_TYPE_ID)) ){
                // Use shared ownership, add ref
                m_mode = hmShared;
                DynSharedI *psi = do_cast<DynSharedI*>(pdo);
                if( psi )
                    psi->doAddRef();
                else{
                    DO_LOG_ERR1( DOERR_CLASS_DYNOBJHOLDER, "DynObjHolder<T>, Request to store hold as shared object, no DynSharedI available", pdo );
                    goto Assign_Return_Failed;
                }
            }
            else{
                // Single-point ownership
                m_mode = hmOwn;
                if( pdo->CanBeA(DYNSHAREDI_TYPE_ID) || doIsStatic(pdo) ){
                    DO_LOG_ERR1( DOERR_CLASS_DYNOBJHOLDER, "DynObjHolder<T>, Request for single point ownership, but object is shared (or static), no DynSharedI available", pdo );
                    goto Assign_Return_Failed;
                }
            }
        }
        
        else if( mode==hmStatic ){
            if( !doIsStatic(pdo) ){
                DO_LOG_ERR1( DOERR_CLASS_DYNOBJHOLDER, "DynObjHolder<T>, Request for static holding, but object type is not static", pdo );
                goto Assign_Return_Failed;
            }
        }

        else if( mode==hmWeakRef || mode==hmAutoNoOwn ){
            if( mode==hmAutoNoOwn && doIsStatic(pdo) ){
                m_mode = hmStatic;
            }
            else {
#if DO_USE_NOTIFIER==1
                // Must make WeakRef
                m_mode = hmWeakRef;
                if( !pdo->CanBeA(NOTIFIERI_TYPE_ID) ){
                    DO_LOG_ERR1( DOERR_CLASS_DYNOBJHOLDER, "DynObjHolder<T>, hmWeakRef - Object does not support NotifierI", pdo );
                    goto Assign_Return_Failed;
                }
                // Assign new Weak ref if NULL or or old pointer is not WeakRef
                if( old_pwr ){
                    m_pwr = old_pwr;
                    m_pwr->Reset(pdo);
                }
                else
                    m_pwr = new WeakRef<T>(pdo);
#else
                DO_LOG_ERR1( DOERR_CLASS_DYNOBJHOLDER, "DynObjHolder<T>, hmWeakRef - Weak ref not supported in build", pdo );
                goto Assign_Return_Failed;
#endif        
            }
        }
        else{
            DO_LOG_ERR1( DOERR_CLASS_DYNOBJHOLDER, "DynObjHolder<T>, Unknown holding mode", mode );
            goto Assign_Return_Failed;
        }

        // Success
        return *this;
        
Assign_Return_Failed:
#if DO_USE_NOTIFIER==1
        if( m_mode!=hmWeakRef ){
            m_pdo = NULL;
            delete old_pwr;
        }
        else{ 
            m_pwr = old_pwr;
            if(m_pwr) m_pwr->Reset(NULL);
        }
#else
        m_pdo = NULL;
#endif
        return *this;
    }

    ~DynObjHolder(){ 
        Release( );
#if DO_USE_NOTIFIER==1
        if( m_mode==hmWeakRef )
            delete m_pwr;
#endif
    }
     
    /** Get a pointer to the held object. */
    T *Get() const { 
        if( m_mode!=hmWeakRef )
            return m_pdo; 
        else{
#if DO_USE_NOTIFIER==1
            return m_pwr ? m_pwr->Get() : NULL;
#else
            assert(0);   // Not supported 
            return NULL;
#endif        
        }
     }
    
    /** Get a reference to the held object. */
	 T &operator()() const { return *Get(); }
     
    /** Get a pointer to the held object. */
	 T *operator->() const { return Get(); }
	 
    /** Operator! (test if NULL object held) */
	 bool operator !() const { return !IsOk(); }
		 
    /** Assign a new DynObj to this holder. */
     DynObjHolder& operator = (T *pdo){
        return Assign( pdo, m_mode );
     }
     
    /** Return true if we are holding an object. */
	 bool IsOk() const { 
        if( m_mode!=hmWeakRef )
            return m_pdo!=NULL; 
        else{
#if DO_USE_NOTIFIER==1
            return m_pwr && m_pwr->Get()!=NULL;
#else
            return false;
#endif        
        }
    }

    /** Release currently held object */
    void Release() { 
        switch(m_mode){
            case hmOwn:
            case hmShared:
            case hmAutoOwn: doReleaseDestroy( (DynObj*)m_pdo ); m_pdo=NULL; break;
            case hmStatic: m_pdo=NULL; break; 
            case hmWeakRef:
#if DO_USE_NOTIFIER==1
                m_pwr->Reset( NULL );
#endif        
                break;
        }
    }
    
protected:
    union {
        T *m_pdo;
#if DO_USE_NOTIFIER==1
        WeakRef<T> *m_pwr;
#endif        
    };
    HoldMode m_mode;
};

#define DOH DynObjHolder // Convenience macro

#ifdef TYPEOF_H
    #undef TYPE_REG_FILE
    #define TYPE_REG_FILE 500
    TYPE_REG_T1(DynObjHolder)
#endif

#endif // DYNOBJ_HOLDER_HPP

