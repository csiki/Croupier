
#include "pi/CritSect.h"
#include "pi/Atomic.h"
#include "pi/Thread.h"

#include <assert.h>

CritSect::CritSect( bool do_construct ){_CFE_;
	if( do_construct ){
		m_cnt = 0;
		m_thid = 0;
	}
}

void CritSect::Enter( ){_CFE_;
    int cnt = AtomicIncrement( &m_cnt );
	if( m_thid==(int)piGetThreadId() ){
        if( cnt>1 )   
            return; // Repeated entry
        assert( !m_thid );
	}
	
    while( cnt>1 ){
        if( !AtomicDecrement(&m_cnt) )
        	// If final release for other thread happened when we had temp increase
        	m_thid = 0;	
        piYield();
        cnt = AtomicIncrement(&m_cnt);
    }
    
    // Have it
    m_thid = piGetThreadId();
}

void CritSect::Leave( ){_CFE_;
    int cnt = AtomicDecrement( &m_cnt );
    if( !cnt )
        // Thread leaving it
        m_thid = 0;
    else
        // Check not released too many times
        assert( cnt>0 );
}

bool CritSect::TryEnter( ){_CFE_;
    int cnt = AtomicIncrement( &m_cnt );
	if( m_thid==(int)piGetThreadId() ){
        if( cnt>1 )   
            return true; // Repeated entry
        assert( !m_thid );
	}
	
    // Have it
    if( cnt==1 ){
        m_thid = piGetThreadId();
        return true;
    }
    else{
        // Failed
        AtomicDecrement( &m_cnt );
        return false;
    }
}

