
#ifndef NOTIFIABLE_HPP
#define NOTIFIABLE_HPP

// define NOTIF_SILENT to suppress warnings below

#include "NotifiableI.hpp"
#include "TemplUtils.hpp"


// For compatibility with DynObj.h
#ifndef docall
	#ifndef NOTIF_SILENT
		#warning docall not defined, defining to nothing
	#endif
	#define docall
#endif

// If threaded implementation, NOTIF_LOCK locks a global recursive mutex
// so that NotifierI and Notifiable can decouple in an ordered way.
#ifndef  NOTIF_LOCK
    #include "pi/CritSect.h"
    // Use a shared global in Notifiable.cpp
    extern CritSect g_not_cs;
	#define NOTIF_LOCK   g_not_cs.Enter()
	#define NOTIF_UNLOCK g_not_cs.Leave()
#endif 

#ifndef NOTIF_TRACE
	#ifndef NOTIF_SILENT
		#warning NOTIF_TRACE not defined, defining to nothing
	#endif 
	#define NOTIF_TRACE(str)
#endif 


// Reader-write lock for WeakRef
#include "pi/RWLock.h"


class NotifLocker {
public:
	NotifLocker(){ NOTIF_LOCK; }
	~NotifLocker(){ NOTIF_UNLOCK; }
};

// Helper struct, represents one subscriber to a specific notification.
struct NotTarget {
    NotTarget(NotifiableI* pni, int evt) : m_pni(pni), m_evt(evt) { }
    NotTarget(const NotTarget& nt) : m_pni(nt.m_pni), m_evt(nt.m_evt) { }
    NotifiableI *m_pni;
    int m_evt;
    bool operator == (const NotTarget& nt) { return nt.m_pni==m_pni && nt.m_evt==m_evt; }
};

/** @ingroup VObj 
 * Base class to include support for notfications.
 * A class that derives from Notifier send off destructor notifications
 * so we can have weak references to them. For thread safety, it uses a lock 
 * that is shared between all instances of Notifier.
 */
class Notifier : public NotifierI {
public:
	Notifier() : m_fst(NULL) { }
	~Notifier() {
		Notify(NOT_EVT_DESTRUCTOR,NULL);
		NotifLocker lock;
		delete m_fst; 
		m_fst=NULL; 
	}
    /** Adds a notifcation target to this object */
	virtual bool docall AddNotifiable(NotifiableI* pnot, int evt){
        if( !pnot ) return false;
		NOTIF_TRACE("Adding notifiable");
		NotifLocker lock; 
        // Only add once
        if( TLLFind(NotTarget(pnot,evt),m_fst) ) return false;
		m_fst=new TLLNode<NotTarget>(NotTarget(pnot,evt),m_fst); 
        return true;
	}
    /** Remove a notifcation target to this object */
	virtual bool docall RemoveNotifiable(NotifiableI* pnot, int evt){ 
		NOTIF_TRACE("Removing notifiable");
		NotifLocker lock; 
		return TLLErase<NotTarget>(NotTarget(pnot,evt),&m_fst); 
	}
    /** Notifies all notifcation targets registered with this object */
	virtual int docall Notify( int evt, NotDataI *pnd ){
		NotifLocker lock; 
		// We send pointer to ourseleves as first arg
        int cnt=0;
		for( TLLNode<NotTarget> *pnode=m_fst; pnode; pnode=pnode->m_nxt )
            if( pnode->m_t.m_evt==evt || pnode->m_t.m_evt==NOT_EVT_ALL )
                cnt++, pnode->m_t.m_pni->OnNotify( this, evt, pnd );
        return cnt;
	}
	
protected:
	TLLNode<NotTarget> *m_fst;
};

// The point with the below class is that it can convert back to the main class pointer
// However, then we loose the type (NotifierI). The receiver can resolve type
/*
template<class T>
class Notifier : public NotifierBaseImpl {
public:
	~Notifier(){ Notify(NOT_DESTRUCTOR,NULL); } 
	virtual void docall Notify( int msg, NotDataI *pnd ){
		NotifLocker lock; 
 		// NOTE: Conversion to T* will cast back to whole object (if multiple bases)
 		// T and Notifier will be in the same object and on the same SDO side
		T* pt = static_cast<T*>(this);
		for( TLLNode<NotifiableI*> *pnode=m_fst; pnode; pnode=pnode->m_nxt )
			pnode->m_t->OnNotify( pt, msg, pnd );
	}
};
*/


/** @ingroup VObj 
 * Weak reference to another object of type T.
 * The other object must support NotifierI, otherwise we cannot 
 * make a reference. To generate a NotifierI from T, there is 
 * GenericCaster which by default uses static_cast<NotfierI*>(T*).
 * DynObj will specialize this to provide the interface in a 
 * different way (using do_cast).
 * A WeakRef can be used in a thread safe way, using HoldGet() and 
 * HoldPut(). It has a reader-writer lock object per instance, so
 * the reference cannot be overwritten nor destroyed while we do 
 * HoldGet(). */
template<class T>	
class WeakRef : public NotifiableI {
public:
	WeakRef(T* pobj=NULL) : m_pobj(NULL), m_pni(NULL) { Reset(pobj); }
	~WeakRef(){ Reset(NULL); NOTIF_TRACE("WeakRef Dtor"); }

    /** Set the reference to another object */
	WeakRef& Reset( T* pobj ){ 
        piWriteLocker<RWLock> locker( m_lock );
        if( m_pobj==pobj ) return *this;
        
        assert( locker.Locked() );
        if( m_pni || m_pobj ){
            assert( m_pni && m_pobj );
            m_pni->RemoveNotifiable(this,NOT_EVT_DESTRUCTOR);
            m_pni = NULL; m_pobj = NULL;
        }
        if( !pobj ) return *this;
        
        // This will use a static_cast if available and if not, it can be 
        // specialized (with value 1)
		m_pni = GenericCaster<NotifierI*,T*,
                            1-IsConvertible<NotifierI*,T*>::v>::Cast(pobj);
        if( !m_pni ){
			NOTIF_TRACE("WeakRef::Reset - Could not cast to object to NotifierI. Resetting object.");
            m_pobj = NULL;
            return *this;
        }
        m_pobj = pobj;
		m_pni->AddNotifiable(this,NOT_EVT_DESTRUCTOR);
        return *this;
	}
	
    /** Operator support */
	WeakRef&  operator = (T *pobj){ return Reset(pobj); }
    bool operator ==(T *pobj){ return m_pobj==pobj; }

	/** These are non-synced methods. Only use from the same thread as where 
     * the reference owner. */
	operator T*(){ return m_pobj; }
    /** Non-synced */
	T* operator ->(){ return m_pobj; }
    /** Non-synced */
	T* Get(){ return m_pobj; }
    /** Return true if the reference is empty (we have NULL pointer) */
    bool operator!(){ return !m_pobj; }
    
    /** Synchronized access. 
     * This increases read counter (and must be compensated with HoldPut) */
    T* HoldGet(){ 
        if( !m_pobj ) return NULL;
        if( !m_lock.Lock() ) 
            return NULL;
        return m_pobj;
    }
	
    /** Synchronized end-of-access. 
     * This decreases read counter. On zero, the reference can be modified. */
    void HoldPut( T *pt ){ 
        if( !pt ) return;
        assert( pt==m_pobj );
        bool b = m_lock.Unlock();
        assert( b );
    }
    
protected:
	virtual bool docall OnNotify( NotifierI *psrc, int evt, NotDataI *pnd ){
		if( evt==NOT_EVT_DESTRUCTOR ){
            piWriteLocker<RWLock> locker(m_lock);
            if( psrc!=m_pni ){
				NOTIF_TRACE("WeakRef::OnNotify: Unknown source, keeping object"); 
                return false;
            }
            m_pobj = NULL;
            m_pni = NULL;
            return true;
		}
		else{
			NOTIF_TRACE("WeakRef::OnNotify: Unknown event, doing nothing"); 
			return false;
		}
	}
	
	T* m_pobj;          // 16 bytes all in all
    NotifierI *m_pni;
    RWLock m_lock;
};



/*
// The difference compared to WeakRef is that here we don't keep a
// pointer to the compound object, instead we have a pointer to 
// a sub object. We cannot do the 'inside' test, but have to 
// explicitely store a pointer to the NotifierI. 
template<class T>	
class WeakRefSubObject : public WeakRef<T> {
public:
	WeakRefSubObject(T* pobj=NULL) : WeakRef<T>(pobj), m_pni(NULL) { }
	~WeakRefSubObject(){  }
	
	void Reset(T* pobj ){
		NotifLocker lock;	// # Recursive lock!
		WeakRef<T>::Reset( pobj );
		if( this->m_pobj )
			m_pni = Cast<NotifierI*>(this->m_pobj,NULL);
	}
	
	void operator = (T *pobj){ Reset(pobj); }
	
	virtual bool docall OnNotify( NotifierI *psrc, int evt, NotDataI *pnd ){
		NotifLocker lock;
		if( evt==NOT_DESTRUCTOR ){
			if( psrc==m_pni ){
				this->m_pobj = NULL; 
				m_pni=NULL;
				NOTIF_TRACE("WeakRefSubObject::OnNotify: Releasing"); 
				return true;
			}
			else{
				NOTIF_TRACE("WeakRefSubObject::OnNotify: Unknown source, releasing reference anyway"); 
				this->m_pobj = NULL; 
				m_pni=NULL;
				return false;
			}
		}
		else{
			NOTIF_TRACE("WeakRefSubObject::OnNotify: Unknown event, doing nothing"); 
			return false;
		}
	}
	
protected:
	NotifierI *m_pni;
};
*/

#endif // NOTIFIABLE_HPP

