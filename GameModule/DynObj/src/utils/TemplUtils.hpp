
#ifndef TEMPLUTILS_HPP
#define TEMPLUTILS_HPP

/////////////////

// Increases its counter in constructor
// and decreases it in destructor
template <class T>
class CounterLock{
public:    
    CounterLock( T &cntr ) : m_cntr(cntr) { m_cntr++; }
    ~CounterLock( ) { m_cntr--; }
    T &m_cntr;
};

/////////////////

// Not a template, but small utility
class BoolHolder{
public:    
    BoolHolder( bool &b, bool set_on_init=true ) : m_b(b), m_set_on_init(set_on_init) { m_b=set_on_init; }
    ~BoolHolder( ) { m_b = !m_set_on_init; }
    bool &m_b;
    bool m_set_on_init;
};

/////////////////
// Uses 'Lock/Unlock to hold/release
template<class T, bool test=false>
struct LockLocker;

template<class T>
struct LockLocker<T,false> {
    LockLocker( T& t ) : m_t(t) { m_t.Lock(); }
    ~LockLocker( ) { m_t.Unlock(); }
protected:    
    T &m_t;
};

// Test for success when locking
template<class T>
struct LockLocker<T,true> {
    LockLocker( T& t ) : m_t(t) { m_ok=m_t.Lock(); }
    ~LockLocker( ) { if(m_ok) m_t.Unlock(); }
    bool Locked(){ return m_ok; }
protected:    
    T &m_t;
    bool m_ok;
};


/////////////////
// Uses 'Enter/Leave to hold/release
template<class T, bool test=false>
struct EnterLocker;

template<class T>
struct EnterLocker<T,false> {
    EnterLocker( T& t ) : m_t(t) { m_t.Enter(); }
    ~EnterLocker( ) { m_t.Leave(); }
protected:    
    T &m_t;
};

// Test for success when locking
template<class T>
struct EnterLocker<T,true> {
    EnterLocker( T& t ) : m_t(t) { m_ok=m_t.Enter(); }
    ~EnterLocker( ) { if(m_ok) m_t.Leave(); }
    bool Locked(){ return m_ok; }
protected:    
    T &m_t;
    bool m_ok;
};


/////////////////
// Use 'AddRef/Release to hold/release
template<class T>
struct RefLocker {
    RefLocker( T& t ) : m_t(t) { m_t.AddRef(); }
    ~RefLocker( ) { m_t.Release(); }
    
protected:    
    T &m_t;
};

/////////////////

template <class U, class V>
class TPair{
public:
	TPair( U &u, V &v ) : m_u(u), m_v(v) { }
	TPair( ) { }
	U m_u;
	V m_v;
	// This does not work from inside EapFind !! 
	bool operator == (const U &u){ return m_u==u; }
	bool operator == (const V &v){ return m_v==v; }
};


/////////////////

// Template linked list (TLL)
template <class T>
class TLLNode {
public:
	TLLNode( const T& t, TLLNode *nxt ) : m_t(t), m_nxt(nxt) { }
	TLLNode( TLLNode *nxt ) : m_nxt(nxt) { }
	~TLLNode(){ delete m_nxt; }
	T 	m_t;			// User object
	TLLNode *m_nxt;
};

// Remove a node and delete it (but not the data)
template <class T>
bool TLLErase(const T& t, TLLNode<T> **ppnode){
	while( *ppnode && !((*ppnode)->m_t==t) )
		ppnode = &((*ppnode)->m_nxt);
	if( !*ppnode ) return false;
	TLLNode<T> *pnode = *ppnode;
	*ppnode = pnode->m_nxt;
	pnode->m_nxt = 0;
	delete pnode;
	return true;
}


// Search for an item, remove from linked list if found, return TLLNode
// U allows for searching for a subitem of T, but U can also be T
template <class T, class U>
TLLNode<T> *TLLRemove(U u, TLLNode<T> **ppnode){
	while( *ppnode && !((*ppnode)->m_t==u) )
		ppnode = &((*ppnode)->m_nxt);
	if( !*ppnode ) return 0;
	TLLNode<T> *pnode = *ppnode;
	*ppnode = pnode->m_nxt;
	pnode->m_nxt = 0;
	return pnode;
}

// Search for an item, and return it
template <class T, class U>
TLLNode<T> *TLLFind(U u, TLLNode<T> *pnode){
	while( pnode && !(pnode->m_t==u) )
		pnode = pnode->m_nxt;
    return pnode;
}

// Search for an item, and return it
template <class T, class U>
TLLNode<T> **TLLFind(U u, TLLNode<T> **ppnode){
	while( *ppnode && !((*ppnode)->m_t==u) )
		ppnode = &(*ppnode)->m_nxt;
    return ppnode;
}

///////////////////////////////////////////////////
// Detect if a class is convertible to another
template <class U, class V>
struct IsConvertible {
	// Test functions
	//static char F( const V& );
	static char F( V );
	static short F(...);
	static U u;
	enum { v=sizeof(F(u))==sizeof(char) };
};

////////////////////////////////////////////
// Ordinary test for same type 
template <class U, class V>
struct IsSame {
    enum { v=false };
};

template <class U>
struct IsSame<U,U> {
    enum { v=true };
};


///////////////////////////////////////////////
// Tests if a class has a VTable or not
template<class T>
struct HasVTable {
    struct IA : public T {
        int m;
    };
    struct IB : public T {
        virtual void F( );
        int m;
    };
    enum { v=sizeof(IA)==sizeof(IB) }; 
};

template<class T>
struct HasVTable<T*> {
    enum { v=HasVTable<T>::v };
};

template<>
struct HasVTable<void> {
    enum { v=false };
};


/////////////////
// If n==0 it turns into a static cast 
template <class T, class U, int n>
struct GenericCaster;

template <class T, class U>
struct GenericCaster<T,U,0> {
    static T Cast( U u ) {	return static_cast<T>(u); }
};

/////////////////
// Return true if pv points to a location inside of T at pt
template <class T>
bool IsInside( T *pt, void *pv ){
	return pv>=pt && pv<(pt+1);
}


/////////////////
// Very simple singleton implementation
template <class T>
class Singleton {
public:
	// Will only instantiate when requested
	// Will call T::~T at unload/exit
	static T &GetInstance( ){
		static T t;
		return t;
	}
};

// This will count pointer depth: int:0, int*:1, int**:2, ...
template<class T>
class PointerDepth {
public:
	static int Get(){ return 0; }
};

template<class T>
class PointerDepth<T*> {
public:
	static int Get(){ return 1+PointerDepth<T>::Get(); }
};

// Detect if type is pointer or not
template<class T>
struct IsPointer { enum { v=false }; };

template<class T>
struct IsPointer<T*> { enum { v=true }; };


// Delete object if says so in ctor, otherwise just hold it
template<class T>
struct TempOwner {
    TempOwner( T* pt, bool own ) : m_pt(pt), m_own(own) { }
    ~TempOwner(){ Release(); }
    T *operator->(){ return m_pt; }
    T& operator()(){ return *m_pt; }
    bool operator!(){ return !m_pt; }
    
    TempOwner& Assign( T *pt, bool own ){ 
        Release();
        m_pt = pt;
        m_own = own;
        return *this;
    }
    
protected:
    void Release(){ 
        if(m_own && m_pt) delete m_pt;
        m_pt = NULL;
        m_own = false;
    }
    T *m_pt;
    bool m_own;
};


// Execute a member function if pointer is non-NULL
template<class C>
void Try( C *pc, void (C::*func)() ){
    if( pc ) (pc->*func)();
}

template<class C, class RV>
RV Try( C *pc, RV (C::*func)(), RV rv=0 ){
    if( !pc ) return rv;
    return (pc->*func)();
}


template<class C,class A1>
void Try( C *pc, void (C::*func)(A1), A1 a1 ){
    if( pc ) (pc->*func)(a1);
}

template<class C, class RV, class A1>
RV Try( C *pc, RV (C::*func)(A1), A1 a1, RV rv=0 ){
    if( !pc ) return rv;
    return (pc->*func)(a1);
}


template<class C,class A1, class A2>
void Try( C *pc, void (C::*func)(A1,A2), A1 a1, A2 a2 ){
    if( pc ) (pc->*func)(a1,a2);
}

template<class C, class RV, class A1, class A2>
RV Try( C *pc, RV (C::*func)(A1,A2), A1 a1, A2 a2, RV rv=0 ){
    if( !pc ) return rv;
    return (pc->*func)(a1,a2);
}


template<class C,class A1, class A2, class A3>
void Try( C *pc, void (C::*func)(A1,A2,A3), A1 a1, A2 a2, A3 a3 ){
    if( pc ) (pc->*func)(a1,a2,a3);
}

template<class C, class RV, class A1, class A2, class A3>
RV Try( C *pc, RV (C::*func)(A1,A2,A3), A1 a1, A2 a2, A3 a3, RV rv=0 ){
    if( !pc ) return rv;
    return (pc->*func)(a1,a2,a3);
}


/*
// Adopted from Andrei Alexandrescu: http://www.ddj.com/dept/cpp/184403791
template<int _v>
struct Int2Type {
    //enum { v = _v; };
};
*/

#endif // TEMPLUTILS_HPP


