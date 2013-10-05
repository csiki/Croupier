// This will cause PersonI class registration info to come in our file.
#define DO_IMPLEMENT_PERSONI
#include "PersonI.h"

// Simlar with ProfessionI
#define DO_IMPLEMENT_PROFESSIONI
#include "ProfessionI.h"

// %%DYNOBJ section general
// This section is auto-generated and manual changes will be lost when regenerated!!
#define DO_IMPLEMENTING     // Source file, always implementing
#include "dynobj/DynObj.h"

//
// --- Integer type ids for defined interfaces/classes ---
#define PERSONIMPL3_TYPE_ID 0xE75D2000

// %%DYNOBJ section end


// Our includes
#include <string.h>
#include <stdlib.h>

#include "dynobj/DoBaseI.h"
#include "dynobj/DynStr.h"
#include "dynobj/DynObjHolder.hpp"
#include "dynobj/DoBase.hpp"


// %% DYNOBJ class(dyni,usertype) 
class PersonImpl3 : public PersonI, public ProfessionI {
public:

    // DynObj methods
	virtual DynObjType* docall doGetType( const DynI **pself ) const;
	virtual void docall doDestroy( ) { delete this; }
    
    // PersonI methods
    virtual const char* docall GetName( ) const {
        return m_name().c_str();
    }
    virtual int docall GetAge() const { return m_age; }
    
    virtual bool docall SetName( const char *name ){
        m_name() = name;
        return true;
    }
    virtual bool docall SetAge( int age ){
        if( age<0 ) return false;
        m_age = age;
        return true;
    }


    // ProfessionI methods
    // We're implementing the second interace here.
    // Still very fix, no improvements...
    virtual const char* docall GetProfession( ) const {
        return "Miner";
    }
    
    virtual int docall GetExperience() const {
        return 18; // years
    }


    // We override the comparison operator and use the Unicode aware
    // string comparison of DynStr.
    virtual bool docall operator<( const PersonI& other ) const { 
        // Is the other one a PersonImpl3 ? We do dynamic type query to find out. 
        const PersonImpl3 *opi3 = do_cast<const PersonImpl3*>(&other);
		if( opi3 ){
			int r = m_name().Stricmp(opi3->m_name().c_str());
            return r<0;
		}
        else 
            // Give to base case 
            return PersonI::operator<(other);
    }

    // Constructor
    // DynObjHolder will ask DynObjRunTimeI to create an object of type
    // DynStr. So here we're back operating at a C++ type level.
    PersonImpl3( const DynI *pdi_init ) : m_age(0), m_name() { 
        if( !m_name.IsOk() ){
            // This does object specific error tracking, but keeps all 
            // error data stored away from the object.
            // We derive twice from DynI, so we have to tell which base
            // we're calling SetError for.
            PersonI::SetError( "Failed setting up DynObjHolder with DynStr" );
        }
        
        else if( pdi_init ) {
            // See what we have got as argument 
            const char *pc = NULL;
            const DynData& dd = do_cast_ref<const DynData&>(*pdi_init);
            if( dd.IsOk() && dd.IsA<const char*>() )
                // It's a DynData and it has a string for us
                pc = dd.Get<const char*>();
            else{
                // Check if arg is a DynStr
                const DynStr& ds = do_cast_ref<const DynStr&>(*pdi_init);
                if( ds.IsOk() )
                    // Cast DynStr to const char*
                    pc = (const char*)ds;
            }
            
            m_name() = (const char*)pc;
            
            // Use DynStr functions to chop up string
            int pos = m_name().Find(",");
            if( pos>0 ) {
                SetAge( atoi(m_name().Right(pos)+1) );
                m_name().Trunc(pos);
            }
        }
    }
    
//protected:
    // Need not be protected since user of PersonI cannot look here anyway.
    
    DynObjHolder<DynStr> m_name;
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

    if( ((!strcmp(type,"PersonImpl3") || type_id==PERSONIMPL3_TYPE_ID)) || 
        ((!strcmp(type,"PersonI") && type_id==PERSONI_TYPE_ID)) ){
        return new PersonImpl3(pdi_init);
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
DynObjTypeI2R<PersonImpl3,PersonI,true> g_do_vtype_PersonImpl3("PersonImpl3:PersonI",PERSONIMPL3_TYPE_ID,((DOT_USER_TYPE)<<16)|1);
SideBaseDecl g_do_vtype_sbd_PersonImpl3_1("PersonImpl3","ProfessionI",sizeof(ProfessionI),sidebase_offset<PersonImpl3,ProfessionI>(),NULL,-1,NULL);
// DynI::doGetType implementation for: PersonImpl3
DynObjType* PersonImpl3::doGetType( const DynI **pself ) const {
   if(pself) *pself=(const DynI*)(const void*)this;
   return &g_do_vtype_PersonImpl3;
}

// Declarations to make types visible to library users
doUserTypeDecl g_do_ut_decl_PersonImpl3_0("PersonImpl3:PersonI",PERSONIMPL3_TYPE_ID);

//
// %%DYNOBJ section end

