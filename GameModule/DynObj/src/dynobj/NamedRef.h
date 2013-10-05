// Copyright (c) 2007, Arne Steinarson
// Licensing for DynObj project - see DynObj-license.txt in this folder

#ifndef NAMED_REF_H
#define NAMED_REF_H


// %%DYNOBJ section general
// This section is auto-generated and manual changes will be lost when regenerated!!
#ifdef DO_IMPLEMENT_NAMEDREF
    #define DO_IMPLEMENTING     // If app has not defined it already
#endif
#include "dynobj/DynObj.h"

// --- Integer type ids for defined interfaces/classes ---
#define NAMEDREF_TYPE_ID 0xA99E9000

// --- Forward class declarations ---
class NamedRef;

// --- For each declared class, doTypeInfo template specializations ---
DO_DECL_TYPE_INFO(NamedRef,NAMEDREF_TYPE_ID);

// %%DYNOBJ section end


//#include "DoBase.hpp"
#include "DoBaseI.h"
#include "DoNotifiable.hpp"
#include "utils/StringBuffer.h"

template<class T>
class DynObjHolder;

// # NamedRefI would benefit from a mutex, or using a linked list
// This would bring thread safety

// %%DYNOBJ class(dyni) flags(inst2reg) 
class NamedRef : public NamedRefI {
public:
	~NamedRef();
	
	virtual DynObjType* docall doGetType( const DynI **pself=0 ) const;
	
	virtual DynI* docall GetByName( const char *name, int type_id=0 );
	virtual DynI* docall GetByIndex( int index, DynStr &ds, int type_id=0 );
	virtual bool  docall Set( const char *name, DynI* pdi );
	virtual bool  docall Remove( DynI* pdi );

protected:	
	ExpArrP<DynObjHolder<DynI>*> m_refs;
	StrBuf m_names;
};


// %%DYNOBJ section implement
// This section is auto-generated and manual changes will be lost when regenerated!!
#ifdef DO_IMPLEMENT_NAMEDREF

// Generate type information that auto-registers on module load
DynObjTypeI2R<NamedRef,NamedRefI,false> g_do_vtype_NamedRef("NamedRef:NamedRefI",NAMEDREF_TYPE_ID,1);
DynObjType* NamedRef::doGetType( const DynI **pself ) const {
   if(pself) *pself=(const DynI*)(const void*)this;
   return &g_do_vtype_NamedRef;
}
#endif //DO_IMPLEMENT_...
// %%DYNOBJ section end

#endif // NAMED_REF_H


