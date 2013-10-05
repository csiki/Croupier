
#ifndef CRITSECT_H
#define CRITSECT_H

// A fast critical section / mutex class
// It will work under Gcc using AtomicAdd or under
// Windows using InterlockedIncrement. For other platforms
// a replacement for this atomic increment must be found.
class CritSect {
public:
	// If using the crit sect statically or globally, 
	// set do_construct to false, then it can be used
	// correctly without init (it works well from the
	// zero state)
	CritSect( bool do_construct=true );

	void Enter( );
	void Leave( );
	bool TryEnter( );

protected:
	int m_cnt;	// Allocated count.
	int m_thid;	// Thread ID of owner
};


class CritSectLocker {
public:
	CritSectLocker( CritSect &cs ) : m_cs(cs) { m_cs.Enter(); }
	~CritSectLocker( ) { m_cs.Leave(); }

	CritSect &m_cs;
};

// Should have this name really
typedef CritSect piCritSect;

// For backwards compatibility
#ifdef WXWIDGETS
typedef CritSect wxCritSect;
typedef CritSectLocker wxCritSectLocker;
#endif

#endif // CRITSECT_H

