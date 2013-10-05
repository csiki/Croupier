// Copyright (c) 2007, Arne Steinarson
// Licensing for DynObj project - see DynObj-license.txt in this folder


#define DO_IMPLEMENT_NAMEDREF
#define DO_IMPLEMENTING 
#include "NamedRef.h"
#include "DoBaseI.h"
#include "DoError.h"
#include "DynStr.h"
#include "DynObjHolder.hpp"


NamedRef::~NamedRef(){_CFE_; m_refs.DeleteAll(); }

DynI* NamedRef::GetByName( const char *name, int type_id ){_CFE_;
	int ix = m_names.FindIndexOf(name);
	if( ix<0 || !m_refs[ix]->Get() ) return NULL;
	if( type_id && !m_refs[ix]->Get()->CanBeA(type_id) ) return NULL;
	return m_refs[ix]->Get();
}

DynI* NamedRef::GetByIndex( int index, DynStr &ds, int type_id ){_CFE_;
	for( int ix=0; ix<m_refs.Size(); ix++ ){
		if( !type_id || (m_refs[ix]->Get() && m_refs[ix]->Get()->CanBeA(type_id)) ){
			if( !index ){
				if( ds.IsOk() ) ds = m_names[ix];
				return m_refs[ix]->Get();
			}
			index--;
		}
	}
	return NULL;
}

bool NamedRef::Set( const char *name, DynI* pdi ){_CFE_;
	// Must implement NotifierI
	if( !name ) return false;
	if( pdi && !(doIsStatic(pdi) || pdi->CanBeA(NOTIFIERI_TYPE_ID)) ){
		SetError("NamedRef: NotifierI not supported on refered object",DOERR_CLASS_NAMEDREF);
		return false;
	}
	int ix = m_names.FindIndexOf(name);
	if( ix==-1 ){
		m_names.Push(name);
		m_refs.Push( new DynObjHolder<DynI>(pdi,hmAutoNoOwn) );
	}
    else
        m_refs[ix]->Assign(pdi,hmAutoNoOwn);
	return true;
}
	
bool NamedRef::Remove( DynI* pdi ){_CFE_;
	for( int ix=0; ix<m_names.Size(); ix++ ){
		if( m_refs[ix]->Get()==pdi ){
			m_names.RemoveIndex(ix);
            delete m_refs[ix];
			m_refs.RemoveIndex(ix);
			return true;
		}
	}
	return false;
}	

