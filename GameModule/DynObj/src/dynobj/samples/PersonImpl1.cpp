
// This will cause PersonI class registration info to come in our file.
#define DO_IMPLEMENT_PERSONI
#include "PersonI.h"

// %%DYNOBJ section general
// This section is auto-generated and manual changes will be lost when regenerated!!
#define DO_IMPLEMENTING     // Source file, always implementing
#include "dynobj/DynObj.h"

//
// --- Integer type ids for defined interfaces/classes ---
#define PERSONIMPL1_TYPE_ID 0xCADBC000

// %%DYNOBJ section end

// Our includes
#include <string.h>
#include <stdlib.h>
#include "dynobj/DoBase.hpp"    // This file gives us DynData which allow for
                                // flexible constructor arguments

#define NAME_MAX_LENGTH 100

// Declare the class to the pre-processor. The preprocessor picks 
// up the bases automatically (if we don't specify manually).
// %% DYNOBJ class(dyni,usertype) 
class PersonImpl1 : public PersonI {
public:

    // DynObj methods
	virtual DynObjType* docall doGetType( const DynI **pself ) const;
	virtual void docall doDestroy( ) { delete this; }
    
    // PersonI methods
    virtual const char* docall GetName( ) const {
        return m_name;
    }
    virtual int docall GetAge() const { return m_age; }
    
    virtual bool docall SetName( const char *name ){
        if( !name || strlen(name)>NAME_MAX_LENGTH ) return false;
        strcpy( m_name, name );
        return true;
    }
    virtual bool docall SetAge( int age ){
        if( age<0 ) return false;
        m_age = age;
        return true;
    }

    // Constructor, Init from a string: "Bart,45"
    PersonImpl1( const DynI* pdi_init ) : m_age(0) { 
        // We will do this setup slightly awkward now, and improve in 
        // the following examples.
        *m_name = 0;    // NUL terminated
        
        // A DynData allows to pass most basic C++ types and references
        // to any registered user types. It's a convenient way of 
        // sebding typed arguments to a constructor inside a plugin.
        
        // See if we have got a DynData as constructor argument?
        if( !pdi_init ) return;
        
        // This is the 'raw' way of casting to another type. 
        // It can be improved, doGetObj is not a const method, so we have to 
        // cast away constness temporarily.
        const DynData* pdd = (const DynData*)((DynI*)pdi_init)->doGetObj("DynData");
        if( pdd ){
        
            // Does it have string in store for us ?
            if( pdd->IsA<const char*>() ){
                const char *pc = pdd->Get<const char*>();
                if( pc ){
                    // Got a string, parse it
                    char buf[100];
                    strncpy( buf, pc, strlen(pc)<100?strlen(pc):99); buf[99]=0;
                    char *pcc = strchr(buf,',');
                    if( pcc ) *pcc = 0;
                    SetName(buf);
                    if( pcc )
                        SetAge(atoi(pcc+1));
                }
            }
        }
    }
    
protected:
    // Need not be protected since user of PersonI cannot look here anyway.
    char m_name[NAME_MAX_LENGTH];
    int  m_age;
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

    if( ((!strcmp(type,"PersonImpl1") || type_id==PERSONIMPL1_TYPE_ID)) || 
        ((!strcmp(type,"PersonI") && type_id==PERSONI_TYPE_ID)) ){
        return new PersonImpl1(pdi_init);
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
//
// This section is for setting up the type system at run-time using global or 
// static variable constructors. For types deriving from DynI, the doGetType 
// member function is implemented. For other types, global declarations are used.

// Generate type information that auto-registers on module load
DynObjTypeI2R<PersonImpl1,PersonI,true> g_do_vtype_PersonImpl1("PersonImpl1:PersonI",PERSONIMPL1_TYPE_ID,((DOT_USER_TYPE)<<16)|1);
// DynI::doGetType implementation for: PersonImpl1
DynObjType* PersonImpl1::doGetType( const DynI **pself ) const {
   if(pself) *pself=(const DynI*)(const void*)this;
   return &g_do_vtype_PersonImpl1;
}

// Declarations to make types visible to library users
doUserTypeDecl g_do_ut_decl_PersonImpl1_0("PersonImpl1:PersonI",PERSONIMPL1_TYPE_ID);

//
// %%DYNOBJ section end

