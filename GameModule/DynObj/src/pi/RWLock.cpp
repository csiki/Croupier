
#include "pi/RWLock.h"

#include "pi/Atomic.h"
//#include "pi/Thread.h"

#define RWLOCK  AtomicExchange(&m_lock,1)
#define RWUNLOCK  AtomicExchange(&m_lock,0)

bool RWLock::WriteLock( bool try_once ){_CFE_;
    int cnt = SL_MAX_WAIT;
    while( --cnt>0 ){
        if( !RWLOCK ){
            if( !m_cnt ) m_cnt--;
            RWUNLOCK;
            if( m_cnt<0 || try_once )
                return m_cnt<0;
        }
        RWLOCK_YIELD;
    }
    return false;
}

bool RWLock::WriteUnlock( ){_CFE_;
    int cnt = SL_MAX_WAIT;
    while( --cnt>0 ){
        if( !RWLOCK ){
            bool res = false;
            if( m_cnt==-1 ) m_cnt++,res=true;
            RWUNLOCK;
            return res;
        }
        RWLOCK_YIELD;
    }
    return false;
}


bool RWLock::Lock( bool try_once ){_CFE_;
    int cnt = SL_MAX_WAIT;
    while( --cnt>0 ){
        if( !RWLOCK ){
            if( m_cnt>=0 ) m_cnt++;
            RWUNLOCK;
            if( m_cnt>0 || try_once )
                return m_cnt>0;
        }
        RWLOCK_YIELD;
    }
    return false;
}

bool RWLock::Unlock( ){_CFE_;
    int cnt = SL_MAX_WAIT;
    while( --cnt>0 ){
        if( !RWLOCK ){
            bool res = false;
            if( m_cnt>0 ) m_cnt--,res=true;
            RWUNLOCK;
            return res;
        }
        RWLOCK_YIELD;
    }
    return false;
}
