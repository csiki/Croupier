
#ifndef RWLOCK_H
#define RWLOCK_H

// What is a local stack offset?
#define SL_MAX_WAIT       0x10000000

#ifndef RWLOCK_YIELD
    #define RWLOCK_YIELD ;/*Spin*/
#endif

struct RWLock {
    RWLock( ) : m_lock(0), m_cnt(0) { }
    
    // Returns -1 on failure (waited max number)
    
    // Reader lock
    bool Lock( bool try_once=false );
    bool Unlock( );
    
    bool WriteLock( bool try_once=false );
    bool WriteUnlock( );
    
protected:
    int    m_lock;
    int    m_cnt;
};


// Test for success when locking
template<class T>
struct piWriteLocker {
    piWriteLocker( T& t ) : m_t(t) { m_ok=m_t.WriteLock(); }
    ~piWriteLocker( ) { if(m_ok) m_t.WriteUnlock(); }
    bool Locked(){ return m_ok; }
protected:    
    T&   m_t;
    bool m_ok;
};

#endif // SPINLOCK_H

