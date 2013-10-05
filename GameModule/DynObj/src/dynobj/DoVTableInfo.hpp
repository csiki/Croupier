// Copyright (c) 2007, Arne Steinarson
// Licensing for DynObj project - see DynObj-license.txt in this folder


#ifndef VTABLEINFO_HPP
#define VTABLEINFO_HPP

/*
    Note: This file is not part of the 'core' features of DynObj, but rather more 
    experimental. DynObj does not reqire any changes to VTables for normal operation.
*/

/*
	This code is inspired and partly built on the work with FastDelegates by Don Clugston.
	The problem soved here is similar, but opposite in a way. From a bound member function
	pointer he retrieved the address of a function to call (and the adjusted 'this' pointer).
	We don't care about those, but want to find the index of a virtual function in the VTable.
	
	For most compilers, the information is there, in a field in the member function pointer.
	However, for Intel and MSVC compilers, they don't use the offset into the VTable when
	calling it, but rather a fix address to a stub function that has the offset hard coded
	into it (not efficient nor pretty IMHO). We make a special case for that and obtain 
	the function address using some inline assembly. 
	
	Also, DMC (Digital Mars) solve the whole problem with member function pointers using 
	stub callers to a function for the various cases. For DMC, we look for the address
	contained in the function pointer in the VTable.
	
	NOTE: It is quite possible for the compiler to point to stubs that are not in the VTable,
	but rather refer the VTable themselves. The assembly code to extract the function 
	address of a virtual function (VTblSizer::AVfunc) is currently in MSVC syntax but 
	should be straight forward to move to another syntax.
	
	NOTE: The C++ standard ABI has settled on using the GNU solution below, where the 
	index is directly available, so most newer compilers will do that.
	
	Single stepping through assembly code calling member function pointers and looking
	at where the VTable entries are used will help in cases not covered below.
*/

#include <string.h>

#if defined(_MSC_VER) || defined(__INTEL_COMPILER)
	// For Microsoft & Intel compilers, it is not possible to get VTable
	// index from member function pointer. Get the address of the function
	// by calling it instead, using inline assembly to extract function 
	// address.
	#define GET_ADDR_VIA_CALL
#endif

// This says how many entries to copy from before the VTable start
// It is somewhat arbitrary now.
#define VTP_N_ENTRIES_BEFORE 4


// Declared in vt_test.cpp
extern void *g_do_TFA;
extern void *g_do_RA1;


// When looking for a function index in a VTable, how long do we go?
#define MAX_VTABLE_SEARCH 256

// A class to measure VTable size of Base 

// This just gives us ability to construct base with one ctor arg
template<class Base,class ARG>
struct VTblSizerBase : public Base {
    VTblSizerBase() : Base( (ARG)0 ) { }
};

// No arg case
template<class Base>
struct VTblSizerBase<Base,void> : public Base {
    VTblSizerBase() : Base() { }
};


template<class Base,class ARG>
struct VTblSizer : public VTblSizerBase<Base,ARG> {
	virtual int  AVFunc( ){
#ifdef GET_ADDR_VIA_CALL
		static bool b_inside = false;
		// We want two things (one might be enough):
		// - A return address inside this function (call recursively once)
		// - The plain non-virtual address of this function 
		if( !b_inside ){
			b_inside = true;
			//VTblSizer_VFunc();
			AVFunc();
			b_inside = false;
			return 0;
		}
		// There are two different symbols: AVFunc and VTblSizer<SomeClass>
		// AVfunc jumps here [VTblSizer<SomeClass>] and AVfunc is the one
		// we find in the VTable. The templatig may add some complexity 
		// to function calling, but we're lucky here.
		__asm{
			//mov eax, VTblSizer_VFunc
			mov eax, AVFunc
			mov g_do_TFA, eax
			mov eax, [ebp+4];
			mov g_do_RA1, eax
		}
#endif 
		return 0;
	}	
};


//template <class MIB,ARG>
//int GetVIndex( int (VTblSizer<MIB,ARG>::*mfn)(), void **vtbl ){
template <class C>
int GetVIndex( int (C::*mfn)(), void **vtbl ){
	// These are convenient to have later
	void **ppvfn = reinterpret_cast<void**>(&mfn);
	void *pvfn = ppvfn ? *ppvfn : NULL;
	
	// Size of member function pointer, what we have to look through 
	int mfn_size = sizeof(mfn);
	
#if defined(__GNUC__) || defined(__VISAGECPP__) || defined(__xlc__) || defined(__xlC__) 
	// G++ case
	if( mfn_size==sizeof(void*)+sizeof(int) ){
		int i = *(int*)&mfn;
		//return (i>0 && i&1) ? (i+1)/2 : -1;
		// G++ stores the actual offset+1 into the VTable, so we get:
		// entry 0: 1
		// entry 1: 5
		// entry 2: 9 ,...etc
		return (i>0 && i&1) ? i/4 : -1;
	}
#elif defined(__BORLANDC__) || defined(__TURBOC__) || defined(__WATCOMC__) 
	if( mfn_size==sizeof(void*)+2*sizeof(int) ){
		// Integer at offset 2 is vindex+1
		int *pi = (int*)(((char*)ppvfn)+sizeof(void*)+sizeof(int));
		return (*pi)-1;
	}
#elif defined(__MWERKS__)
	if( mfn_size==sizeof(void*)+2*sizeof(int) ){
		// Integer at offset 1 is vindex
		int *pi = (int*)(((char*)ppvfn)+sizeof(int));
		return *pi;
	}
#elif defined (__sgi) || defined (__EDG__) || defined(__COMO__)
	// MrCpp, MIPS Pro, Pro64, Open64 should go here also  // Edison Grouop, Comeau
	if( mfn_size==2*sizeof(short)+sizeof(void*) ){
		// Short at offset 1 is vindex
		int *ps = (short*)(((char*)ppvfn)+sizeof(short));
		return (int)*ps;
	}
#elif defined (_MSC_VER) || defined(__INTEL_COMPILER)
	// These apparently behaves similar. We have called the function now and can read
	// the globals it has written to.
#endif

	// The alternative that is left (and rather good) is to search the VTable
	// for an address and when found, return the index. We have up to 3 addresses
	// to look for.
	void *pvs[3];
	pvs[0] = NULL;
	pvs[1] = g_do_TFA;
	pvs[2] = g_do_RA1;
	// If member function pointer is same size as void*, then look directly
	// in supplied VTable for this value
	if( mfn_size==sizeof(void*) ){
		if( vtbl && ppvfn )
			pvs[0] = pvfn;
	}	

	// Iterate
	for( int ix=0; ix<MAX_VTABLE_SEARCH; ix++ ){
		if( !vtbl[ix] ) continue; 
        void *pv = vtbl[ix];
 		if( pv==pvs[0] || pv==pvs[1] || pv==pvs[2] )
			/*|| (pvs[2]>vtbl[ix] && pvs[2]<vtbl[ix+1]) )*/ // The 'in between' case (inside function body) 
			return ix;
	}

	// Exhausted possibilities for now 	
	return -1;
}

/*
template<class C>
void** GetVTableOf(){
	C c;
	return *(void***)&c;
}
*/

template<class C,class ARG>
int GetVTableSizeOf( ){
	VTblSizer<C,ARG> vtbs;				// Object derived from side base
	int (VTblSizer<C,ARG>::*mfn)() = &VTblSizer<C,ARG>::AVFunc;
	// Call the virtual function (for MSVC and Intel compilers, but it is cheap to do anyway
	g_do_TFA = g_do_RA1 = NULL;
	vtbs.AVFunc();
	int ix = GetVIndex( mfn, *(void***)&vtbs );
	return ix>=0 ? ix : -1;
}


//////////////////////////////////////////////////////////////////////////
// Structure used for keeping original, derived and replacement VTables
//
#define VTP_N_ENTRIES_BEFORE 4
struct DynObjType;

class VTableInfo {
public:
	VTableInfo( void** td, void **to, int sz) : vtbl_d(td), vtbl_o(to), vtbl_r(NULL), size(sz), pdt(NULL) { } 
	~VTableInfo(){ delete [] (vtbl_r-VTP_N_ENTRIES_BEFORE); }
	bool AllocSoftVTbl(){
		void **vtbl_a = new void*[size+VTP_N_ENTRIES_BEFORE];
		if( !vtbl_a ) return false;
		vtbl_r = vtbl_a+VTP_N_ENTRIES_BEFORE;
		// Copy entries
		memcpy( vtbl_a, vtbl_d-VTP_N_ENTRIES_BEFORE, sizeof(void*)*(size+VTP_N_ENTRIES_BEFORE) );
		return true;
	}
	
	void **vtbl_d;	 // Derived VTable
	void **vtbl_o;	 // Original VTable
	void **vtbl_r;   // Ram VTable 
	int  size;		 // Number of entries in dervied VTable
	DynObjType *pdt;
	//bool operator == (void** vtbl){ return vtbl_d==vtbl || vtbl_o==vtbl || vtbl_r==vtbl; } 
	bool operator == (void** vtbl){ return vtbl_d==vtbl || vtbl_r==vtbl; } 
};


#endif // VTABLEINFO_HPP
