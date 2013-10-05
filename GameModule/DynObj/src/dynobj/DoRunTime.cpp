// Copyright (c) 2007, Arne Steinarson
// Licensing for DynObj project - see DynObj-license.txt in this folder

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#define EA_ASSERT assert
#define DO_IMPLEMENTING
#define DO_IMPLEMENT_DORUNTIME
#include "DoRunTime.h"

//#include "DoBase.hpp"
#include "DynObjLib.h"
#include "DynStr.h"

// Owner of mutex - Diagnostics in case of deadlock
int g_mut_own_thid;

#include "pi/Thread.h"

class MutexC : public MutexI {
public:
    PI_MUTEX_HANDLE m_mh;
    int m_cnt;
	MutexC() { 
        m_mh = piMutexCreate(true);
        m_cnt = 0;
    }
	
	virtual bool docall Lock() {_CFE_; 
        bool b = !piMutexLock(m_mh); 
        if( b && !m_cnt++ ) g_mut_own_thid = piGetThreadId(); 
        return b;
    }
	virtual bool docall Unlock(){_CFE_; 
		bool b = !piMutexUnlock(m_mh);
		if( b && !--m_cnt ) g_mut_own_thid = -1;
        return b;
	}
    
	virtual bool docall TryLock() {_CFE_; 
		bool b = !piMutexTryLock(m_mh); 
		if( b && !m_cnt++ ) g_mut_own_thid = piGetThreadId(); 
		return b;
	}
    
	bool docall IsOk() {_CFE_; return m_mh!=NULL; }
};

/*
#include <wx/thread.h>
class MutexC : public MutexI {
public:
	MutexC() : m_mut(wxMUTEX_RECURSIVE) { }

	bool IsOk(){_CFE_; return true; }
	
	virtual bool docall Lock(){_CFE_;
	   bool b = m_mut.Lock()==wxMUTEX_NO_ERROR;
	   if( b )
		   g_mut_own_thid = wxThread::GetCurrentId(); 
		return b;
	}
	virtual bool docall Unlock(){_CFE_; 
	   int prev_thid = g_mut_own_thid;
	   g_mut_own_thid = -1;
		bool b = m_mut.Unlock()==wxMUTEX_NO_ERROR; 
		if( !b )
			g_mut_own_thid = prev_thid;
	   return b;
	}
	virtual bool docall TryLock() {_CFE_; 
		bool b = m_mut.TryLock()==wxMUTEX_NO_ERROR; 
		if( b )
		   g_mut_own_thid = wxThread::GetCurrentId(); 
		return b;
	}
	
	wxMutex m_mut;
};
*/

// Can set another allocator using DoRunTimeI::SetMalloc
class MallocC : public MallocI {
	virtual void* docall malloc( int size ){_CFE_; 
		return malloc(size); 
	}
	virtual void  docall free( void *pv ){_CFE_; 
		free(pv); 
	}
};


class TextIoC : public TextIoI {
	virtual int  docall puts(const char *ps){_CFE_; 
		return (int)fputs(ps,stdout); 
	}
	virtual int  docall puterr(const char *ps){_CFE_; 
		return (int)fputs(ps,stderr); 
	}
	virtual int  docall gets(DynStr& ds, bool sync){_CFE_;
		int ch;
		int cnt = 0;
		while( ch=fgetc(stdin) ) {
			if( ch=='\n' || ch==EOF ) break;
			ds += ch;
			cnt++;
		}
		return (int)cnt; 
	}
};


#if (defined(__WXDEBUG__) && wxUSE_MEMORY_TRACING) || wxUSE_DEBUG_CONTEXT==1 
#include <wx/memory.h>
// To run our global destructors before memory dump
wxDebugContextDumpDelayCounter g_debug_do_rt;
#endif 


DoRunTimeC *g_pdort;
MutexC 	  *g_pdort_mut;
int        g_dort_ok;	// Set to 1 at end of constructor, -1 at end of destructor

MutexI *GetDortMutex(){_CFE_; 
	// Only create mutex before run time has started
	// This gives access to mutex before all global constructors have been run
	if( !g_pdort_mut && !g_dort_ok ){
		g_pdort_mut = new MutexC;
        // Check that mutex creation went OK
        DO_ASSERT_MSG( g_pdort_mut->IsOk(), "GetDortMutex - Not OK" );
    }
	return g_pdort_mut;
}

// Proto from DynObj.cpp
void doSetRunTime( DoRunTimeI* pdort );

// This ensures that we can have an instance before all global constructors are run
DoRunTimeC &DoRunTimeC::GetInstance(){_CFE_;
	if( !g_pdort && !g_dort_ok ){
		g_pdort = new DoRunTimeC;
		doSetRunTime( g_pdort );
	}
	return *g_pdort;
}

DoRunTimeI *MainGetDort() {_CFE_;
	return &DoRunTimeC::GetInstance();
}

DoRunTimeC &MainGetDortC() {_CFE_;
	return DoRunTimeC::GetInstance();
}


// Assures that the runtime goes down on application shutdown
class DoRunTimeGlobConstr{
public:
	~DoRunTimeGlobConstr( ){ 
		if( g_pdort ){
			doSetRunTime( NULL );
			g_pdort->doDestroy();
			delete g_pdort_mut;
			g_pdort_mut = NULL;
		    g_pdort = NULL;
		}
	}
} g_dort_glob_constr;


DoRunTimeC::DoRunTimeC(){_CFE_;
	// Set up interface pointers
	static MallocC st_malloc;
	static TextIoC st_tio;
	m_pmut = GetDortMutex();
	m_pmalloc = &st_malloc;
	m_ptio = &st_tio;
	g_dort_ok = 1;
    m_lib_last = NULL;
    m_lib_cache_ix = 0;
	m_tmp_disable = 0;
}

void DoRunTimeC::Dtor(){_CFE_;
	MutexLocker lock(g_pdort_mut);
	g_dort_ok = -1;
    m_libs.DeleteAll();
}

bool DoRunTimeC::IsOk() {_CFE_;
    return this && g_dort_ok>0;
}

void *DoRunTimeC::doGetObj( const char *type ) {_CFE_;
	if( g_dort_ok<=0 ) return NULL;

	// So that we can return this interface before type init finished	
	if( !strcmp(type,"NamedRefI") ) return static_cast<NamedRefI*>(this);

	// Owned objects?
	if( !strcmp(type,"MutexI") ) return m_pmut;
	if( !strcmp(type,"MallocI") ) return m_pmalloc;
	if( !strcmp(type,"TextIoI") ) return m_ptio;
	
	return DoRunTimeI::doGetObj(type);
}
	
DynObj *DoRunTimeC::Create( const char *lib, const char *type, int type_id, const DynI* pdi_init, int obj_size ){_CFE_;
	if( g_dort_ok<=0 || m_tmp_disable>0 ) return NULL;
	
	// Protect object creation with mutex (only 1 object created at a time)
    MutexLocker lock(g_pdort_mut);
    DynObj *pdo = NULL;
    bool found_lib = false;
    for( EapIter<DynObjLib*> it(m_libs); it(); it++ ){
		if( (!lib || (it()->GetLibName() && !strcmp(lib,it()->GetLibName()))) && 
		    it()->Implements(type)>0 ){
            found_lib = true;
			if( pdo=it()->Create(type,type_id,pdi_init,obj_size) ){
                m_lib_last = it();
				return pdo;
            }
		}
	}
	if( found_lib )
		SetLastError(-2, CharBuf(true,"DoRunTimeC: Library for type found but failed Create: ", type) );
	else
		SetLastError(-3, CharBuf(true,"DoRunTimeC: No matching library for type found: ", type) );
	return NULL;
}
	
void DoRunTimeC::TempDisableCreate( bool disable ){_CFE_;
	m_tmp_disable += (disable ? 1 : -1); 
}

void DoRunTimeC::OnObjectDestroy( DynObj *pdo ){_CFE_;
    if( !pdo ) return;

	// Object going out of scope, don't keep error log for it
	ClearObjectError( pdo );

    // Track back to library that created object
    DynObjType *pdt = DI_GET_TYPE(pdo);
    if( pdt && pdt->_module->m_pv1 )
        ((DynObjLib*)pdt->_module->m_pv1)->OnObjectDestroy( pdo, pdt );
}


int DoRunTimeC::GetLastErrCode( int thid ){_CFE_;
	if( g_dort_ok<=0 ) return false;
	MutexLocker lock(g_pdort_mut);
	if( thid==-1 )
		thid = GetThreadId();
		
	DOError *pe = m_errors.Find(thid);
	return pe ? pe->m_err_code : 0;
}

bool DoRunTimeC::GetLastErrStr( DynStr &ds, int thid ){_CFE_;
	if( g_dort_ok<=0 ) return false;
	MutexLocker lock(g_pdort_mut);
	if( thid==-1 )
		thid = GetThreadId();
		
	DOError *pe = m_errors.Find(thid);
	if( !pe ) return false;
	ds = pe->m_err_str;
	return true;
}

bool DoRunTimeC::SetLastError( int err_code, const char *err_msg ){_CFE_;
	if( g_dort_ok<=0 ) return false;
	MutexLocker lock(g_pdort_mut);
	int thid = GetThreadId();
	
	// Output to stderr
	if( err_code ){
		if( !err_msg || !*err_msg ) err_msg=" ";
		if( m_ptio ){
			CharBuf cb;
			int el = (int)strlen(err_msg);
			cb.Resize(el+32);
			if( cb.Get() ){
				// \n terminate
				sprintf( (char*)cb.Get(), "(Error: 0x%X) - %s%c", err_code, err_msg, err_msg[el-1]=='\n'?' ':'\n' );
				m_ptio->puterr(cb);
			}
			else{
				m_ptio->puterr("DoRunTimeC: Mem alloc failure\n");
				m_ptio->puterr(err_msg);
			}
		}
	}
	
	DOError *pe = m_errors.Find(thid);
    bool did_set = false;
	if( !pe ){
        if( m_oe_avail.Size() )
            pe = m_oe_avail.Pop();
        else{
            pe = new DOError( GetThreadId(), err_code, err_msg );
            if( !pe ) return false;
            m_oe_allocated.Push(pe);
            did_set = true;
        }
		m_errors.Push(pe);
	}
	if( !did_set ){
		pe->m_err_code = err_code;
		pe->m_err_str = err_msg;
	}
    
	// Check that memory alloc went OK
	if( !pe->m_err_str || (err_msg && strcmp(pe->m_err_str,err_msg)) )
		return false;
	
	return true;
}

int DoRunTimeC::GetThreadId( ){_CFE_;
    return piGetThreadId();
}

bool DoRunTimeC::RegisterModule( DoModuleC *pmi ){_CFE_;
	if( g_dort_ok<=0 ) return false;
	MutexLocker lock(g_pdort_mut);
    
	if( !pmi || m_modules.Has(pmi) )
		return false;
	m_modules.Push(pmi);
	return true;
}

bool DoRunTimeC::UnRegisterModule( DoModuleC *pmi ){_CFE_;
	if( g_dort_ok<=0 ) return false;
	MutexLocker lock(g_pdort_mut);
    
	int ix = m_modules.IndexOf(pmi);
	if( ix<0 ) return false;
	m_modules.RemoveIndexUnordered(ix);
	return true;
}

bool DoRunTimeC::RegisterType( void **vtbl, DynObjType *pdt ){_CFE_;
	if( g_dort_ok<=0 ) return false;
	if( vtbl && pdt ){
		MutexLocker lock(g_pdort_mut);
		if( !m_types.Lookup(vtbl) ){
			m_types.Insert(vtbl,pdt);
			return true;
		}
	}
	return false;
}

bool DoRunTimeC::UnRegisterType( void **vtbl ){_CFE_;
	if( g_dort_ok<=0 ) return false;
	if( vtbl ){
		MutexLocker lock(g_pdort_mut);
		return m_types.Remove(vtbl);
	}
	return false;
}

DynObjType* DoRunTimeC::GetTypeOf( void **vtbl ){_CFE_;
	if( g_dort_ok<=0 ) return NULL;
	return m_types.Lookup(vtbl);
}

bool DoRunTimeC::AddDOLib( DynObjLib *pdol ){_CFE_;
	if( g_dort_ok<=0 || !pdol ) return false;
	
	MutexLocker lock(g_pdort_mut);
	m_libs.Push(pdol);
    
    // Register types in library (if not done yet)
    DynObjType **pdot = pdol->m_pdol_impl->_types;
    DynObjType *dot;
    for( int ix=0; dot=pdot[ix]; ix++ )
		if( dot->_vtbl && !m_types.Lookup(dot->_vtbl) )
			m_types.Insert(dot->_vtbl,dot);
    
	return true;
}

bool DoRunTimeC::RemoveDOLib( const DynObjLib *pdol ){_CFE_;
	if( g_dort_ok<=0 ) return false;
	
	MutexLocker lock(g_pdort_mut);
    
    // Unregister types in library (if not done yet)
    DynObjType **pdot = pdol->m_pdol_impl->_types;
    DynObjType *dot;
    for( int ix=0; dot=pdot[ix]; ix++ )
		if( dot->_vtbl && m_types.Lookup(dot->_vtbl) )
			m_types.Remove(dot->_vtbl);
    
    // Remove among cached libraries also
    for( int ix=0; ix<m_type_libs.Size(); ix++ )
        if( m_type_libs[ix]==pdol )
            m_type_libs[ix] = NULL;
    
    // Remove from array of libraries
	return m_libs.Remove((DynObjLib*)pdol);
}

const char *DoRunTimeC::LoadLib( const char *lib_name ){_CFE_;
	if( g_dort_ok<=0 ) return false;
	
    DynObjLib *pdol = new DynObjLib(lib_name);
	if( !pdol )
		return "<Failed new>";
    if( !pdol->IsOk() ){
		static CharBuf cb_err;
		cb_err = pdol->GetError();
        delete pdol;
        return cb_err;
    }
    AddDOLib(pdol);
	return NULL;
}

DynObjLib* DoRunTimeC::GetLib( const char *lib ){
	return m_libs.Find(lib);
}


bool DoRunTimeC::SetObjectError( const VObj *pvo, const char *err_str, int err_code ){_CFE_;
	if( g_dort_ok<=0 ) return false;
	MutexLocker lock(g_pdort_mut);
    
    DOError *pdoe = m_obj_errors.Lookup(pvo);
    if( !pdoe ) {
        if( m_oe_avail.Size() ){
            pdoe = m_oe_avail.Pop();
            m_obj_errors.Insert(pvo,pdoe);
        }
        else {
            pdoe = new DOError(GetThreadId(),err_code,err_str);
            if( !pdoe ) return false;
            m_oe_allocated.Push(pdoe);
            m_obj_errors.Insert(pvo,pdoe);
            return true;
        }
    }
    else{
        pdoe->m_thid = GetThreadId();
        pdoe->m_err_code = err_code;
        pdoe->m_err_str = err_str;
    }
    return true;
}

bool DoRunTimeC::ClearObjectError( const VObj *pvo ) {_CFE_;
	if( g_dort_ok<=0 ) return false;
	MutexLocker lock(g_pdort_mut);
    
    DOError *pdoe = m_obj_errors.Lookup(pvo);
    if( !pdoe ) return true;
    
    m_obj_errors.Remove(pvo);
    // Keep unallocated error
    m_oe_avail.Push(pdoe);
    
    return true;
}

const char *DoRunTimeC::GetObjectError( const VObj *pvo, int *perr_code ) {_CFE_;
	if( g_dort_ok<=0 ) return NULL;
	MutexLocker lock(g_pdort_mut);
    
    DOError *pdoe = m_obj_errors.Lookup(pvo);
    if( !pdoe ){
        if( perr_code ) *perr_code = 0;
        return NULL;
    }
    if( perr_code ) *perr_code = pdoe->m_err_code;
    return pdoe->m_err_str;
}

