// Copyright (c) 2007, Arne Steinarson
// Licensing for DynObj project - see DynObj-license.txt in this folder


#include <string.h>
#include <stdio.h>
#include <ctype.h>
 
#include "DynObjLib.h"
#include "pi/DynLib.h"
#include "DoBaseI.h"
#include "DoError.h"
#include "vt_test.h"

class DoRunTimeI;


DynObjLib::DynObjLib( const char *lib, bool do_call_init, int load_flags )
	: m_pdlib(new DynLib(lib,load_flags)), m_pdol_impl(NULL), 
	 m_name(lib),	m_implements(NULL), m_callstr(NULL),  
	  m_platform(0), m_do_traits(0), m_state(NotLoaded)
#if DO_ENABLE_OBJECT_TRACKING==1
	  ,m_ot_enable(true), m_ot_cnt(0), m_ot_size(0), m_ot_size_total(0) 
#endif 
{
	Ctor(lib,do_call_init);
}

void DynObjLib::Ctor( const char *lib, bool do_call_init ) {_CFE_;
	// Loaded OK?
	m_last_err = m_pdlib->GetLastErr();
	if( m_last_err && *m_last_err ){
		m_state = LoadFailed;
		DO_LOG_ERR(DOERR_LIB_LOAD_FAILED,m_last_err);
		return;	
	}
	m_state = Loaded;
	
	// Get addresses to functions
	m_pcfn = (CreateDynObjFn)m_pdlib->GetSymbol("CreateDynObj");
	
	// If library has SetBaseCreate, set it right away (if given to constr)
	m_pidlfn = (InitDynLibFn)m_pdlib->GetSymbol("InitDynLib");
	m_pedlfn = (ExitDynLibFn)m_pdlib->GetSymbol("ExitDynLib");
	m_pdofn = (DestroyDynObjFn)m_pdlib->GetSymbol("DestroyDynObj");

	if( !m_pcfn ){
		m_state = InitFailed;
		DO_LOG_ERR(DOERR_LIB_FAILED_INIT,"DynObjLib: Failed locating symbol 'CreateDynObj' in library" );
		return;	
	}
		
	/*
	// Get addresses to symbols - this does not work easily on Linux
	// Addresses to data in dynamic libraries is a shady area.
	const char **ppc;
	ppc = (const char**)m_pdlib->GetSymbol("g_do_call_type");
	if( ppc ) m_callstr = *ppc;
	
	ppc = (const char**)m_pdlib->GetSymbol("g_do_implements");
	if( ppc ) m_implements = ppc;

	m_platform = (int *)m_pdlib->GetSymbol("g_do_platform");
	m_do_traits = (int *)m_pdlib->GetSymbol("g_do_traits");
	*/
			
	if( do_call_init )
		InitDynLib( );
}

// A constructor for the main program, which is not really a library. But we want it 
// to look that way to DoRunTime
DynObjLib::DynObjLib( const char *name, CreateDynObjFn pcfn, 
   		             const char *implements, const char *callstr ) 
 : m_pdlib(0), m_pcfn(pcfn), m_pidlfn(NULL), m_pedlfn(NULL), 
   m_implements(implements), m_callstr(callstr), m_name(name), 
   m_state(NotLoaded), m_do_traits(0), m_platform(0)
#if DO_ENABLE_OBJECT_TRACKING==1
   ,m_ot_enable(false), m_ot_cnt(0), m_ot_size(0) 
#endif
{
	// We need the creation function and the implements string for operation
	if( m_pcfn && m_implements )
		m_state = Initialized;
}


typedef void (*DoExitFn)( );

DynObjLib::~DynObjLib( ){_CFE_;
	// See if we should call Uninitialize
	if( m_pedlfn && m_state==Initialized ){
		m_pedlfn(false);
		// This will cause the modoule to unregister with DORT
		DoExitFn doefn = (DoExitFn)FindSymbol("doExit");
		if( doefn )
			doefn();
		else
			// Not fatal but good to report
			DO_LOG("NOTE: Failed locating doExit function of the library");
	}
	m_state = NotLoaded;
	
#if DO_ENABLE_OBJECT_TRACKING==1
	// Dump any remaining objects
	if( m_ot_enable ){
		if( m_objs.Size() ){
			DO_LOG1( "DynObjLib shutdown (%s). There are non-destroyed objects.", m_pdlib->GetName() );
			for( int ix=0; ix<m_objs.Size(); ix++ ){
				DynObjType *pdt = &DORT ? DORT.GetTypeOf(*(void***)m_objs[ix]) : NULL;
				DO_LOG2("Object: %p   Type:%s", m_objs[ix], pdt?pdt->_type:"");
			}
		}
	}
#endif

	// delete library holder
	delete m_pdlib;
}


typedef DynObjLibImpl* (*doInitFn)( DoRunTimeI *pdort );

bool DynObjLib::InitFromLibStruct( DynObjLibImpl* pdol ){_CFE_;
	
	// # This is the 1st time we access memory from a loaded library 
	// It would really be good to protect it with robust exception handling.
	
	// This checks that the header of the struct is fine
	if( !pdol->_sh.IsOk(sizeof(DynObjLibImpl),0) ){
		m_state = Incompatible;
		DO_LOG_ERR(DOERR_LIB_DATA_SIZE_MISMATCH, "DynObjLib: Library implementation desciption is corrupt" );
		return false;
	}
	
	m_pdol_impl = pdol;
	
    // We want the module to point back to us, so it can be used
    // when deleting objects
    if( pdol->_module )
        pdol->_module->m_pv1 = this;
    
	// Copy values here. These are constants inside the loaded library but it's OK.
	m_callstr = pdol->_call_str;
	m_implements = pdol->_implements;
	m_do_traits = pdol->_do_traits;
	m_platform = pdol->_platform;
	
	// # Check platform match?
	if( ((DO_PLATFORM_FLAGS^m_platform)>>16)&0x3F ){
		m_state = Incompatible;
		DO_LOG_ERR(DOERR_LIB_DATA_SIZE_MISMATCH, "DynObjLib: Mismatch for primitive data type sizes (32/64 bit issue)" );
		return false;
	}
	
	// Check VTable compatibility
	if( VTableCompatible(m_do_traits)<0 ){
		m_state = Incompatible;
		DO_LOG_ERR(DOERR_LIB_VTABLE_INCOMPATIBLE,"DynObjLib: VTable traits of library not compatible with this module");
		return false;
	}
	
	return true;
}

bool DynObjLib::InitDynLib( ){_CFE_;
	if( m_state<Loaded || m_state==InitFailed ||
	    m_state==CannotInit ) return false;
	if( m_state==Initialized ) return true;

	// It would be good to read back a global data symbol in the library before 
	// calling into it, but that has proved difficult under Linux. It adds  
	// complexity, since then a library must have more custom build options.
	
	
	// # This is the 1st time we call into a loaded library 
	// It would really be good to protect it with robust exception handling.
	
	// Best to run doBaseInit first, since then module will have access
	// to DoRunTime
	doInitFn doifn = (doInitFn)FindSymbol("doBaseInit");
	if( doifn ){
		m_pdol_impl = doifn(&DORT);
		if( !m_pdol_impl ){
			// Fatal 
			m_state = CannotInit;
			DO_LOG("DynObjLib: doInit returned invalid value - could not initialize");
			return false;
		}
		else if( !InitFromLibStruct(m_pdol_impl) )
			return false;
	}

	// Is there a custom initialize library function? 
	if( m_pidlfn ){
		DynObjLibImpl *pdol_impl = m_pidlfn(&DORT);
		// Fail if we have no DynObjLibImpl by now (or two different ones) 
		if( (!pdol_impl && !m_pdol_impl) || 
			(pdol_impl && m_pdol_impl && pdol_impl!=m_pdol_impl) ){
			// Fatal 
			m_state = CannotInit;
			DO_LOG("InitDynLibFn: returned invalid value - could not initialize");
			return false;
		}
		if( !m_pdol_impl ){
			if( InitFromLibStruct(pdol_impl) )
				return false;
		}
	}
	else if( !m_pdol_impl ){
		m_state = InitFailed;
		DO_LOG_ERR(DOERR_LIB_FAILED_INIT,"DynObjLib: Have no 'InitDynLibFn' (cannot initialize)");
		return false;
	}
	
    #if DO_USE_RUNTIME==1
    // We should go through the types and register those that have VTables
    if( &DORT ){
        DynObjType *pdot;
        for( int ix=0; pdot=m_pdol_impl->_types[ix]; ix++ )
            if( pdot->_vtbl )
                DORT.RegisterType( pdot->_vtbl, pdot );
    }
    #endif // DO_USE_RUNTIME==1
    
	m_state = Initialized;
	return true;
}

bool DynObjLib::ExitDynLib( bool is_query ){_CFE_;
	if( m_state!=Initialized )
		return false;
	if( !m_pedlfn )
		return true;
	
	// Uninitialize
	bool rv = m_pedlfn(is_query);
	if( !is_query )
		// State rolls back
		m_state = Loaded;
	return rv;
}

// Read arbitrary symbol from library
void *DynObjLib::FindSymbol( const char *symb_name ){_CFE_;
	return m_state>=Loaded ? m_pdlib->GetSymbol(symb_name) : NULL;
}
   
const char* DynObjLib::GetLibFileName( ){_CFE_;
	return m_state>=Loaded ? m_pdlib->GetName() : NULL; 
}

const char* DynObjLib::GetLibName( ){_CFE_;
	return m_pdlib ? m_pdlib->GetName() : NULL;
}

// Create a DynObj
DynObj* DynObjLib::Create( const char *type, int type_id, const DynI* pdi_init, int object_size ){_CFE_;
	if( !m_pcfn || m_state!=Initialized ) return NULL;
    
	DynObj *pdo = m_pcfn(type,type_id,pdi_init,object_size);
    if( !pdo ) return NULL;
    
	DoRunTimeI *pdort = &DORT;
    // This will do any registration needed for the object
    if( !doConstruct(pdo,pdo->doGetType(NULL),true) ){
        // Failed construction
        doReleaseDestroy( pdo );
        if( pdort )
            pdort->SetLastError( -1, CharBuf(true,"DoRunTimeC: Failed doConstruct: ",type) );
        return NULL;
    }

	// If there are any errors around for a previous instances at this address, clear it.
	// (Maybe we could be more selective here, and only call for libs that don't support
	// tracking, but that might be delicate).
	if( pdort ) pdort->ClearObjectError( pdo );

#if DO_ENABLE_OBJECT_TRACKING==1    
    if( m_ot_enable && m_pdol_impl->_lib_flags&DOLI_OBJECT_TRACKING ){
        m_objs.Push(pdo);
        m_ot_cnt++;
        DynObjType *pdt = DI_GET_TYPE(pdo);
        if( pdt ){
            m_ot_size += pdt->_size;
            m_ot_size_total += pdt->_size;
        }
        else if( &DORT )
            DORT.SetLastError( -1, CharBuf(true,"DoRunTimeC: (ObjectTracker) - Failed doGetType for new object: ",type) );
    }
#endif
    
	return pdo;
}

// Delete a DynObj - Note: Best is to use DynObj::doDestroy function
bool DynObjLib::Destroy( VObj *pvo ){_CFE_;
	if( !pvo ) return true;
	if( m_state!=Initialized ) return false;
	if( !m_pdofn ){
        if( &DORT )
            DORT.SetLastError( DOERR_LIB_FAILED_DESTROY, "DynObjLib: Failed Destroy, have no 'DestroyDynObj' function" );
		return false;
	}
	
	bool b = false;
	DynObjType *dot = pvo->GetType(); 
	if( dot ){
		// Check that module matches
		if( dot->_module==m_pdol_impl->_module )
			b = m_pdofn(pvo,dot);
		else if( &DORT )
			DORT.SetLastError( DOERR_LIB_FAILED_DESTROY, "DynObjLib: Failed Destroy, wrong module" );
	}
	else if( &DORT )
		DORT.SetLastError( DOERR_LIB_FAILED_DESTROY, "DynObjLib: Failed Destroy, object type not found" );
	
	return b;
}

// Come here for object tracking
void DynObjLib::OnObjectDestroy( DynObj *pdo, DynObjType *pdt ){_CFE_;
#if DO_ENABLE_OBJECT_TRACKING==1
	if( !m_pdol_impl || !(m_pdol_impl->_lib_flags&DOLI_OBJECT_TRACKING) ){
		DO_LOG_ERR2(DOERR_LIB_TRACKER,"DynObjLib: Tracker - OnObjectDestroy - library does not support tracking: %p (%s)", pdo, pdt?pdt->_type:"" );
		return;
	}
	
    if( m_objs.Remove(pdo) ){
        if( pdt )
            m_ot_size -= pdt->_size;
    }
    else{
    	if( m_ot_enable )
    		DO_LOG_ERR2(DOERR_LIB_TRACKER,"DynObjLib: Tracker - OnObjectDestroy - object not found: %p (%s)", pdo, pdt?pdt->_type:"" );
    }
#endif
}

bool DynObjLib::EnableObjectTracking( bool enable ){_CFE_;
#if DO_ENABLE_OBJECT_TRACKING!=1
    return false;
#else
    // It's a two way thing. The library must have support for it also.
    if( !m_pdol_impl || !(m_pdol_impl->_lib_flags&DOLI_OBJECT_TRACKING) )
    	return false;
    if( m_ot_enable==enable ) 
    	return true;
    
    if( enable ){
        // Nothing to do
    }
    else{
        // Clear the records
        m_objs.Empty();
        m_ot_size = 0;
    }
    m_ot_enable = enable;
    return true;
#endif
}

const char *DynObjLib::GetError( ){ 
	return m_pdlib ? m_pdlib->GetLastErr() : NULL;
}
/*
bool DynObjLib::SetBaseCreate( CreateDynObjFn pbcr ){_CFE_;
	if( !m_pbc ) return false;
	return m_pbc(pbcr);
}
*/

int DynObjLib::Implements( const char *type ) {_CFE_;
    // # An alternative algorithm is to iterate on the types 
    // of the library and check for the DOT_USER_TYPE flag.
    
	if(!type||!*type) return -1;
	if(!m_implements || !*m_implements) return 0;
	
	// 'm_implements' is a string like this:
	//   DerivedClass:BaseI,OtherI,...
	// It can contain a ':' to inform about both class name and base interface 
	int sl = (int)strlen(type);
	const char *pc = strstr(m_implements,type);
	if( !pc ) return -1;
	if( isalnum(pc[sl]) ){
		// If we found a prefix 
		pc = strstr(pc+sl,type);
		if( !pc ) return -1;
	}
	return ((pc[sl]==':' || pc[sl]==',' || !pc[sl]) && 
	        (pc==m_implements || pc[-1]==':' || pc[-1]==',')) ? 1 : -1;
}

int DynObjLib::Implements( int type_id ){_CFE_;
    // # An alternative algorithm is to iterate on the types 
    // of the library and check for the DOT_USER_TYPE flag.
    
	if(!m_implements || !*m_implements) return 0;
	
	char buf[16];
	sprintf(buf,"%X",type_id);
	if( Implements(buf)==1 ) return 1;
	sprintf(buf,"%x",type_id);
	if( Implements(buf)==1 ) return 1;
	sprintf(buf,"%08X",type_id);
	if( Implements(buf)==1 ) return 1;
	sprintf(buf,"%08x",type_id);
	if( Implements(buf)==1 ) return 1;
	return -1;
}

const char *DynObjLib::GetCallConv( ){_CFE_;
	return m_callstr;
}

void* DynObjLib::GetBase( ){_CFE_;
	if( !m_pdlib ) return NULL;
	// Try get base using two symbols, one of them (at least) should exist
	void *pv = m_pdlib->GetBase("doBaseInit");
	if( !pv ) pv = m_pdlib->GetBase("InitDynLib");
	return pv;
}

