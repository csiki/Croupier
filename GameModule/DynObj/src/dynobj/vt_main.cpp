// Copyright (c) 2007, Arne Steinarson
// Licensing for DynObj project - see DynObj-license.txt in this folder

#define DO_IMPLEMENTING 
#define VTABLE_CORR_MINIMAL 0

#include <stdio.h>
#include "vt_test.h"

//#include "DynObj.h"
#include "DoVTableInfo.hpp"


extern "C" int g_do_traits;

int main(){_CFE_;
	int n_fail = 0;
	// Diagnostics for handling of virtual functions with this compiler 
	int vtbl_pos = VTablePos();	// 1
	if( vtbl_pos!=0 ){
		printf( "VTable not found at beginning of created object.\n" );
		printf( "This feature is vital. Bailing out.\n" );
		return -100;
	}
	printf( "VTablePos [VPTR]: At beginning of object [expected]\n" );
	
	int bc1_vts = Bc1VTableSize();	// 1
	if( bc1_vts!=1 ) n_fail++;
	printf( "Bc1VTableSize: %d [1 expected]\n", bc1_vts );
	
	int bc3_vts = Bc3VTableSize();	// 3
	printf( "Bc3VTableSize: %d [3 expected]\n", bc3_vts );
	if( bc3_vts!=3 ) n_fail++;
	
	int dc1_vts = Dc1VTableSize();	// 2
	printf( "Dc1VTableSize: %d [2 expected]\n", dc1_vts );
	if( dc1_vts!=2 ) n_fail++;
	
	int dc3_vts = Dc3VTableSize();	// 5
	printf( "Dc3VTableSize: %d [5 expected]\n", dc3_vts );
	if( dc3_vts!=5 ) n_fail++;
	
	// Inhertience padding 
	int ihp = InheritPadding();
	printf( "Inheritance padding: %d [0 expected]\n", ihp );
	if( ihp<0 ) n_fail++;
	
	int dtor_ne = DtorEntries();	// 1 (MSVC) or 2 (g++)
	printf( "DtorEntries: %d [1 or 2 expected]\n", dtor_ne );
	if( dtor_ne<1 || dtor_ne>4 ) n_fail++;
	
	int dtor_pos = DtorPosition();	// 1 - last, 0 - first,  < 
	printf( "DtorPosition: %d [1 expected, 0 is OK]\n", dtor_pos );
	if( dtor_pos<0 ) n_fail++;
    
	int tmfptr = TestMembFuncPtr();	// 0 if pointer to member function invocation works
	printf( "TestMembFuncPtr: %d [0 for success]\n", tmfptr );
	if( tmfptr ) n_fail++;
	
	int off_1st = -1;
	int vte_sz = VTableEntrySize(off_1st);	// Usually 4 (8 on 64 bit arch). Can be 8/16 if each vtable entry uses two void*
	printf( "VTableEntrySize: %d [4 expected (8/12/16 possible)]\n", vte_sz );
	if( vte_sz<4 || vte_sz>16 || vte_sz&3 ) n_fail++;

	int ct_tst = CallThisTest();	
	printf( "CallThisTest: %d [0 for success]\n", ct_tst );
	if( ct_tst ) n_fail++;
	
	int cvtbl_tst = CorrectVTableTest();
	printf( "CorrectVTableTest: %d [0 for success]\n", cvtbl_tst );
	if( cvtbl_tst ) n_fail++;
	
	int ol_tst = ObjectLayoutTest();	
	printf( "ObjectLayoutTest: %d [0 for success]\n", ol_tst );
	if( ol_tst ) n_fail++;
	
	int if_size = InterfaceSize();	
	printf( "InterfaceSize: %d [4/8 expected]\n", if_size );
	if( if_size<4 || if_size>8 ) n_fail++;
	
	/*
	int doct_tst = DoCtorTest();
	printf( "DoCtorTest: %d [0 for success]\n", doct_tst );
	if( doct_tst ) n_fail++;
	*/
	
	printf("\n==================================================\n");
	printf( "Summary: g_do_traits is 0x%08X [Compatibility info in one field]\n",g_do_traits );
	if( !n_fail )
		printf( "All tests succeeded - :-) - DynObj library should work well.\n" );
	else
		printf( "%d tests failed - :-(...\n", n_fail );
	printf("====================================================\n");

	return -n_fail;
}



