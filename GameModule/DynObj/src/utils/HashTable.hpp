
#ifndef HT_PTR_HPP
#define HT_PTR_HPP

#include "utils/ExpArr.hpp"
#include "utils/TemplUtils.hpp"
#include "pi/platform.h"

#define HTP_INIT_WITH_WIDTH   3		   // Two power of initial size
#ifndef HTP_LOAD_FACTOR_GROW 
	#define HTP_LOAD_FACTOR_GROW  2.0	// Grow when load factor exceeds this
#endif


template <class Key, class Val>
class HashData {
public:
	//HashData( Key key, Val val ) : m_k(key), m_v(val) { }
	HashData( ) { }
	Key		m_k;
	Val		m_v;
	bool operator == (Key k){ return m_k==k; }
};

template<class Key>
struct HashTblHasher;

// Generic pointer case
template<class T>
struct HashTblHasher<T*> {
    static int Hash( T* pt, int bw_half ){
		// Assumption: We hash directly on pointer, not looking at the contents
		// For 64-bit case: OK to loose upper 32 bits here
		int iv = reinterpret_cast<int>(pt) >> PTR_ALIGN_BITS;
		return (0x3c97*iv + (iv>>bw_half) + iv/153);
    }
};

// int (index) case
template<>
struct HashTblHasher<int> {
    static int Hash( int i, int bw_half ){ return i; }
};


template<class Key, class Val>
class HashTable {
public:
	HashTable( int ibw=HTP_INIT_WITH_WIDTH, float lfg=HTP_LOAD_FACTOR_GROW ) : 
		m_bw(ibw), m_lfg(lfg), m_n_elem(0) {
		// Init hash table
		m_ht.MinCurrentSize( 1<<m_bw );
		m_base = m_ht.Base();
		m_hmask = (1<<m_bw)-1;
		m_bw_half = m_bw / 2;
	}
	// This will only delete the nodes we're using
	~HashTable( ){ m_ht.DeleteAll(); m_unused.DeleteAll(); } 
	
	//int Hash( Key k ){
    //    return (HashTblHasher<Key>::Hash(k)&m_hmask);
	//}
	
	bool Insert( Key k, Val v ){
		// If multiple threads insert, we need to lock this
		int ix = HashTblHasher<Key>::Hash(k,m_bw_half)&m_hmask;
		TLLNode<HashData<Key,Val> >* pnode;
		if( m_unused.Size() ){
			pnode = m_unused.Pop();
			pnode->m_nxt = m_base[ix];
		}
		else pnode = new TLLNode<HashData<Key,Val> >(m_base[ix]);
		if( !pnode ) return false;
		pnode->m_t.m_k = k;
		pnode->m_t.m_v = v;
		// Put first at this slot
		m_base[ix] = pnode;
		
		// Rehash?
		if( (float)m_n_elem++/m_ht.Size()>=m_lfg )
			GrowTable( );
		return true;
	}

	Val Lookup( Key k ){
		int ix = HashTblHasher<Key>::Hash(k,m_bw_half)&m_hmask;
		TLLNode<HashData<Key,Val> >* pnode = m_base[ix];
		while( pnode ){
			if( pnode->m_t.m_k==k )
				return pnode->m_t.m_v;
			pnode = pnode->m_nxt;
		}
		return GetNullElem();
	}
	
	Val LookupNext( Key k, Val prev ){
		int ix = HashTblHasher<Key>::Hash(k,m_bw_half)&m_hmask;
		TLLNode<HashData<Key,Val> >* pnode = m_base[ix];
        bool do_break = false;
		while( pnode && !do_break ){
			if( pnode->m_t.m_k==k && pnode->m_t.m_v==prev )
                do_break = true;
			pnode = pnode->m_nxt;
		}
        // Found previous match, now look for next 
		while( pnode ){
			if( pnode->m_t.m_k==k )
                return pnode->m_t.m_v;
			pnode = pnode->m_nxt;
		}
		return GetNullElem();
	}
	
	// This is a little easier than to remove based on Key
	bool Remove( Key k ) {
		int ix = HashTblHasher<Key>::Hash(k,m_bw_half)&m_hmask;
		TLLNode<HashData<Key,Val> >* pnode;
        // Removes all instances
        int n_elem = m_n_elem;
        while( pnode = TLLRemove(k,m_base+ix) ){
            m_unused.Push(pnode);
            m_n_elem--;
        }
		return m_n_elem<n_elem;
	}
	
	bool Remove( Key k, Val v ) {
		int ix = HashTblHasher<Key>::Hash(k,m_bw_half)&m_hmask;
		TLLNode<HashData<Key,Val> >** ppnode = TLLFind(k,m_base+ix);
        while( *ppnode ){
            if( (*ppnode)->m_t.m_k==k && (*ppnode)->m_t.m_v==v ){
                m_unused.Push(*ppnode);
                m_n_elem--;
                *ppnode = (*ppnode)->m_nxt;   
                return true;
            }
        }
		return false;
	}
	
    static Val& GetNullElem(){
        static Val val;
        return val;
    }
    
	bool GrowTable( ){
		// Grow the array
		m_bw++;
		m_ht.MinCurrentSize(1<<m_bw);
		if( m_ht.Size()!=(1<<m_bw) ) { m_bw--; return false; }
		m_hmask = (1<<m_bw)-1;
		m_base = m_ht.Base();
		m_bw_half = m_bw/2;
		
		// Gather all keys
		ExpArr<Key> keys;
		for( int ix=0; ix<m_ht.Size(); ix++ )
			for( TLLNode<HashData<Key,Val> >*pnode=m_base[ix]; pnode; pnode=pnode->m_nxt )
				keys.Push( pnode->m_t.m_k );
		
		// Go to old size limit, reinsert elements where Hash index has changed
		int new_ix;//, tot_cnt=0;
		for( int ix=m_hmask/2; ix>=0; ix-- ){
			TLLNode<HashData<Key,Val> >** ppnode = m_base+ix;
			while( *ppnode ){
				new_ix = HashTblHasher<Key>::Hash((*ppnode)->m_t.m_k,m_bw_half) & m_hmask;
				if( new_ix!=ix ){
					// Move to new slot
					TLLNode<HashData<Key,Val> >* pnode = *ppnode;
					*ppnode = pnode->m_nxt;
					pnode->m_nxt = m_base[new_ix];
					m_base[new_ix] = pnode;
				}
				else ppnode = &(*ppnode)->m_nxt;
			}
		}
		
		for( int ix=0; ix<keys.Size(); ix++ )
			if( !Lookup(keys[ix]) )
				printf( "Rehash, lookup failed for key %p\n", keys[ix] );
				
		return true;
	}
								
	int m_bw;		// Two power size of table
	int m_bw_half;
	int m_hmask;
	float m_lfg;	// Load factor to grow
	int m_n_elem;
	
	ExpArrP<TLLNode<HashData<Key,Val> >*> m_ht;		// The hash table
	TLLNode<HashData<Key,Val> >** m_base;			// Pointer to base of array
	ExpArrP<TLLNode<HashData<Key,Val> >*> m_unused;	// Store of unused nodes
};

#endif // HT_PTR_HPP


