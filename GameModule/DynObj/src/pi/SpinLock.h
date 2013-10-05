
#ifndef SPINLOCK_H
#define SPINLOCK_H

#include "Atomic.h"

// What is a local stack offset?
#define SL_STACK_LOCALITY 512
#define SL_MAX_WAIT       0x10000000

#define SIUINT unsigned int

// Currently 32-bit implementation


struct SpinLock {
    SpinLock( ) : m_stack(0) /*, m_lock(0), m_cnt(0)*/ { }
    
    // Returns -1 on failure (waited mux number)
    int Enter( );
    int Leave( int old_stack );
    
protected:
    //int    m_lock;
    int     m_stack;
    //int    m_cnt;
};


// Holds the lock and releases it from the dtor
struct SpinLocker {
    SpinLocker( SpinLock& sl ) : m_sl(sl) { m_key = sl.Enter(); }
    ~SpinLocker( ) { if(m_key!=-1) m_sl.Leave(m_key); }
    
    bool FirstEntry( ){ return !m_key; }
    bool IsOk(){ return m_key!=-1; }
    
protected:    
    SpinLock& m_sl;
    int m_key;
};


#endif // SPINLOCK_H

