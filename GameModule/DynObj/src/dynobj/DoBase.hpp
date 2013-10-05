// Copyright (c) 2007, Arne Steinarson
// Licensing for DynObj project - see DynObj-license.txt in this folder

#ifndef DOBASE_HPP
#define DOBASE_HPP

#include <string.h>

// Basic interfaces
#include "DoBaseI.h"
#include "DoError.h"

// Include DynShared support
#if DO_USE_DYNSHARED==1
    #include "DoDynShared.hpp"
#endif


// Doxygen stuff
/// @cond internal

///////////////////////////////////
// This compile-time assert is adopted from Boost
template<bool b>
struct DoErrorIfNot;

template<>
struct DoErrorIfNot<true>{ };

#define DO_ASSERT(cond) sizeof(DoErrorIfNot<cond>)

//////////////////////////////////////
// Peel off pointers, refs & const
template <class T>
struct DoBaseType {
    typedef T type;
};

template <class T>
struct DoBaseType<T*> {
    typedef typename DoBaseType<T>::type type;
};

template <class T>
struct DoBaseType<T&> {
    typedef typename DoBaseType<T>::type type;
};

template <class T>
struct DoBaseType<const T> {
    typedef typename DoBaseType<T>::type type;
};


/////////////////////////////////////////
// Simple test for same type
template <class T, class U>
struct IsSameType {
    enum { v=false };
};

template <class T>
struct IsSameType<T,T> {
    enum { v=true };
};




///////////////////////////////////////////////
// Tests if a class has a VTable or not
template<class T>
struct IsVObjImpl {
    struct IA : public T {
        int m;
    };
    struct IB : public T {
        virtual void F( );
        int m;
    };
    enum { v = (sizeof(IA)==sizeof(IB)) }; 
};

/// @endcond internal


/** @ingroup DynTemplate
 * Test if a type has a VTable or not. 
 * @return v is set to true if type has a VTable and false otherwise.
 */
template<class T>
struct IsVObj {
    typedef typename DoBaseType<T>::type type;
    enum { v=IsVObjImpl<type>::v || IsSameType<VObj,type>::v };
};


/// @cond internal

/*
template<class T>
struct IsVObj<T*> {
	enum { v = IsVObj<T>::v };
};

template<class T>
struct IsVObj<T&> {
	enum { v = IsVObj<T>::v };
};
*/

// This avoids long error messages for the void case
template<>   
struct IsVObj<void> {
	enum { v = false };
};

///////////////////////////////////////////////////
// Is a class derived from DynI?

template <class T, class U>
struct DoIsTypeAImpl {
	// Test functions
	static char F( U* );
	static short F(...);
	static T* pt;
	enum { v=sizeof(F(pt))==sizeof(char) };
};

template <class T, class U>
struct DoIsTypeA {
    enum { v=DoIsTypeAImpl<typename DoBaseType<T>::type,U>::v };
};



// Check if type is const
template <class T>
struct DoIsConst {
    enum { v=0 };
};

template <class T>
struct DoIsConst<const T> {
    enum { v=1 };
};

template <class T>  // Should this be needed?
struct DoIsConst<const T&> {
    enum { v=1 };
};

template <class T>  // Should this be needed?
struct DoIsConst<const T*> {
    enum { v=1 };
};


/////////////////////////////////////////
// Check if type is ref or not
template <class T>
struct DoIsRef { 
    enum { v=0 };
};

template <class T>
struct DoIsRef<T&> { 
    enum { v=1 };
};


/////////////////////////////////////////
// Check if type is ref or not
template <class T>
struct DoIsPointer { 
    enum { v=0 };
};

template <class T>
struct DoIsPointer<T*> { 
    enum { v=1 };
};




// Casting based on type ID:s
template<class U, int M, int N>
struct DoCaster;	// Generic case, compile time error

template<class U, int N>
struct DoCaster<U,0,N> {	// Case for detected VObj
	static U Cast( const void *pv, doCastAlgo algo ){
        if( !pv ) return 0;
        return reinterpret_cast<U>(::doGetObj((void*)pv,doTypeInfo<U>::Id(),NULL,algo));
	}
};

template<class U, int N>
struct DoCaster<U,1,N> {	// Case for DynI
	static U Cast( const DynI *pdi, doCastAlgo algo ) {
        if( !pdi ) return 0;
		return reinterpret_cast<U>(::doGetObj((void*)pdi,doTypeInfo<U>::Id(),DI_GET_TYPE(pdi),algo));
	}
};

#if DO_USE_DYNSHARED==1
template<class U, int N>
struct DoCaster<U,N,1> {	// Case for RTDynShared
	static U Cast( const RTDynShared *prtds, doCastAlgo algo ) {
        if( !prtds ) return 0;
        // Return pointer to self?
        if( IsSameType<U,DynSharedI>::v ||
            IsSameType<U,DynI>::v ||
            IsSameType<U,VObj>::v ) return prtds;
        else
        	return DoCaster<U,2,0>::Cast( prtds, algo );
	}
};
#else // DO_USE_DYNSHARED
    // Provide dummy template class for test below
    struct RTDynShared { };
#endif


// Type-string case (for do_cast_str)
template<class U, int M, int N>
struct DoCasterStr;	// Generic case, compile time error

template<class U, int N>
struct DoCasterStr<U,0,N> {	// Case for detected VObj
	static U Cast( const void *pv, doCastAlgo algo ){
        if( !pv ) return 0;
        return reinterpret_cast<U>(::doGetObj((void*)pv,doTypeInfo<U>::Name(),NULL,algo));
	}
};

template<class U, int N>
struct DoCasterStr<U,1,N> {	// Case for DynI
	static U Cast( const DynI *pdi, doCastAlgo algo ) {
        if( !pdi ) return 0;
		return reinterpret_cast<U>(::doGetObj((void*)pdi,doTypeInfo<U>::Name(),DI_GET_TYPE(pdi),algo));
	}
};

#if DO_USE_DYNSHARED==1
template<class U, int N>
struct DoCasterStr<U,N,1> {	// Case for RTDynShared
	static U Cast( const RTDynShared *prtds, doCastAlgo algo ) {
        if( !prtds ) return 0;
        // Return pointer to self?
        if( IsSameType<U,DynSharedI>::v ||
            IsSameType<U,DynI>::v ||
            IsSameType<U,VObj>::v ) return prtds;
        else
        	return DoCasterStr<U,2,0>::Cast( prtds, algo );
	}
};
#endif


/// @endcond internal


/** @ingroup DynTemplate
 * Cast a pointer to another C++ type across DLL/SO boundaries. 
 * This does type lookup using information from the library compiler.
 * It is the equivalent of dynamic_cast in C++ or QueryInterface in COM. 
 * It uses integer type ID:s internally (not type strings) and is therefore 
 * faster then string based casts.
 * @return a pointer to the requested interface/class if found, otherwise NULL.
 */
template<class U, class T>
U do_cast( T* pt, doCastAlgo algo=doCastCross ) {
	// OK when we have positive on IsVObj or IsDynI, 
	// negative when both are false (compile time error, we 
	// cannot cast from the type).
    // Verify constness first (compile time)
    DO_ASSERT( DoIsConst<U>::v || !DoIsConst<T>::v );
    DO_ASSERT( IsVObj<T>::v );
	return DoCaster< U, DoIsTypeA<T,DynI>::v, DoIsTypeA<T,RTDynShared>::v >::Cast(pt,algo);
}

/** @ingroup DynTemplate
 * Cast a reference to another C++ type across DLL/SO boundaries. 
 * This does type lookup using information from the source object compiler.
 * It is the equivalent of dynamic_cast in C++ or QueryInterface in COM. 
 * It uses integer type ID:s internally (not type strings) and is therefore 
 * faster then string based casts.
 * @return a pointer to the requested interface/class if found, otherwise NULL.
 */
template<class U, class T>
U do_cast_ref( T& t, doCastAlgo algo=doCastCross ) {
    // Verify constness first (compile time)
    DO_ASSERT( (int)DoIsConst<U>::v >= (int)DoIsConst<T>::v );
    DO_ASSERT( IsVObj<T>::v );
    // Then cast
	return *DoCaster< typename DoBaseType<U>::type*, DoIsTypeA<T,DynI>::v, DoIsTypeA<T,RTDynShared>::v >::Cast(&t,algo);
}



/** @ingroup DynTemplate
 * As above but uses type strings (and can therefore find custom objects).
 * Using string based casts is slower than casts based on integer ID.
 * @return a pointer to the requested interface/class if found, otherwise NULL.
 */
template<class U, class T>
U do_cast_str( T* pt, doCastAlgo algo=doCastCross ) {
    // Verify constness first (compile time)
    DO_ASSERT( DoIsConst<U>::v || !DoIsConst<T>::v );
    DO_ASSERT( IsVObj<T>::v );
	return DoCasterStr< U, DoIsTypeA<T,DynI>::v, DoIsTypeA<T,RTDynShared>::v >::Cast(pt,algo);
}

/** @ingroup DynTemplate
 * As above but uses type strings and for references.
 * @return a referece to the requested interface/class if found, otherwise NULL.
 */
template<class U, class T>
U do_cast_ref_str( T& t, doCastAlgo algo=doCastCross ) {
    // Verify constness first (compile time)
    DO_ASSERT( (int)DoIsConst<U>::v >= (int)DoIsConst<T>::v );
    DO_ASSERT( IsVObj<T>::v );
	return *DoCasterStr< typename DoBaseType<U>::type*, DoIsTypeA<T,DynI>::v, DoIsTypeA<T,RTDynShared>::v >::Cast(&t,algo);
}



/** @ingroup DynTemplate
 * Cast arbitrary type to VObj if it can be safely done. 
 * If T is not a VObj, it generates a compile time error.
 * @param t a pointer/reference/instance of any type
 * @return a pointer to a VObj (the same object) if this can be done.
 */
template<class T>
VObj *to_vobj( T t ) {
    DO_ASSERT( IsVObj<T>::v );
	return reinterpret_cast<VObj*>(t);
}

template<class T>
VObj &to_vobj( T& t ) {
    DO_ASSERT( IsVObj<T>::v );
	return reinterpret_cast<VObj&>(t);
}


template<class T>
struct doTypeWrapper{ };



///////////////////////////////////////////////////////
// DynData - temporary arbitrary type data holder
//
#include <string.h>
#include <assert.h>

#include "DoTypeBase.hpp"

#define DYNDATA_MAX_SIZE sizeof(double)

/** @ingroup DynI 
 * Store/retrieve any datatype at run-time. 
 * Dynamically typed data holder. Stores data any (most) run-time data together 
 * with its type. The most important use is together with DynObj constructors. 
 */
// %%DYNOBJ class(dyni) flags(notypeid,inst2reg)
class DynData : public DynI { 
public:
    DynObjType* docall doGetType( const DynI **pself ) const;
    
    DynData( ) : m_type(0) { }
    
    /** @name Main interface */
    //@{
    
    /** Constructor, takes arbitrary type. */
    template<class T>
    DynData( T t ) {
        // Check that size is OK
        DO_ASSERT( sizeof(T) <= DYNDATA_MAX_SIZE );
        
        m_type = DoFullType2Int<T>::Id();
        
        // Fill up with 0 or -1 if signed
        memset( m_data, ((m_type&DOT_TYPE_MASK)==DOT_SINT)?-1:0, DYNDATA_MAX_SIZE );
        
#ifdef PFM_BIG_ENDIAN        
        // Motorola style, value stored at end
        *(T*)(m_data+DYNDATA_MAX_SIZE-sizeof(T)) = t;
#else
        // Intel style, value always stored first
        *(T*)m_data = t;
#endif
    }
    
    /** Returns current type id. 
     * @return full type ID including pointer level, const and ref 
     */
    int GetType(){ return m_type; }
    
    /** Check if the DynData can return requested type without loosing precision.
     * This mainly follows C++ casting rules. 
     * @return true if type can be cast to, false otherwise.
     */
    template<class T>
    bool IsA() const {
        int type = DoFullType2Int<T>::Id();
        if( type==UNKNOWN_TYPE_ID ) return false;
        
        if( type==m_type ||             // Exactly the same type
            type==(m_type|DOT_CONST) )  // or a const version is asked for 
            return true;
            
        // If wrong pointer/ref level, we fail
        if( DOT_PTR_REF_LEVEL(type)!=DOT_PTR_REF_LEVEL(m_type) ) return false;

        // OK const-ness? Out must be equal or more const
        if( !(type&DOT_CONST) && (m_type&DOT_CONST) ) return false;
            
        // Request for a user-type ?
        if( !(type&DOT_PRIMITIVE) ){
            // If we are a primitive type, fail.
            if( m_type&DOT_PRIMITIVE ) return false;
            
            // We check if the requested type is a direct base class of ours
            DynObjType *dot = doFindType(m_type&TYPE_ID_MASK);
            if( !dot ){
                VObj::SetError( "DynData::IsA - Failed looking up DynObjType", m_type );
                return false;
            }
            // ## This asks type from callers module. Code should be (and is)
            // in callers module.
            // See if we have this class as a main base class
            return dot->IsA( type&TYPE_ID_MASK );
        }

        // If we are a user type, the only primitive type we can convert
        // to is const void* or bool
        if( !(m_type&DOT_PRIMITIVE) ) {
            // Must be pointer or ref
            if( !(m_type&(DOT_PTR_MASK|DOT_REF)) ) return false;
            return type==DoType2Int<const void*>::v || 
                   type==DoType2Int<bool>::v;
        }
            
        // Both are primitive types
        
        // Same base type? (char/signed int/unsigned int)
        if( (type&DOT_TYPE_MASK)==(m_type&DOT_TYPE_MASK) ){
            // We can expand a small integer/char to a larger one
        	if( (type&DOT_SIZE_MASK)>=(m_type&DOT_TYPE_MASK) )
        		return true;
        	// We could also check that our value is in the requested range
        	// That is really a run-time check
            // I.e int(5) could be casted to unsigned int
            // but int(-2) could not
        }
        
        return false;
    }
    
    /** Cast to requested type.
     * This can be applied after checking the IsA function above. If type is 
     * not supported a run-time assert happens.
     * @return The internal value casted to requested type without any loss of precision.
     */
    template<class T>
    T Get() const {
		bool b = IsA<T>();
        assert( b ); // # Is this a good solution? Maybe better to set object error.
        // We must request either a const reference or 
        // not a reference at all.
        DO_ASSERT( DoIsConst<T>::v || 
                   !(DoIsRef<T>::v && !DoIsPointer<T>::v) );
                   
#ifdef PFM_BIG_ENDIAN        
        // Motorola style, value stored at end
        return *reinterpret_cast<const T*>(m_data+DYNDATA_MAX_SIZE-sizeof(T)); 
#else
        // Intel style, value always stored first
        return *reinterpret_cast<const T*>(m_data); 
#endif
    }
    //@}

/*
    // Basically we want the same signature without args.
    double GetDouble() const {
        assert( m_type&DOT_TYPE_MASK==DOT_FLOAT );
        switch( m_type&DOT_SIZE_MASK ){
#ifdef PFM_BIG_ENDIAN        
            case DOT_SIZE_8: return (double)*reinterpret_cast<const float*>(m_data+DYNDATA_MAX_SIZE-sizeof(double));
#else
            case DOT_SIZE_8: return *reinterpret_cast<const double*>(m_data);
#endif
            
#ifdef PFM_BIG_ENDIAN        
            case DOT_SIZE_4: return (double)*reinterpret_cast<const float*>(m_data+DYNDATA_MAX_SIZE-sizeof(float));
#else
            case DOT_SIZE_4: return (double)*reinterpret_cast<const float*>(m_data);
#endif
            default: assert(0);
        }
        return 0.0;
    }
*/    
    // We could also provide:
    //   bool CanBeA()
    // and 
    //   T Extract() 
    // which would allow for converting/lossy casts
    
protected:
    int m_type;
	union {
		char m_data[DYNDATA_MAX_SIZE];
		const char* m_str;		// For debugging convenience
	};
};


/** @ingroup DynTemplate
 * Wrapper around a C++ reference to an instance. 
 * C++ cannot have both a function that takes an instance
 * and a function that takes a reference to an instance. This 
 * template allows to keep the reference without instantiating.
 * Used in the dispatch functions below (do_try). */
template<class T>
struct doKeepRef {
    doKeepRef( T& t ) : m_t(t) { }
    operator T& (){ return m_t; }
    T& m_t;
};


/** @ingroup DynTemplate
 * A template to try to execute a single function on another interface.
 * The function and arguments are passed along, and a default return value 
 * as last argument (0 if not specified).
 * If cast to the other interface fails, the default value is returned.
 * A reference cannot be returned in this way. */
template<class I>
void do_try( DynI *pdi, void (I::*func)() ){
	I *pi = do_cast<I*>(pdi);
	if( pi ) (pi->*func)();
}
/** @ingroup DynTemplate
 * As above, but no args, return value */
template<class I, class RV>
RV do_try( DynI *pdi, RV (I::*func)(), RV rv=0 ){
	I *pi = do_cast<I*>(pdi);
	if( !pi ) return rv;
	return (pi->*func)();
}

/** @ingroup DynTemplate
 * As above, one arg, no return value */
template<class I, class A1, class _A1>
void do_try( DynI *pdi, void (I::*func)(A1), _A1 a1 ){
	I *pi = do_cast<I*>(pdi);
	if( pi ) (pi->*func)(a1);
}
/** @ingroup DynTemplate
 * As above, one arg, return value */
template<class I, class RV, class A1, class _A1>
RV do_try( DynI *pdi, RV (I::*func)(A1), _A1 a1, RV rv=0 ){
	I *pi = do_cast<I*>(pdi);
	if( !pi ) return rv;
	return (pi->*func)(a1);
}

/** @ingroup DynTemplate
 * As above, two args, no return value */
template<class I, class A1, class A2, 
                  class _A1, class _A2>
void do_try( DynI *pdi, void (I::*func)(A1,A2), _A1 a1, _A2 a2 ){
	I *pi = do_cast<I*>(pdi);
	if( pi ) (pi->*func)(a1,a2);
}
 /** @ingroup DynTemplate
 * As above, two args, return value */
template<class I, class RV, class A1, class A2, 
                            class _A1, class _A2>
RV do_try( DynI *pdi, RV (I::*func)(A1,A2), _A1 a1, _A2 a2, RV rv=0 ){
	I *pi = do_cast<I*>(pdi);
	if( !pi ) return rv;
	return (pi->*func)(a1,a2);
}

template<class I, class A1, class A2, class A3, 
                  class _A1, class _A2, class _A3>
void do_try( DynI *pdi, void (I::*func)(A1,A2,A3), _A1 a1, _A2 a2, _A3 a3 ){
	I *pi = do_cast<I*>(pdi);
	if( pi ) (pi->*func)(a1,a2,a3);
}
 // No args, return value
template<class I, class RV, class A1, class A2, class A3, 
                            class _A1, class _A2, class _A3>
RV do_try( DynI *pdi, RV (I::*func)(A1,A2,A3), _A1 a1, _A2 a2, _A3 a3, RV rv=0 ){
	I *pi = do_cast<I*>(pdi);
	if( !pi ) return rv;
	return (pi->*func)(a1,a2,a3);
}



// %%DYNOBJ section general
// This section is auto-generated and manual changes will be lost when regenerated!!
#ifdef DO_IMPLEMENT_DOBASE
    #define DO_IMPLEMENTING     // If app has not defined it already
#endif
#include "dynobj/DynObj.h"

// --- Forward class declarations ---
class DynData;

// --- For each declared class, doTypeInfo template specializations ---
DO_DECL_TYPE_INFO(DynData,DYNDATA_TYPE_ID);

// %%DYNOBJ section end



// %%DYNOBJ section implement
// This section is auto-generated and manual changes will be lost when regenerated!!
#ifdef DO_IMPLEMENT_DOBASE

// Generate type information that auto-registers on module load
DynObjTypeI2R<DynData,DynI,false> g_do_vtype_DynData("DynData:DynI",DYNDATA_TYPE_ID,1);
DynObjType* DynData::doGetType( const DynI **pself ) const {
   if(pself) *pself=(const DynI*)(const void*)this;
   return &g_do_vtype_DynData;
}
#endif //DO_IMPLEMENT_...
// %%DYNOBJ section end

#endif //DOBASE_HPP


