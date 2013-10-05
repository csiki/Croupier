// This will cause PersonI class registration info to come in our file.
#define DO_IMPLEMENT_PERSONI
#include "PersonI.h"

#define DO_IMPLEMENT_PROFESSIONI
#include "ProfessionI.h"

// %%DYNOBJ section general
// This section is auto-generated and manual changes will be lost when regenerated!!
#define DO_IMPLEMENTING     // Source file, always implementing
#include "dynobj/DynObj.h"

//
// --- Integer type ids for defined interfaces/classes ---
#define PERSONIMPL2_TYPE_ID 0xC2FD7000

// %%DYNOBJ section end


// Our includes
#include <string.h>
#include <stdlib.h>
#include "dynobj/DoBaseI.h"
#include "dynobj/DynStr.h"
#include "dynobj/DoBase.hpp"

// We're implementing two interfaces here. 
struct ABC{ };

// %% DYNOBJ class(dyni,usertype)
class PersonImpl2 : public PersonI, public ProfessionI {
public:

    // DynObj methods
	virtual DynObjType* docall doGetType( const DynI **pself ) const;
	virtual void docall doDestroy( ) { delete this; }
    
    // PersonI methods
    virtual const char* docall GetName( ) const {
        return m_pds ? m_pds->c_str() : NULL;
    }
    virtual int docall GetAge() const { return m_age; }
    
    virtual bool docall SetName( const char *name ){
        if( !m_pds ) return false;
        *m_pds = name;
        return true;
    }
    virtual bool docall SetAge( int age ){
        if( age<0 ) return false;
        m_age = age;
        return true;
    }

    // ProfessionI methods
    // We're implementing the second interace here.
    // This is very fix (cannot change profession), but will do for now
    virtual const char* docall GetProfession( ) const {
        return "Surgeon";
    }
    
    virtual int docall GetExperience() const {
        return 12; // years
    }


    // Constructor
    PersonImpl2( const DynI* pdi_init ) : m_age(0), m_pds(0) {
    	Ctor( pdi_init ); 
    }
    void Ctor( const DynI* pdi_init ) {
        DoRunTimeI &dort = doGetRunTime();
        if( dort.IsOk() ){
            // Create our internal string, using DoRunTimeI 
            m_pds = (DynStr*)dort.Create(NULL,"DynStr",DYNSTR_TYPE_ID);
			if( !m_pds )
                // We derive twice from DynI, so we need to tell from which of 
                // these to get SetError.
                PersonI::SetError( "Failed creating DynStr from DoRunTimeI" );
                
            else if( pdi_init ){
                // See what we have got as argument 
                const char *pc = NULL;
                const DynData& dd = do_cast_ref<const DynData&>(*pdi_init);
                if( dd.IsOk() && dd.IsA<const char*>() )
                    // It's a DynData and it has a string for us
                    pc = dd.Get<const char*>();
                else{
                    const DynStr& ds = do_cast_ref<const DynStr&>(*pdi_init);
                    if( ds.IsOk() )
                        // Cast DynStr to const char*
                        pc = (const char*)ds;
                }
                
                if( pc ){
                    char buf[100];
					int sl = (int)strlen(pc);
					if( sl>99 ) sl=99;
                    strncpy( buf, pc, sl ); buf[sl]=0;
                    char *pcc = strchr(buf,',');
                    if( pcc ) *pcc = 0;
                    SetName(buf);
                    if( pcc )
                        SetAge(atoi(pcc+1));
                }
            }
        }
    }
    
    ~PersonImpl2( ){ if(m_pds) m_pds->doDestroy(); }
    
protected:
    // Need not be protected since user of PersonI cannot look here anyway.
    
    DynStr *m_pds;
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

    if( ((!strcmp(type,"PersonImpl2") || type_id==PERSONIMPL2_TYPE_ID)) || 
        ((!strcmp(type,"PersonI") && type_id==PERSONI_TYPE_ID)) ){
        return new PersonImpl2(pdi_init);
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
DynObjTypeI2R<PersonImpl2,PersonI,true> g_do_vtype_PersonImpl2("PersonImpl2:PersonI",PERSONIMPL2_TYPE_ID,((DOT_USER_TYPE)<<16)|1);
SideBaseDecl g_do_vtype_sbd_PersonImpl2_1("PersonImpl2","ProfessionI",sizeof(ProfessionI),sidebase_offset<PersonImpl2,ProfessionI>(),NULL,-1,NULL);
// DynI::doGetType implementation for: PersonImpl2
DynObjType* PersonImpl2::doGetType( const DynI **pself ) const {
   if(pself) *pself=(const DynI*)(const void*)this;
   return &g_do_vtype_PersonImpl2;
}

// Declarations to make types visible to library users
doUserTypeDecl g_do_ut_decl_PersonImpl2_0("PersonImpl2:PersonI",PERSONIMPL2_TYPE_ID);

//
// %%DYNOBJ section end

