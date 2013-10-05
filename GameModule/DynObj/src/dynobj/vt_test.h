// Copyright (c) 2007, Arne Steinarson
// Licensing for DynObj project - see DynObj-license.txt in this folder


#ifndef VT_TEST_H
#define VT_TEST_H


// Bit offsets in the traits field
#define DO_TRAITS_MASK        0x0F
#define DO_TRAITS_VT_ES			0	// Entry size divided by 4
#define DO_TRAITS_VT_OFF_1ST	4	// void* offset to 1st entry
#define DO_TRAITS_VT_OFF		8	// void* offset inside entry
#define DO_TRAITS_VT_DEC		12	// Nr of slots used by virtual destructor
#define DO_TRAITS_VT_DTOR_POS	15	// Destructor positioning 0 - first, 1 - at end
#define DO_TRAITS_VT_IHP		16	// Inheritance padding 
#define DO_TRAITS_VT_ERROR    0x80000000	// Set if tests failed


// The results of tests
int doGetDoTraits();


// Get the actual slot (void*) of index ix
int VTableIndex( int ix );

// Are the traits passed here compatible with current compiler/environment?
// Returns <0 on error (incompatible)
// 0 - Compatible, but virtual destructors use different count
//     This is not an issue since don't use them by default.
// 1 - Fully compatible
int VTableCompatible( int traits );


// Default is to include only basic tests
// Define VT_ALL_TESTS to include all tests

int InheritPadding(); // >=0, 0 expected
int VTablePos( );	// 0 - at start
int DtorEntries(); // >=1, 1 or 2 expected
int VTableEntrySize( int &off_first );


#ifdef VT_ALL_TESTS
int PodVTableSize();	// 0
int Bc1VTableSize(); // 1
int Bc3VTableSize();	// 3
int Dc1VTableSize(); // 2
int Dc3VTableSize(); // 5

int CallThisTest();	// 0
int CorrectVTableTest();	// 0
int TestMembFuncPtr();	// 0
int ObjectLayoutTest(); // 0 
int InterfaceSize();	// 4/8 - sizeof(void*)
int DtorPosition(); // 0-first, 1-at end

#endif // VT_ALL_TESTS


#endif // VT_TEST_H


