#ifndef PROFFESIONI_H
#define PROFFESIONI_H

// %%DYNOBJ section general
// This section is auto-generated and manual changes will be lost when regenerated!!
#ifdef DO_IMPLEMENT_PROFESSIONI
    #define DO_IMPLEMENTING     // If app has not defined it already
#endif
#include "dynobj/DynObj.h"
//
// These are general definitions & declarations used on both the user side [main program]
// and the implementor [library] side. 

//
// --- Integer type ids for defined interfaces/classes ---
#define PROFESSIONI_TYPE_ID 0xFD0A7000

//
// --- Forward class declarations ---
class ProfessionI;

// --- For each declared class, doTypeInfo template specializations ---
// This allows creating objects from a C++ types and in run-time casts
DO_DECL_TYPE_INFO(ProfessionI,PROFESSIONI_TYPE_ID);

// %%DYNOBJ section end

#include <string.h>     // We use strcmp below


// We look at ProfessionI as an extension to another object, so we
// don't derive it from DynObj but from DynI (cannot be instantiated
// separately).
// The "inst2reg" flag tells the parser to generate code which instantiates 
// an object at registration. This helps with setting up side base class 
// information. The function bodies are there to allow instantiating thje 
// object.

// %%DYNOBJ class(DynI) flags(novcorr)
class ProfessionI : public DynI {
public:
    // DynI method
	virtual DynObjType* docall doGetType( const DynI **pself=NULL ) const;

    // A very useful interface :-).
    virtual const char* docall GetProfession( ) const { return 0; }
    virtual int docall GetExperience() const { return -1; }
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

#ifdef DO_IMPLEMENT_PROFESSIONI

// Generate type information that auto-registers on module load
DynObjType g_do_vtype_ProfessionI("ProfessionI:DynI",PROFESSIONI_TYPE_ID,((DOT_NO_VTABLE_CORR)<<16)|1,sizeof(ProfessionI));
// DynI::doGetType implementation for: ProfessionI
DynObjType* ProfessionI::doGetType( const DynI **pself ) const {
   if(pself) *pself=(const DynI*)(const void*)this;
   return &g_do_vtype_ProfessionI;
}
#endif //DO_IMPLEMENT_...
//
// %%DYNOBJ section end

#endif // PROFESSIONI_H

