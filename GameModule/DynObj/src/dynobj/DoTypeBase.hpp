// Copyright (c) 2007, Arne Steinarson
// Licensing for DynObj project - see DynObj-license.txt in this folder


#ifndef DOTYPEBASE_HPP
#define DOTYPEBASE_HPP

#include "DoBase.hpp"

/// @cond internal

////////////////////////////////
// Type machinery - Convert type to integer
// 
#define DOT_IS_TYPE_POINTER 0x01   // 1: This is a type pointer, opens a nested type
#define DOT_UNUSED      0x02    // This is the only other bit that can be used for 32-bit aligned pointers.
//#define DOT_IS_NOT_LAST     0x02   // 1: This item is not last in a type list
#define DOT_REF         0x04    // It's a C++ reference (only for primitive types)
#define DOT_CONST       0x08
#define DOT_ARRAY       0x10       // A sized array (requires trailing size id)
#define DOT_PTR         0x20
#define DOT_PTR_MASK    0xE0
#define TO_PTR_MASK(n)  ((n&7)<<5)
#define TO_PTR_LEVEL(n) ((n>>5)&7)
#define DOT_PRIMITIVE   0x100	// A primitive type


#define DOT_COMMON_MASK     0xFFF    // Bits shared by both common and user-types
#define DOT_HAS_NO_VTABLE   0x80000000 // If type has a VTable
#define DOT_USERTYPE_MASK   0xFFFFF000 // All user-type bits

// Bits for primitive type 
#define DOT_TYPE_MASK 0xF000
#define DOT_VOID      0x0000
#define DOT_BOOL      0x1000
#define DOT_CHAR      0x2000
#define DOT_SINT      0x3000    // Signed integer
#define DOT_UINT      0x4000    // Unsigned integer
#define DOT_UNSIGNED  0x1000    // Difference between signed & unsigned
#define DOT_FLOAT     0x5000
#define DOT_FUNCTION  0x6000
#define DOT_PADDING   0x7000    // Padding inside structures
#define DOT_STRUCTURE 0x8000    // struct

#define DOT_SIZE_MASK 0x070000
#define DOT_SIZE_POS  16
#define DOT_SIZE_1    0x000000
#define DOT_SIZE_2    0x010000
#define DOT_SIZE_4    0x020000
#define DOT_SIZE_8    0x030000
#define DOT_SIZE_16   0x040000


// We jam pointer and ref together here which is good enough for now.
#define DOT_PTR_REF_LEVEL(t) (((t&DOT_PTR_MASK)+((t&DOT_REF)?DOT_PTR:0))>>3)

template<int N>
struct DoLog2{ enum { v=1+DoLog2<N/2>::v }; };

template<>
struct DoLog2<0>{ enum { v=-1 }; };

// Default case, user-type
template<class T>
struct DoType2Int {
    // If custom type, can check if it has a VTable
    enum { v = IsVObj<T>::v ? 0 : DOT_HAS_NO_VTABLE };
};

template<class T>
struct DoType2Int<const T> {
    enum { v=DOT_CONST|DoType2Int<T>::v };
};

template<class T>
struct DoType2Int<T&> {
    enum { v=DOT_REF|DoType2Int<T>::v };
};

template<class T>
struct DoType2Int<T*> {
    enum { v=DOT_PTR+DoType2Int<T>::v };
};

template<> struct DoType2Int<void> { enum { v=DOT_VOID|DOT_PRIMITIVE }; };
template<> struct DoType2Int<bool> { enum { v=DOT_BOOL|DOT_PRIMITIVE }; };

template<> struct DoType2Int<char> { enum { v=DOT_CHAR|DOT_PRIMITIVE }; };
template<> struct DoType2Int<wchar_t> { enum { v=DOT_CHAR|(DoLog2<sizeof(wchar_t)>::v<<DOT_SIZE_POS)|DOT_PRIMITIVE }; };

template<> struct DoType2Int<short> { enum { v=DOT_SINT|DOT_SIZE_2|DOT_PRIMITIVE }; };
template<> struct DoType2Int<int> { enum { v=DOT_SINT|(DoLog2<sizeof(int)>::v<<DOT_SIZE_POS)|DOT_PRIMITIVE }; };
template<> struct DoType2Int<long> { enum { v=DOT_SINT|(DoLog2<sizeof(long)>::v<<DOT_SIZE_POS)|DOT_PRIMITIVE }; };
template<> struct DoType2Int<long long> { enum { v=DOT_SINT|(DoLog2<sizeof(long long)>::v<<DOT_SIZE_POS)|DOT_PRIMITIVE }; };

template<> struct DoType2Int<unsigned short> { enum { v=DOT_UINT|DOT_SIZE_2|DOT_PRIMITIVE }; };
template<> struct DoType2Int<unsigned int> { enum { v=DOT_UINT|(DoLog2<sizeof(int)>::v<<DOT_SIZE_POS)|DOT_PRIMITIVE }; };
template<> struct DoType2Int<unsigned long> { enum { v=DOT_UINT|(DoLog2<sizeof(long)>::v<<DOT_SIZE_POS)|DOT_PRIMITIVE }; };
template<> struct DoType2Int<unsigned long long> { enum { v=DOT_UINT|(DoLog2<sizeof(long long)>::v<<DOT_SIZE_POS)|DOT_PRIMITIVE }; };

template<> struct DoType2Int<float> { enum { v=DOT_FLOAT|DOT_SIZE_4 }; };
template<> struct DoType2Int<double> { enum { v=DOT_FLOAT|DOT_SIZE_8 }; };

/// @endcond internal


/** @ingroup DynTemplate
 * Extract full type ID for a type.
 * This extracts full type ID (user type or primitive type) and adds any modifiers
 * to it (const/ref/pointers).
 * @return type ID integer.
 */
template<class T>
struct DoFullType2Int {
    /** The function returns full type ID for given type */
    static int Id(){ 
        // doTypeInfo::Id() return 0 for anything unknown and peels of any qualifiers
        return DoType2Int<T>::v | doTypeInfo<T>::Id();
    }
};


#endif // DO_TYPEBASE_HPP

