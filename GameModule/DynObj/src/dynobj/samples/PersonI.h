#ifndef PERSONI_H
#define PERSONI_H

// %%DYNOBJ section general
// This section is auto-generated and manual changes will be lost when regenerated!!
#ifdef DO_IMPLEMENT_PERSONI
    #define DO_IMPLEMENTING     // If app has not defined it already
#endif
#include "dynobj/DynObj.h"
//
// These are general definitions & declarations used on both the user side [main program]
// and the implementor [library] side. 

//
// --- Integer type ids for defined interfaces/classes ---
#define PERSONI_TYPE_ID 0xBA7C8000

//
// --- Forward class declarations ---
class PersonI;

// --- For each declared class, doTypeInfo template specializations ---
// This allows creating objects from a C++ types and in run-time casts
DO_DECL_TYPE_INFO(PersonI,PERSONI_TYPE_ID);

// %%DYNOBJ section end


#include <string.h>     // We use strcmp below
class DynStr;

// %%DYNOBJ class(DynI)
class PersonI : public DynObj {
public:
    // DynI method
	virtual DynObjType* docall doGetType( const DynI **pself=NULL) const;
    
    virtual const char* docall GetName( ) const = 0;
    virtual int docall GetAge() const = 0;
    
    virtual bool docall SetName( const char *name ) = 0;
    virtual bool docall SetAge(int age) = 0;
    
    // Simple default implementation, derived may override
    virtual bool docall operator<( const PersonI& other ) const { 
        return strcmp(GetName(),other.GetName()) < 0;
    }
    
    // Non-virtual, inline convenience function. Derived cannpt override.
    PersonI& operator=( const PersonI& other ) {
        SetAge( other.GetAge() );
        SetName( other.GetName() );
        return *this;
    }
    
};



// %%DYNOBJ section implement
// This section is auto-generated and manual changes will be lost when regenerated!!
//
// This section is for setting up the type system at run-time using global or 
// static variable constructors. For types deriving from DynI, the doGetType 
// member function is implemented. For other types, global declarations are used.
//
// Define the symbol below from -only one place- in the project implementing 
// these interfaces/classes [the library/module].

#ifdef DO_IMPLEMENT_PERSONI

// Generate type information that auto-registers on module load
DynObjType g_do_vtype_PersonI("PersonI:DynObj",PERSONI_TYPE_ID,1,sizeof(PersonI));
// DynI::doGetType implementation for: PersonI
DynObjType* PersonI::doGetType( const DynI **pself ) const {
   if(pself) *pself=(const DynI*)(const void*)this;
   return &g_do_vtype_PersonI;
}
#endif //DO_IMPLEMENT_...
//
// %%DYNOBJ section end

#endif // PERSONI_H

