// Copyright (c) 2007, Arne Steinarson
// Licensing for DynObj project - see DynObj-license.txt in this folder


// Instructions to PDOH preprocessor
// %%DYNOBJ  options(brief)  


// %%DYNOBJ section general
// This section is auto-generated and manual changes will be lost when regenerated!!
#define DO_IMPLEMENTING     // Source file, always implementing
#include "dynobj/DynObj.h"

// --- Integer type ids for defined interfaces/classes ---
#define DYNSTRC_TYPE_ID 0xA47EB000

// %%DYNOBJ section end

#include "DoError.h"
// To make sure ExpArr<char> never throws exceptions
#define EA_NO_THROW
// And reports errors through DynObj.cpp
#define EA_LOG_ERROR(str) DO_LOG_ERR(DOERR_CLASS_DYNSTR,str)


#include <string.h>
#define DO_IMPLEMENT_DYNSTR
#include "DynStr.h"
#include "DoBase.hpp"
#include "utils/utf8String.h"
#include "utils/unicode.h"

// Two base classes, the string class is put as a side class to the interface
// %%DYNOBJ class(dyni,usertype) bases(DynStr) flags(inst2reg)
class DynStrC : public DynStr, public u8Str {
public:
	DynStrC( const DynI* pdi_init=NULL ) { 
        if( pdi_init->IsOk() ){
            // Check if string
            const DynStr &ds = *do_cast<const DynStr*>(pdi_init);
            if( ds.IsOk() ){
                AssignA( ds.Get(), ds.ByteSize() );
            }
            else {
                const DynData &dd = *do_cast<const DynData*>(pdi_init);
                if( dd.IsOk() ){
                    if( dd.IsA<const char*>() )
                        AssignA( dd.Get<const char*>(), -1 );
                    else if( dd.IsA<const wchar_t*>() )
                        AssignW( dd.Get<const wchar_t*>(), -1 );
                }
            }
        }
    }
	
	virtual DynObjType* docall doGetType( const DynI **pself ) const;
	virtual void docall doDestroy( ){_CFE_; ::delete this; }
	
	virtual DynStr& docall AssignA(const char *ps, int l){_CFE_;
		u8Str::Assign(ps,l);
		return *(DynStr*)this;
	}
	virtual DynStr& docall AssignW(const wchar_t *pws, int l){_CFE_;
		u8Str::Assign(pws,l);
		return *(DynStr*)this;
	}
	virtual int docall Strcmp (const char *s) const {
		if( !s || !Base() )
			return Base()<s;
		return strcmp(Base(),s);
	}
	virtual int docall Stricmp(const char *s) const {
		if( !s || !Base() )
			return Base()<s;
		return utf8stricmp(Base(),s);
	}
	
	virtual const char* docall Get( ) const { 
		return Base();
	}
	virtual int docall GetByte(int ix) const {	// Returns one byte (8-bit char)
        //if( ix==STR_AT_END ) ix=cur_size-2;
        if( ix<0 ) ix += cur_size-1;
		if( ix<0 || ix>=Size() )
			return -1;
		return ta[ix];
	}
	virtual int docall GetUniChar(int ix) const {	// Returns one Unicode code-point (32-bit)
		return u8Str::operator[](ix);
	}
	virtual bool docall SetByte(int ix, char ch){_CFE_;	// Returns one byte (8-bit char)
		if( ix<0 || ix>Size() )
			return false;
		ta[ix] = ch;
		return true;
	}
	virtual bool docall SetUniChar(int ix, int ch){_CFE_;	
		return u8Str::SetUniChar(ix,ch);
	}
	
	virtual int docall Length() const {	
		return u8Str::Length();
	}
	virtual int docall ByteSize() const {	
		return scString::Size();
	}
	
	virtual DynStr& docall Insert(int at, const char *s, int len=-1){
		scString::Insert(at,s,len);
		return *(DynStr*)this;
	}
	virtual DynStr& docall Insert(int at, int ch){_CFE_;
		u8Str::Insert(at,ch);
		return *(DynStr*)this;
	}
	virtual DynStr& docall Remove(int at, int cnt){_CFE_;
		u8Str::Remove(at,cnt);
		return *(DynStr*)this;
	}
    
	virtual const char* docall Right(int from_pos) const {
		return u8Str::Right(from_pos);
	}
	virtual DynStr& docall Strip(bool left=true, bool right=true){
		scString::Strip(left,right);
		return *(DynStr*)this;
	}
	
	virtual DynStr& docall Reset(){_CFE_;
		scString::Reset();
		return *(DynStr*)this;
	}
	virtual bool docall IsEmpty() const {
		return !ta || !scString::Size();
	}
	
	virtual int docall Subst(int ch_find, int ch_repl){_CFE_;
		return u8Str::Subst(ch_find,ch_repl);
	}
	virtual int docall Subst(const char *s_find, const char *s_repl){_CFE_;
		return u8Str::Subst(s_find,s_repl);
	}
	// Looks for given string, return Unicode offset
	virtual int docall Find(const char *str) const {
		return u8Str::Find(str);
	}
	// Looks for given string at offset 'pos' (Unicode offset)
	virtual bool docall FindAt(const char *str, int pos=0) const {
		return u8Str::FindAt(str,pos);
	}
};


// %% DYNOBJ library

// %%DYNOBJ section library
// This section is auto-generated and manual changes will be lost when regenerated!!

// The code below implements the library part in a loadable class library (plugin). 

// Only include below when compiling as a separate library
#ifdef DO_MODULE

extern "C" SYM_EXPORT DynObjLibImpl* InitDynLib( DoRunTimeI* pdort ){
	// Library specific init code here.
	
    // If the library is compiled with DynObj.cpp, it does not need 
    // to return a pointer to DynObjLibImpl. 
    // If independent of DynObj.cpp, a correctly initialized pointer
    // must be returned.
	return 0;
}

// The object creation function for this module
extern "C" SYM_EXPORT DynObj* CreateDynObj( const char *type, int type_id, const DynI *pdi_init, int object_size ){
    // This library don't support creating objects with other sizes
    if( object_size>0 ){ DO_LOG_ERR( DOERR_NO_FLEX_OBJECT_SIZE, "Custom object size not supported" ); return 0; }
    
    // Match type names and type IDs, then return a new object

    if( ((!strcmp(type,"DynStrC") || type_id==DYNSTRC_TYPE_ID)) || 
        ((!strcmp(type,"DynStr") && type_id==DYNSTR_TYPE_ID)) ){
        return new DynStrC(pdi_init);
    }
    DO_LOG_ERR1( DOERR_UNKNOWN_TYPE, "CreateDynObj(%s): Unknown object type", type );
    return 0;
}

#ifdef _WIN32
// Every DLL needs this on Win32
extern "C" bool __stdcall DllMain( void* hModule,
                       int  ul_reason_for_call,
                       void* lpReserved )
{
    return true;
}
#endif

// Library specific exit code here
extern "C" SYM_EXPORT bool ExitDynLib( bool is_query ){
    return true;
}
#endif // DO_MODULE
// %%DYNOBJ section end


// %%DYNOBJ section implement
// This section is auto-generated and manual changes will be lost when regenerated!!

// Generate type information that auto-registers on module load
DynObjTypeI2R<DynStrC,DynStr,true> g_do_vtype_DynStrC("DynStrC:DynStr",DYNSTRC_TYPE_ID,((DOT_USER_TYPE)<<16)|1);
DynObjType* DynStrC::doGetType( const DynI **pself ) const {
   if(pself) *pself=(const DynI*)(const void*)this;
   return &g_do_vtype_DynStrC;
}

// Declarations to make types visible to library users
doUserTypeDecl g_do_ut_decl_DynStr_0("DynStrC:DynStr",DYNSTRC_TYPE_ID);
// %%DYNOBJ section end

