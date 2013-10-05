
#include "SpinLock.h"

// Can adjust for upwards/downwards stack
#define SL_ON_SAME_STACK(stack,ostack) (stack<ostack && stack>ostack-SL_STACK_LOCALITY)

int SpinLock::Enter( ){_CFE_; 
    int stack = reinterpret_cast<int>(&stack);
    int ostack = AtomicExchange( &m_stack, stack );
    if( !ostack ||  // First entry
        SL_ON_SAME_STACK(stack,ostack) ) // Repeated entry, same thread
        return ostack;      
    
    int cnt = SL_MAX_WAIT;
    while( cnt-->0 ){
        ostack = AtomicExchange( &m_stack, ostack );
        if( !ostack ) return 0;     // First entry
        if( SL_ON_SAME_STACK(stack,ostack) )
            return ostack;      // Repeated entry, same thread
    }
    
    // Failed
    return -1;
}

int SpinLock::Leave( int ostack ){_CFE_; 
    AtomicExchange( (int*)&m_stack, ostack );
    return ostack;
}


#define SL_IS_LOCAL(s,v) ((s>v-SL_STACK_LOCALITY) && (s<v+SL_STACK_LOCALITY))

/*
int SpinLock::Enter( ){_CFE_; 
    SIUINT stack = (int)&stack;
    // Are we the owner ?
    if( SL_IS_LOCAL(m_stack,stack) ){
        SIUINT old_stack = m_stack;
        m_stack = stack;
        //m_cnt++; // Increase thread count
        return (int)old_stack;
    }
    
    // Wait until we read back 0, then we can take ownership
    int cnt = SL_MAX_WAIT;
    SIUINT value;
    while( cnt-->0 ) { 
        value = AtomicExchange( &m_lock, (int)1 );
        if( !value ){
            // Obtained it
            m_stack = stack;
            // assert( ++m_cnt==1 );
            return 0;
        }
    }
    
    // Failed
    return -1;
}

int SpinLock::Leave( int old_stack ){_CFE_; 
    m_stack = old_stack;
    if( !old_stack )
    //if( !--m_cnt )
        // Release any waiting thread
        AtomicExchange( (int*)&m_lock, 0 );
    return old_stack;
}
*/
