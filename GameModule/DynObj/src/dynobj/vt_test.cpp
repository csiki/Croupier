// Copyright (c) 2007, Arne Steinarson
// Licensing for DynObj project - see DynObj-license.txt in this folder


#define DO_IMPLEMENT_VTTEST
#define DO_IMPLEMENTING 

#include <stdio.h>
#include "vt_test.h"
//#include "DynObj.h"
#include "DoVTableInfo.hpp"
#include "pi/visibility.h"

#define docall 

// Packed version describing characteristics of VTable usage.
// It allows for quickly testing for compatibility with a module.
static int g_do_traits;

// Globals used by doVTableInfo.hpp
void *g_do_TFA;	// Address of test function
void *g_do_RA1;	// Return address (inside function)

// Globals descrining VTable layout (after performing tests)
static int g_do_vtbl_es;		// Entry size, (4/8/16)
static int g_do_vtbl_off_1st;	// Offset (in nr of void*) to 1st valid entry (VFunc0 above)
                        // Usually 0 
static int g_do_vtbl_off;		// Inside an entry, offset to the function pointer
                        // Usually 0

int VTableIndex( int ix ){_CFE_;
	if( !g_do_vtbl_es ){
		int of;
		if( VTableEntrySize(of)<0 )
			return -1;
	}
	// This is a complex calculation for a simple thing... 
	return g_do_vtbl_off_1st+ix*g_do_vtbl_es/sizeof(void*)+g_do_vtbl_off;
}


int VTableCompatible( int traits ){_CFE_;
	// If not initialized yet
	if( !g_do_traits ) doGetDoTraits();
	
	if( (g_do_traits|traits)&DO_TRAITS_VT_ERROR ) return -1;
	
	int diff = g_do_traits^traits;
	// Entry size must match
	if( diff&(DO_TRAITS_MASK<<DO_TRAITS_VT_ES) ) return -2;
	// Offset to first entry must match
	if( diff&(DO_TRAITS_MASK<<DO_TRAITS_VT_OFF_1ST) ) return -2;
	// Offset within one entry must match
	if( diff&(DO_TRAITS_MASK<<DO_TRAITS_VT_OFF) ) return -3;
	// Inheritance padding must match
	if( diff&(DO_TRAITS_MASK<<DO_TRAITS_VT_IHP) ) return -4;

	// We can live with different entry count for destructo	
	if( diff&(DO_TRAITS_MASK<<DO_TRAITS_VT_DEC) ) return 1;
	
	return 0;
}



// If compiled with DynObj.cpp the VTables are there
#include "utils/ExpArr.hpp"
ExpArrPOwn<VTableInfo*> g_do_test_vtbls;


// This will restore an entry of the VTable to one of the ancestors
// member functions.
template <class C, class MIB>
int MiSideBaseRestoreVTableEntry( C *pc, MIB *psb, int eix ){_CFE_;
	VTableInfo *vtp = g_do_test_vtbls.Find(*(void***)psb);
	if( !vtp ){
		MIB sb;		// Side base object, have to declare to get its VTable
		int ix = GetVTableSizeOf<MIB,void>();
		if( ix<1 ) return -1;
		vtp = new VTableInfo(*(void***)psb,*(void***)&sb,ix);
		if( !vtp->AllocSoftVTbl() ){ delete vtp; return -2; }
		g_do_test_vtbls.Push(vtp);
	}
	if( eix>=vtp->size || eix<=-VTP_N_ENTRIES_BEFORE ) return -3;
    // Set entry from original VTable in RAM VTable
	vtp->vtbl_r[eix] = vtp->vtbl_o[eix];
	return 0;
}

// Set the Vtable to the RAM (soft) vtable
template <class MIB>
bool MiSideBaseSetRamVTable( MIB *psb ){_CFE_;
	VTableInfo *vtp = g_do_test_vtbls.Find(*(void***)psb);
	if( !vtp ) return false;
	*(void***)psb = vtp->vtbl_r;
	return true;
}

                        

class VTblEntSizeTest { 
public:
	// These functions are packed into a VTable. 
	// We find out which is which by calling two of them.
	// Careful with calling convention here, we're basically
	// converting to an ordinary function pointer below.
	virtual int docall VFunc0() {_CFE_; return 100; }
	virtual int docall VFunc1() {_CFE_; return 101; }
	virtual int docall VFunc2() {_CFE_; return 102; }
	virtual int docall VFunc3() {_CFE_; return 103; }
	virtual int docall VFunc4() {_CFE_; return 104; }
	virtual int docall VFunc5() {_CFE_; return 105; }
	virtual int docall VFunc6() {_CFE_; return 106; }
	virtual int docall VFunc7() {_CFE_; return 107; }
};

// Used to hold member function pointer as ordinary function pointer
typedef int (docall *Vp2IntFn)( void* );


// Testing VTable entry size.
// (we basically accept 1 or 2 void* per entry
// and a small offset before the first virtual function
// in the VTable)
// Usually 
int VTableEntrySize( int &off_first ){_CFE_;
	VTblEntSizeTest vtest;
	void **vtbl = *(void***)&vtest;
	for( int off=0; off<2; off++ ){
		try {
			// Try even entries, 2 and 4 (+offset 0/1)
			Vp2IntFn tfn = (Vp2IntFn)vtbl[2+off];
			int r1 = tfn ? tfn((void*)&vtest) : -1;
			if( r1<100 || r1>107 )
				continue;
			tfn = (Vp2IntFn)vtbl[4+off];
			int r2 = tfn ? tfn((void*)&vtest) : -1;
			if( r2<100 || r2>107 )
				continue;
			if( r2==r1 )
				continue;
			
			// It went fine. Look at result to count offset/size per entry
			int es = 2/(r2-r1);	// Really only es=1 or 2 possible
			off_first = -((r1-100) - (2+off)/es);
			g_do_vtbl_es = es*sizeof(void*);
			g_do_vtbl_off_1st = off_first;
			g_do_vtbl_off = off;
			
			g_do_traits |= ((es&0xF)<<DO_TRAITS_VT_ES);
			g_do_traits |= ((off_first&0xF)<<DO_TRAITS_VT_OFF_1ST);
			g_do_traits |= ((off&0xF)<<DO_TRAITS_VT_OFF);
			return g_do_vtbl_es;
		} catch(...){
			printf("Catch in VTableEntrySize\n");
		}
	}
	return -1;
}


class BC1{
public:
	virtual int A() {_CFE_; return (int)'A'; }
	int m_i;
};

int g_vt_dummy;

class BC1d{
public:
	virtual int A() {_CFE_; return (int)'A'; }
	virtual ~BC1d(){ g_vt_dummy++; }	// Add destructor
	int m_i;
};


// This usually returns 1, since we have 1 virtual func. 
// However, it can be OK to return >1, maybe some additional
// info is put into the VTable.
int Bc1VTableSize(){_CFE_;
	return GetVTableSizeOf<BC1,void>();
}

class BC3{
public:
	virtual int B() {_CFE_; return (int)'B'; }
	virtual int C() {_CFE_; return (int)'C'; }
	virtual int D() {_CFE_; return (int)'C'; }
	int m_i;
};

class BC3d : public BC3{
public:
	virtual ~BC3d(){ g_vt_dummy<<=1; }	// Destructor in derived class
};

class BC3dd : public BC3{
public:
	virtual int D() {_CFE_; return (int)'C'; }
	virtual int E() {_CFE_; return (int)'C'; }
	virtual int F() {_CFE_; return (int)'C'; }
	virtual ~BC3dd(){ g_vt_dummy<<=1; }	// Destructor in derived class
};

// This usually returns 3, since we have 3 virtual funcs. 
// It is correct to return 2 more than for BC1
int Bc3VTableSize(){_CFE_;
	return GetVTableSizeOf<BC3,void>();
}


class DC1 : public BC1 {
public:
	//virtual int A() { return (int)'a'; }
	virtual int B() {_CFE_; return (int)'b'; }
	int m_i;
};

// Test for VTable at beginning of objects. Some compilers may put it
// elsewhere, but that should be unusual.
int VTablePos( ){_CFE_;
	DC1 d1, d2;
	d1.m_i = 1;
	d2.m_i = 2;
	// Assumed VTable at first position should be same for both objects
	if( *(void***)&d1!=*(void***)&d2 )
		return -1;
	
	// In the Vtable for dc2, we should have something from the Vtable of BC1
	// because they share virtual functions
	BC1 bc;
	void **vtdc1 = *(void***)&d1; 
	void **vtbc1 = *(void***)&bc;
	if( !vtdc1 || !vtbc1 ) 
		return -2;
	try{
		for( int ix=0; ix<4; ix++ )
			for( int jx=0; jx<4; jx++ )
				if( vtbc1[jx] && vtdc1[ix]==vtbc1[jx] )
					return 0;
	}catch(...){
		return -3;
	}
	return -4;
}


// This usually returns 2, since we have 2 virtual funcs. 
// Theoretically, some padding could be put between base class
// virt funcs and those of the derived one.
int Dc1VTableSize(){_CFE_;
	return GetVTableSizeOf<DC1,void>();
}


class DC3 : public BC3 {
public:
	virtual int A() {_CFE_; return (int)'a'; }	// Override
	virtual int C() {_CFE_; return (int)'c'; }	// Override
	virtual int D() {_CFE_; return (int)'d'; }
	virtual int E() {_CFE_; return (int)'e'; }
	int m_i;
};


// This usually returns 5, since we have 3+2 virtual funcs. 
// Theoretically, some padding could be put between base class
// virt funcs and those of the derived one.
int Dc3VTableSize(){_CFE_;
	return GetVTableSizeOf<DC3,void>();
}

int InheritPadding(){_CFE_;
	int bc1_vts = Bc1VTableSize();	// 1
	int bc3_vts = Bc3VTableSize();	// 3
	int dc1_vts = Dc1VTableSize();	// 2
	int dc3_vts = Dc3VTableSize();	// 5

	int ihp1 = dc1_vts-bc1_vts-1;
	int ihp2 = dc3_vts-bc3_vts-2;
	if( ihp1!=ihp2 ) return -1;
	g_do_traits |= (ihp1<<DO_TRAITS_VT_IHP);
	return ihp1;
}


// Number of entries used for virtual destructor. DynObj framework
// does not depend on that or use it, so it is not necessary for 
// main and module to have same result here.
// Usually 1/2 entries.
int DtorEntries(){_CFE_;
	int ne_bc1d = GetVTableSizeOf<BC1d,void>() - GetVTableSizeOf<BC1,void>();
	int ne_dc3d = GetVTableSizeOf<BC3d,void>() - GetVTableSizeOf<BC3,void>();
	if( ne_bc1d!=ne_dc3d ){
		printf( "Found two different entry counts for Virt Dtor: %d %d\n", ne_bc1d, ne_dc3d );
		return -1;
	}
	g_do_traits |= ((ne_bc1d&0xF)<<DO_TRAITS_VT_DEC);
	return ne_bc1d;
}


// Positioning of destructor in derived object
// First or last
int DtorPosition(){_CFE_;
    BC1d bc1d;
	int ne_bc1d = GetVTableSizeOf<BC1d,void>();
	int a_ix = GetVIndex<BC1d>(&BC1d::A,*(void***)(&bc1d));
    //printf( "DtorPos - Size: %d  Pos:%d  (3,0)\n", ne_bc1d, a_ix );
    
    BC3dd bc3dd;
	int ne_bc3dd = GetVTableSizeOf<BC3dd,void>();
	int e_ix = GetVIndex<BC3dd>(&BC3dd::E,*(void***)(&bc3dd));
    //printf( "DtorPos - Size: %d  Pos:%d  (7,3)\n", ne_bc3dd, e_ix );

    bool first = false;
    bool at_end = false;
    if( a_ix==0 && e_ix==3 ) at_end = true;
    else if( a_ix>0 && e_ix>3 ) first = true;
    
	if( !first && !at_end ){
		printf( "Could not determine destructor position: S1-%d P1-%d   S2-%d P2-%d\n", 
                ne_bc1d, a_ix,  ne_bc3dd, e_ix );
		return -1;
	}
    if( at_end )
        g_do_traits |= (1<<DO_TRAITS_VT_DTOR_POS);

	return at_end;
}



// All tests below are excluded usually 
#ifdef VT_ALL_TESTS

class DC {
public:
	int m_d1;
};

// See if we can detect VTable size correctly for POD
// (data structure without VTable)
// Return value 0 is correct
int PodVTableSize(){_CFE_;
	return GetVTableSizeOf<DC,void>();
}


// Member function pointers
typedef int (DC3::*DC3PMF)();
typedef int (DC1::*DC1PMF)();

// Test what pointer to member func does
// Can be used for testing how virtual functions are accessed with a certain
// compiler. Stepping through the assembler language level will show how it works. 
// Return 0 if it works as expected.
// Return -1 if it fails.
int TestMembFuncPtr(){_CFE_;
	DC3PMF dc3pmf;
	DC1PMF dc1pmf;
	DC1 dc1, *pdc1=&dc1;
	DC3 dc3, *pdc3=&dc3;
	int n_ok=0;
	
	dc3pmf = &DC3::A;
	if( (dc3.*dc3pmf)()==(int)'a' ) n_ok++;
	dc3pmf = &DC3::B;
	if( (dc3.*dc3pmf)()==(int)'B' ) n_ok++;
	dc3pmf = &DC3::D;
	if( (dc3.*dc3pmf)()==(int)'d' ) n_ok++;
	
	dc1pmf = &DC1::A;
	if( (dc1.*dc1pmf)()==(int)'A' ) n_ok++;
	dc1pmf = &DC1::B;
	if( (dc1.*dc1pmf)()==(int)'b' ) n_ok++;

	return n_ok==5 ? 0 : -1;	
}



class CttClass {
public:
	virtual char* docall TestFunc( int to_add ){_CFE_; return ((char*)this)+to_add; }
};

typedef char* (docall *Int2CpFn)( void*, int );

// Check that we can use the VTable to make an arbitrary call to an index,
// with working this pointer and an argument.
int CallThisTest(){_CFE_;
	// Need to run above config function first (VTableEntrySize)
	if( !g_do_vtbl_es ) return -1;
	CttClass cttc;
	void **vtbl = *(void***)&cttc;
	try{
		Int2CpFn i2cfn = (Int2CpFn)vtbl[VTableIndex(0)];
		char *pc = i2cfn ? i2cfn(&cttc,100) : NULL;
		return pc==((char*)&cttc)+100 ? 0 : -1;
	}
	catch(...){
		return -2;
	}
}

class TheBase{
public:
    virtual const char *GetType() const { return "TheBase"; }
    virtual int AnotherFunc(){_CFE_; return 0; }
};


class MainBase : public TheBase {
public:
	virtual const char* GetType( ) const { 
		return "MainBase:TheBase";
	}
	virtual int AFunc(){_CFE_; return 3; }
	int m_mbi;	// Assure main base position 
};

class SideBase : public TheBase {
public:
	virtual const char* GetType( ) const { 
		return "SideBase:TheBase";
	}
	int m_sbi;
};

class MainType : public MainBase, public SideBase {
public:
	virtual const char* GetType( ) const { 
		return "MainType:MainBase,SideBase";
	}
	int m_mti;
};

class MyType : public MainType {
public:
	virtual const char* GetType( ) const { 
		return "MyType:MainType";
	}
	//virtual void* docall doGetObj(const char* type){ return ::doGetObj((void*)this,doGetType( const DynI **pself ),type); }
};

// Here we test correcting the VTable for a side base class when using multiple 
// inheritence. If we have inheritence situation:
// A : public B, public C
// we want to make sure that if we override function B::f to A::f and there is 
// also a function C::f, that we can restore C::f, so that it does not end up 
// in A::f. 
// This is important, otherwise all sub objects in a compound object will 
// return the same type. 
// This feature adjusts the All-or-nothing effect when using virtual functions
// for multiple base classes. We can select for which bases we want to override
// (the main base is not selectable though).
int CorrectVTableTest(){_CFE_;
	MainType mt, *pmt=&mt;
	//MainBase mb;
	MainBase *pmb = (MainBase*)pmt;
	SideBase *psb = (SideBase*)pmt;
	if( (void*)psb==(void*)pmt ){
		printf("CorrectVTableTest: Unexpected position of side base (before main base?)\n");
		return -1;
	}
	const char *type = mt.GetType();
	const char *mb_type1 = pmb->GetType();
	const char *sb_type1 = psb->GetType();
	
	try{
		if( MiSideBaseRestoreVTableEntry( pmt, psb, 0 )<0 )
			return -2;
		if( !MiSideBaseSetRamVTable( psb ) )
			return -3;
	} catch (...) {
		printf("CorrectVTableTest: Catch while calling MiSideBase...\n");
		return -4;
	}

	const char *mb_type2 = pmb->GetType();
	const char *sb_type2 = psb->GetType();
    
    //printf("Before: %s %s\n", mb_type1, sb_type1 );
    //printf("After: %s %s\n", mb_type2, sb_type2 );

 	// If successful, the side base type should be changed now
	return (mb_type2==mb_type1 && sb_type2!=sb_type1 && sb_type1 && sb_type2) ? 0 : -5;
}

/*
	Two tests:
	1 - Test for base class layout - do the classes appear in declared order
       in an instance with multiple bases?
   2 - What is the space occupied in an object by a class with only methods?
*/
// Some bases with only virtual methods
class V1 {
public:
	virtual int M0(){_CFE_; return 0; }
};

class V2 {
public:
	virtual int M1(){_CFE_; return 0; }
	virtual int M2(){_CFE_; return 0; }
	virtual int M3(){_CFE_; return 0; }
};

class V3Base1 {
public:
	virtual int M0(){_CFE_; return 0; }
	virtual int M1(){_CFE_; return 0; }
};

class V3Base2 {
public:
	virtual int M20(){_CFE_; return 0; }
	virtual int M21(){_CFE_; return 0; }
};

class V3 : public V3Base1, public V3Base2 {
public:
	virtual int M0(){_CFE_; return 0; }
	virtual int M1(){_CFE_; return 0; }
	virtual int M2(){_CFE_; return 0; }
	virtual int M3(){_CFE_; return 0; }
	virtual int M4(){_CFE_; return 0; }
	virtual int M5(){_CFE_; return 0; }
	virtual int M6(){_CFE_; return 0; }
};


// Classes with both virtual methods and data
class VD1 {
public:
	virtual int M0(){_CFE_; return 0; }
	char m_buf[100];
	int m_i;
	void *m_pv;
};

class VD2 : public V2 {
public:
	virtual int M2(){_CFE_; return 2; }
	virtual int M3(){_CFE_; return 3; }
	char m_buf[100];
	char m_buf1[200];
	int m_i;
	int m_i1;
	void *m_pv;
};

class VD3Base {
public:
	int m_vd3i;
};

class VD3 : public VD3Base, public V3 {
public:
	virtual int M1(){_CFE_; return 1; }
	virtual int M2(){_CFE_; return 2; }
	virtual int M3(){_CFE_; return 3; }
	virtual int M4(){_CFE_; return 4; }
	long m_ls[100];
	VD1 m_vd1;
	V2  m_v2;
};


class MBV123 : public V1, public V2, public V3 {
public:
	MBV123(){ g_vt_dummy++; };
};

class MBV321 : public V3, public V2, public V1 {
public:
	MBV321(){ g_vt_dummy++; };
};


class MBD123 : public VD1, public VD2, public VD3 {
public:
	MBD123(){ g_vt_dummy++; };
};

class MBD321 : public VD3, public VD2, public VD1 {
public:
	MBD321(){ g_vt_dummy++; };
};


class MBM123 : public V1, public VD2, public V3 {
public:
	MBM123(){ g_vt_dummy++; };
};

class MBM321 : public VD3, public VD2, public V1 {
public:
	MBM321(){ g_vt_dummy++; };
};


int TripStrInc( void* triple[] ){
   return triple[0]<triple[1] && triple[1]<triple[2];
}

// Returns <0 if some of the tests fails
int ObjectLayoutTest(){_CFE_;
	void *triple[3];
	bool results[6];
	int n_ok = 0;
	
	MBV123 mbv123;
	triple[0] = (V1*)&mbv123;	
	triple[1] = (V2*)&mbv123;	
	triple[2] = (V3*)&mbv123;	
	if( results[0]=(TripStrInc(triple) && triple[0]==&mbv123) )
		n_ok++;
	
	MBV321 mbv321;
	triple[0] = (V3*)&mbv321;	
	triple[1] = (V2*)&mbv321;	
	triple[2] = (V1*)&mbv321;	
	if( results[1]=(TripStrInc(triple) && triple[0]==&mbv321) )
		n_ok++;

				
	MBD123 mbd123;
	triple[0] = (VD1*)&mbd123;	
	triple[1] = (VD2*)&mbd123;	
	triple[2] = (VD3*)&mbd123;	
	if( results[2]=(TripStrInc(triple) && triple[0]==&mbd123) )
		n_ok++;
	
	MBD321 mbd321;
	triple[0] = (VD3*)&mbd321;	
	triple[1] = (VD2*)&mbd321;	
	triple[2] = (VD1*)&mbd321;	
	if( results[3]=(TripStrInc(triple) && triple[0]==&mbd321) )
		n_ok++;


	MBM123 mbm123;
	triple[0] = (V1*)&mbm123;	
	triple[1] = (VD2*)&mbm123;	
	triple[2] = (V3*)&mbm123;	
	if( results[4]=(TripStrInc(triple) && triple[0]==&mbm123) )
		n_ok++;
	
	MBM321 mbm321;
	triple[0] = (VD3*)&mbm321;	
	triple[1] = (VD2*)&mbm321;	
	triple[2] = (V1*)&mbm321;	
	if( results[5]=(TripStrInc(triple) && triple[0]==&mbm321) )
		n_ok++;

	for( int i=0; i<(int)sizeof(results); i++ )
		if( !results[i] )
			printf( "ObjectLayoutTest: Failed test %d \n", i );
			
	return n_ok-sizeof(results);
}

// Returns number of bytes used by an interface in an object 
int InterfaceSize( ){_CFE_;
	int if_sz;
	MBV123 mbv123;
	MBV321 mbv321;
	MBM123 mbm123;
	if_sz = (int)(V2*)&mbv123 - (int)(V1*)&mbv123;
	if( if_sz!=(int)(V3*)&mbv123 - (int)(V2*)&mbv123 )
		return -1;
	
	// V3 has two bases with VFuncs
	if( if_sz!=((int)(V2*)&mbv321 - (int)(V3*)&mbv321)/2 )
		return -2;
	
	if( if_sz!=(int)(V1*)&mbv321 - (int)(V2*)&mbv321 )
		return -3;
			
	if( if_sz!=(int)(VD2*)&mbm123 - (int)(V1*)&mbm123 )
		return -4;
		
	return if_sz;
}

/*
// Here the DoCtor method is invoked to automatically correct 
// vtable entries for 'local main type' methods
int DoCtorTest(){_CFE_;
	MainType mt;
	SideBase *psb = (SideBase*)&mt;
	DynObjType *ptsb1 = psb->doGetType( const DynI **pself );
	try{
		mt.DoCtor();
	}catch(...){
		printf("DoCtorTest: Catch while calling DoCtor\n");
		return -1;
	}

	DynObjType *ptsb2 = psb->doGetType( const DynI **pself );
	return (ptsb1 && ptsb2 && ptsb1!=ptsb2) ? 0 : -2;
}
*/
#endif // VT_ALL_TESTS


int doGetDoTraits() {_CFE_;
    g_do_traits = 0;
    int off_first;
    if( VTablePos()<0 || 
        VTableEntrySize(off_first)<0 || 
        InheritPadding()<0 || 
        DtorEntries()<0 || 
        DtorPosition()<0 ) {
        ; //printf("VTModConstr - Error\n");
        g_do_traits |= DO_TRAITS_VT_ERROR;
    }
    return g_do_traits;
}

/*
// Run basic tests automatically on load 
class VTModConstr {
public:
	VTModConstr() {
        
        // Run required tests for setting up g_do_traits
        int of;
		if( VTablePos()<0 || 
            VTableEntrySize(of)<0 || 
            InheritPadding()<0 || 
            DtorEntries()<0 || 
            DtorPosition()<0 ) {
			; //printf("VTModConstr - Error\n");
			g_do_traits |= DO_TRAITS_VT_ERROR;
		}
		else
			; //printf("VTModConstr - Success 0x%08X\n", g_do_traits);
	}
	
} g_vt_mod_constr;
*/

