// Copyright (c) 2007, Arne Steinarson
// Licensing for DynObj project - see DynObj-license.txt in this folder


#include <string.h>
#include <stdio.h>	// ## Debug
#include <assert.h>	// ## Debug

#define DO_IMPLEMENTING 
#define DO_IMPLEMENT_DYNOBJ
#define DO_IMPLEMENT_DOBASEI 
#define DO_IMPLEMENT_DOBASE 
#define DO_IMPLEMENT_DORTINT
#define DO_IMPLEMENT_DODYNSHARED

#include "utils/ExpArr.hpp"

#include "DynObj.h"
#include "DynObjMethods.h"
#include "DoError.h"
#include "DoBaseI.h" 
#include "DoBase.hpp" 
#include "DortInt.h" 
#include "vt_test.h"

#if DO_USE_DYNSHARED==1
    #include "DoDynShared.hpp"
#endif 

#if DO_ENABLE_VTABLE_DISPATCH==1
    #include "pi/SpinLock.h"
#endif 

#if DO_USE_RUNTIME==1
    #include "DORT.h"
	static DoRunTimeI *g_dort;
    
    #ifdef DO_MAIN
	DoRunTimeI *MainGetDort();
	// DoRunTime calls here to set the instance
	void doSetRunTime( DoRunTimeI* pdort ){
		g_dort = pdort;
	}
    #endif
    
    DoRunTimeI &doGetRunTime(){ 
		#ifdef DO_MAIN
    	if( !g_dort )
    		MainGetDort();	// This will trigger instantiation if not done yet
		#endif
    	return *g_dort; 
    }
	/*
    #if defined(DO_MODULE)
        // If in a module using DoRunTime, declare the global var
        DoRunTimeI *g_dort;
        DoRunTimeI &ModuleGetDort(){ return *g_dort; }
    #endif
    */
#endif


//
// Globals 
//
// Symbols exported to library loader

//int g_do_platform = DO_PLATFORM;

#ifdef DO_IMPLEMENTED_TYPES
    static const char *g_do_implements = DO_IMPLEMENTED_TYPES;
#else
    static const char *g_do_implements;
#endif 

// For building implemented type variable
static const char *g_do_allocated_str;

// To extract a reference to the ponter
struct ExpArrPDoTypes : public ExpArrP<DynObjType*> {
    DynObjType**& GetBasePointerRef( ) {_CFE_; 
        return this->ta;
    }
};

// Used during global construction, use pointers and Get function
// These could be more efficient data types 
//ExpArrP<DynObjType*>  *g_do_types;
static ExpArrPDoTypes        *g_do_types;
static ExpArrP<DynObjType*>  *g_do_types_unres;	// Put types with unresolved base class here
static ExpArrPOwn<MIBSideType*> *g_do_mibst;	

// They might be the same in various libs, but that doesn't really matter
//static int  g_do_side_type_id = DO_MBI_SIDE_1ST_ID; 
static bool g_do_init;
static int g_do_base_init;		// Signal not to notify DynObj lib on object destruction

ExpArrP<DynObjType*> &doGetTypes(){_CFE_;
	if( !g_do_types )
		g_do_types = new ExpArrPDoTypes; //ExpArrP<DynObjType*>;
	return *g_do_types;
}

ExpArrP<DynObjType*> &doGetTypesUnres(){_CFE_;
	if( !g_do_types_unres )
		g_do_types_unres = new ExpArrP<DynObjType*>;
	return *g_do_types_unres;
}

ExpArrP<MIBSideType*> &DoGetMibst(){_CFE_;
	if( !g_do_mibst )
		g_do_mibst = new ExpArrPOwn<MIBSideType*>;
	return *g_do_mibst;
}

void DynObjType_BpDummy(){_CFE_;
	int z=3;
}


// For working with VTables
#if DO_ENABLE_VTABLE_CORR==1
    #include "DoVTableInfo.hpp" 
    static ExpArrPOwn<VTableInfo*> g_do_vtbls;
#endif // DO_ENABLE_VTABLE_CORR


// The DoModuleC interface (DoRunTimeI making calls to us)
class DoModule : public DoModuleC {
	virtual DynObjType* docall FindType( void** vtbl ){_CFE_;
		return doGetTypes().Find(vtbl);
	}
	virtual DynObjType* docall FindType( const char *type ){_CFE_;
		return doGetTypes().Find(type);
	}
	virtual DynObjType* docall FindType( int type_id ){_CFE_;
		return doGetTypes().Find(type_id);
	}
};

static DoModule g_do_mod;


// For closing down module
extern "C" SYM_EXPORT void doExit( );

// Module construction
class DynObj_ModConstr {
public:
	DynObj_ModConstr() {
		// No need to do anything, vt_test.cpp runs the class/VFunc tests automatically 
	}
	
	~DynObj_ModConstr(){
		// Unregister all types/VTables with DoRunTime
		// For main program, no point in doing this, since we might interfere
		// with shutdown, and all types are being deleted anyway.
		DO_LOG("DynObj.cpp module unload");
		doExit( );
		delete [] g_do_allocated_str;
		
		delete g_do_types;
		delete g_do_types_unres;
		delete g_do_mibst;
		g_do_types = NULL;
		g_do_types_unres = NULL;
		g_do_mibst = NULL;
	}
} g_do_mod_constr;


// Make type available to library users in g_do_implements
bool doAddImplemented( const char *type, int type_id ){_CFE_;
#if defined(DO_DEBUG) 
    //DO_TRACE1("doAddImplemented %s entry\n", type?type:"??" );
#endif
	if( !type || (type_id>0 && type_id<VOBJ_TYPE_ID) ){
		DO_LOG_ERR(DOERR_INCORRECT_TYPE_ID,"doAddImplemented: No type give or too low type_id");
		return false;
	}
	char buf[32];
	int sl = (int)((g_do_implements?strlen(g_do_implements):0) + strlen(type) + 16);
	char *pc = new char[sl];
	if( !pc ){
		DO_LOG_ERR(DOERR_NO_MEMORY,"doAddImplemented: No mem (new failed)");
		return false;
	}
	
	// Build new type string
	if( g_do_implements && *g_do_implements ){
		strcpy(pc,g_do_implements);
		strcat(pc,",");
	}
	else *pc = 0;
	strcat(pc,type);
	strcat(pc,",");
	sprintf(buf,"0x%08X",type_id);
	strcat(pc,buf);
	delete [] g_do_allocated_str;
	g_do_allocated_str = pc;
	g_do_implements = pc;
	
#if defined(DO_DEBUG) 
    DO_TRACE1( "g_do_implements: %s \n", g_do_implements );
    /*
    int mod = -1;
	#ifdef DO_MODULE
	    mod = 1;
	#elif DO_MAIN
	    mod=0;
	#endif
    printf("g_do_implements: %x &:%x   module: %d \n", g_do_implements, &g_do_implements, mod );
    */
#endif
    
	return true;
}

const char *doGetImplemented( ){_CFE_;
    return g_do_implements;
}

//
// Implementation, VObj
//
DynObjType* VObj::GetType( ) const {
	return ::doGetType(this);
}

/*
void* VObj::GetObj( const char *type, DynObjType *pdt  ) {_CFE_;
	return ::doGetObj(this,pdt,type);
}
	
VObj* VObj::GetVObj( const char *type, DynObjType *pdt ) {_CFE_;
	DynObjType *pdt_found;
	void* pobj = ::doGetObj(this,pdt,type,doCastCross,&pdt_found);
	if( !pobj ) return NULL;
	if( pobj==this ) return this;
	return pdt_found->IsA(VOBJ_TYPE_ID) ? reinterpret_cast<VObj*>(pobj) : NULL;
}
   
DynI* VObj::GetDynI( const char *type, DynObjType *pdt ) {_CFE_;
	DynObjType *pdt_found;
	void* pobj = ::doGetObj(this,pdt,type,doCastCross,&pdt_found);
	if( !pobj ) return NULL;
	return pdt_found->IsA(DYNI_TYPE_ID) ? reinterpret_cast<DynI*>(pobj) : NULL;
}
   
void* VObj::GetObj( int type_id, DynObjType *pdt ) {_CFE_;
	return ::doGetObj(this,pdt,type_id,doCastCross);
}

VObj* VObj::GetVObj( int type_id, DynObjType *pdt ) {_CFE_;
	DynObjType *pdt_found;
	void* pobj = ::doGetObj(this,pdt,type_id,doCastCross,&pdt_found);
	if( !pobj ) return NULL;
	if( pobj==this ) return this;
	return pdt_found->IsA(VOBJ_TYPE_ID) ? reinterpret_cast<VObj*>(pobj) : NULL;
}

DynI* VObj::GetDynI( int type_id, DynObjType *pdt ) {_CFE_;
	DynObjType *pdt_found;
	void* pobj = ::doGetObj(this,pdt,type_id,doCastCross,&pdt_found);
	if( !pobj ) return NULL;
	return pdt_found->IsA(DYNI_TYPE_ID) ? reinterpret_cast<DynI*>(pobj) : NULL;
}
*/

bool VObj::IsA( const char *type, DynObjType *pdt ) const  {
	if( !this || !type ) return false;
	if( !pdt && !(pdt = ::doGetType(this)) )
        return false;
	//return pdt->IsA(type);
    DynObjType *dot_isa = doFindType(type);
	return dot_isa ? pdt->IsA(dot_isa->_type_id) : false;
}

bool VObj::IsA( int type_id, DynObjType *pdt ) const {
	if( !this || !type_id ) return false;
	if( !pdt && !(pdt = ::doGetType(this)) )
        return false;
	return pdt->IsA(type_id);
}

/*
bool VObj::CanBeA( const char *type, DynObjType *pdt ){_CFE_;
	if( !this || !type ) return false;
	if( !pdt && !(pdt = ::doGetType(this)) )
        return false;
    DynObjType *dot_isa = doFindType(type);
	return dot_isa ? pdt->CanBeA(dot_isa->_type_id) : false;
}
*/

bool VObj::CanBeA( int type_id, DynObjType *pdt ) const {
	if( !this || !type_id ) return false;
	if( !pdt && !(pdt=::doGetType(this)) )
        return false;
	return pdt->CanBeA(type_id);
}



const char* VObj::GetError( int *perr_code ) const  {
#if DO_USE_RUNTIME==1
    if( &DORT )
        return DORT.GetObjectError( this, perr_code );
#endif
 	return NULL;
}

void VObj::ClearError(  ){_CFE_;
#if DO_USE_RUNTIME==1
    if( &DORT )
        bool rv = DORT.ClearObjectError( this );
#endif        
}

bool VObj::SetError( const char *err_str, int err_code ) const {
    bool rv = false;
#if DO_USE_RUNTIME==1
    if( &DORT )
        rv = DORT.SetObjectError( this, err_str, err_code );
#else
    DO_LOG_ERR( err_code, err_str );
#endif        
    return rv;
}



//
// Implementation, DynI
//

// Can return a pointer to any object it knows it contains.
// This uses the type information stored in DynObjType and keeps track of
// inherited bases and side base classes. 
// Note: When overriding this function in DynI derived classes, always call 
// this base version if you cannot resolve the type in the customized function.

void* doGetObj( DynI *pdi, const char *type, doCastAlgo algo, DynObjType **pdt_found, bool inernal_from_dyni );

void* DynI::doGetObj( const char *type ) {_CFE_;
	return  ::doGetObj(this,type,doCastCross,NULL,true);
}


const char* DynI::doGetError( int *perr_code ) const {
    return VObj::GetError( perr_code );
}

void DynI::doClearError(  ){_CFE_;
    return VObj::ClearError( );
}


//void DynI::doSetError( const char *err_str, int err_code ){
//    return VObj::SetError( err_str, err_code );
//}

//
// Implementation, DynObj
//
DynObj::~DynObj(){_CFE_;
#if DO_USE_RUNTIME==1 
    // Tracking is a client feature, not a library one, so we shouldn't 
    // disable it from here.
    if( &DORT && g_do_base_init>=0 ){
    	DynObjType *dot_rt = DORT.doGetType();
        DortInternalI *pdorti = (DortInternalI*)::doGetObj( &DORT, DORTINTERNALI_TYPE_ID, dot_rt );
        if( pdorti )
            // Let DORT do object tracking
            pdorti->OnObjectDestroy( this );
    }
#endif
}


#if DO_ENABLE_VTABLE_CORR==1
// A class to hold the method for retrieving type info for side classes 
// as in C[A]:C in A : public B, public C
// C[A] is a distinct type, and we need it to return its own type (not
// its base type)
class DynObjSideType : public DynObj {
public:
	virtual DynObjType* docall doGetType( const DynI **pself ) const {
		if( !this ) return NULL;
		void **vtbl = *(void***)this;
		//return doGetTypes().Find(vtbl);
		VTableInfo *vtp = g_do_vtbls.Find(vtbl);
		return vtp ? vtp->pdt : NULL;
	}
	virtual void docall doDestroy() {_CFE_;
		// Do nothing at all. 
	}
};
#endif // DO_ENABLE_VTABLE_CORR


DynObjType *doFindType( const char *type ){_CFE_;
	return doGetTypes().Find(type);
}

DynObjType *doFindType( int type_id ){_CFE_;
	return doGetTypes().Find(type_id);
}

// This is an early init, while global construction still happens
void doEarlyInit( DoRunTimeI *pdort=NULL ){
	if( !g_do_init ){
#if DO_USE_RUNTIME==1
	#ifdef DO_MODULE
		if( pdort && !&DORT ){
			g_dort = pdort;
            DO_ASSERT_MSG( &DORT, "doEarlyInit - No DoRunTimeI available" );
        }
	#endif
		if( &DORT ){
			g_do_init = true;
			DORT.RegisterModule( &g_do_mod );
	#ifdef DO_MAIN  
            // Not stricty necessary
            DORT.RegisterType( *(void***)&g_do_mod, g_do_mod.doGetType(NULL) );
    #endif
		}
#else
		g_do_init = true;
#endif
	}
}

// Constructors for DynObjLibImpl 
#include "pi/compiler.h"

DynObjType **g_do_dummy_types;
DynObjLibImpl::DynObjLibImpl(const char *cs, const char *impl, int dt, int pf) 
    : _sh(sizeof(DynObjLibImpl),0,0), _call_str(cs), _implements(impl), 
      _do_traits(dt), _platform(pf),
      _lib_name(DO_LIB_NAME),
      _lib_descr(DO_LIB_DESCR),
      _lib_author(DO_LIB_AUTHOR),
      _lib_major_ver(DO_LIB_MAJOR_VERSION), 
      _lib_minor_ver(DO_LIB_MINOR_VERSION),
      _compiler(PI_COMPILER),
      _comp_major_ver(PI_COMPILER_VERSION), 
      _comp_minor_ver(PI_COMPILER_VERSION_MINOR),
      _types(g_do_dummy_types), _module(0),
      _lib_flags( 0 
                // Setup flags for the library
#if DO_USE_RUNTIME==1
                |DOLI_USES_RUNTIME
                // Set this flag by default. It is really a client decision.
                // DynObj destructor by default calls back to DORT if DORT is available.
                |DOLI_OBJECT_TRACKING
#endif          
                ),
      _dynobj_major_ver(DYNOBJ_MAJOR_VERSION),
      _dynobj_minor_ver(DYNOBJ_MINOR_VERSION)
      {_CFE_; }
//DynObjLibImpl::DynObjLibImpl() : _sh(sizeof(DynObjLibImpl),0,0), _call_str(0), _implements(0), _do_traits(0), _platform(0) { }    
    
DynObjLibImpl g_do_lib_impl(DO_CALLSTR,NULL,0,DO_PLATFORM_FLAGS);


// Called from outside when library has been loaded (usually by DynObjLib)
extern "C" SYM_EXPORT DynObjLibImpl* doBaseInit( DoRunTimeI *pdort ){
    // Make sure internally setup (will usually be)
    if(pdort) doEarlyInit( pdort );
    
    g_do_base_init = -1;
#if DO_USE_RUNTIME==1
    // While instantiating temp objects, stop further objects from being created
    DortInternalI *pdorti = (DortInternalI*)::doGetObj( pdort, DORTINTERNALI_TYPE_ID, doCastCross );
    if( pdorti )
        pdorti->TempDisableCreate( true );
#endif        
    
    // Go over types and see if we can run doConstruct on them
    // (over plain registration it has the advantage of registering sub objects)
    for( EapIter<DynObjType*> it(doGetTypes()); it(); it++ ){
        if( !it()->_vtbl && it()->_cdofn ){
            printf("Creating and registering %s\n", it()->_type );
            // No VTable yet, but we have an instantiate function
            int vtbl_size = -1;
            void *pv = it()->_cdofn(&vtbl_size);
            if( !pv ) { DO_LOG_ERR1(DOERR_FAILED_REGISTER_INSTANTIATE,
                        "Failed instantiate during doBaseInit registration: ", it()->_type ); }
            else{
                // Now we have both a VTable and VTable size
                //it()->_vtbl = *(void***)pv; - doConstruct does this!
                it()->_vtbl_size = vtbl_size;
                doConstruct( (VObj*)pv, it(), !it()->IsSideBase() );
                // Destroy temp object
                it()->_dcdofn(pv);
            }
        }
#if DO_USE_RUNTIME==1
        else if( it()->_vtbl && pdort )
            // Have VTable so it should be registered
            pdort->RegisterType(it()->_vtbl,it());
#endif
        // Check if side base that derives from DynI (and would require VTable correction)
#if DO_ENABLE_VTABLE_CORR!=1 && !defined(DI_GET_TYPE_GLOBAL)
        if( it()->IsSideBase() ){
            if( it()->IsA(DYNI_TYPE_ID) ){
                DO_LOG1( "doBaseInit: The side-base type %s is derived from DynI. Casts FROM this base to other\n"
                         "bases when using DynI::doGetObj / DynI::doGetType are unsafe. Use DI_GET_TYPE\n"
                         "in that case (or enable VTable correction).", it()->_type );
            }
        }
#endif // DO_ENABLE_VTABLE_CORR
    }
    
	// Check if we have unresolved types
	for( EapIter<DynObjType*> it(doGetTypesUnres()); it(); it++ )
		DO_LOG1( "doBaseInit: Unresolved - %s", it()->_type );
    
    // Make sure types are NULL terminated now
    if( !doGetTypes().Size() || doGetTypes().Top() )
        doGetTypes().Push(NULL);
        
    // Set up DynObjLibImpl fields
    if( !g_do_lib_impl._module ){
        g_do_lib_impl._implements = g_do_implements;
        g_do_lib_impl._do_traits = doGetDoTraits();
        g_do_lib_impl._types = g_do_types->GetBasePointerRef( ); 
        g_do_lib_impl._module = &g_do_mod;
    }
    
    g_do_base_init = 1;
#if DO_USE_RUNTIME==1
    if( pdorti )
        pdorti->TempDisableCreate( false );
#endif
    
    return &g_do_lib_impl;
}


// Called on unloading a library, to remove module and types from DoRunTime
extern "C" SYM_EXPORT void doExit( ){
#if defined(DO_MODULE) && DO_USE_RUNTIME==1
	if( &DORT ){
		for( EapIter<DynObjType*> it(doGetTypes()); it(); it++ ){
			if( it()->_vtbl )
				DORT.UnRegisterType(it()->_vtbl);
		}
		DORT.UnRegisterModule( &g_do_mod );
		g_dort = NULL;
#if DO_ENABLE_VTABLE_CORR==1
		for( EapIter<VTableInfo*> it(g_do_vtbls); it(); it++ )
			DORT.UnRegisterType(it()->vtbl_o);
#endif 
	}
#endif
}

//#include "pi/compiler.h"
DynObjType::DynObjType( const char *type, int type_id, int flag_ver, int size, 
							   const void *this_ptr, const void *base_ptr ) : 
		_sh(sizeof(DynObjType),0,0), _type(NULL), _type_id(type_id), _version(0), _size(size),
		_base(NULL), _vtbl(NULL), _base_side(NULL), _vtbl_size(-1), _flags(0), 
        _module(NULL), _cdofn(NULL), _dcdofn(NULL), 
        _idc_prv(0), _idc_nxt(0) {_CFE_;
	
	//void **_ebp;
	//GET_FRAME_PTR(_ebp);
	//void *caller = _ebp ? _ebp[-1] : NULL;
	//printf("::DynObjType - _ebp:%p    caller:%p    doExit:%p    &doExit:%p\n", _ebp, caller, doExit, &doExit );
	
	// GCC ctor bug, force ordinary function
	Ctor( type, type_id, flag_ver, size, this_ptr, base_ptr );
}

void DynObjType::Ctor( const char *type, int type_id, int flag_ver, int size, 
							   const void *this_ptr, const void *base_ptr ) {_CFE_;
	_flags = short(flag_ver>>16);
	_version = short(flag_ver&0xFFFF);
	if( !type ){
		DO_LOG_ERR(DOERR_EMPTY_TYPE_STRING,"::DynObjType: Empty type string received\n"); 
		return; 
	}
    
    if( type_id&(~TYPE_ID_MASK) )
        DO_LOG2( "::DynObjType: type id (%08X) for %s uses bits outside default ID mask", type_id, type );
	
	_module = &g_do_mod;
	
	// Did we get a pointer to an object? 
	// This is only useful for user-types and side bases (which set vtable themselves)
	if( _flags&DOT_USER_TYPE && this_ptr ){
		_vtbl = *(void***)this_ptr;
#if DO_USE_RUNTIME==1
		// This is best time to register the type
		if( &DORT ) DORT.RegisterType( _vtbl, this );
#endif
	}
	
	// For virtual inheritance, the base pointer is different to the this_ptr
	// _offset ends up negative below for virtual inheritance
	_offset = this_ptr && base_ptr ? (short)(((const char*)this_ptr)-((const char*)base_ptr)) : 0;
	
	const char *pc = strchr(type,':');
	if( !pc ){
		// Simple case, no base class 
		char *str = new char[strlen(type)+1];
		if( !str ) return;	// ## Set error
		strcpy(str,type);
		_type = str;
	}
	else{
		int bcl = (int)strlen(pc+1);
		const char *br_pos = strchr(type,']');	// For side bases
		if( strchr(br_pos?br_pos+2:pc+1,':') ){
			DO_LOG_ERR1(DOERR_EXPECTED_ONLY_ONE_BASE,"::DynObjType: More than one direct base class given: %s\n",type); // An error 
			return;
		}
		if( !strncmp(pc+1,type,bcl) && type[bcl]==':' ){
			DO_LOG_ERR1(DOERR_TYPE_USES_SELF_AS_BASE,"::DynObjType: Type referring to itself as base class found: %s\n",type); // An error 
			return;
		}
		// Look for base type in our module
		DynObjType *pdt = doGetTypes().Find(pc+1);
		if( pdt ){
			if( pdt==this ){
				DO_LOG_ERR1(DOERR_TYPE_USES_SELF_AS_BASE,"::DynObjType: Type referring to itself as base class found: %s\n",type); // An error 
				return;
			}
			_base = pdt;
		}
		else{
			// Resolve base type later
			doGetTypesUnres().Push(this);
			//DO_TRACE1( "Unresolved type: %s\n", type );
			pc = pc+strlen(pc);
		}
		char *str = new char[pc+1-type];
		if( !str ) return;	// ## Set error
		strncpy(str,type,pc-type);
		str[pc-type] = 0;
		_type = str;
	}

	// Before pushing, check that we don't have this class already
	// (also, protect from instances created on the stack and repeatedly
	// registered/destroyed).
	DynObjType *pdt;
	for( int ix=doGetTypes().Size()-1; ix>=0; ix-- ){
		pdt = doGetTypes()[ix];
		if( pdt->_sh.m_magic!=SH_MAGIC ){
			DO_LOG1("Invalid DynObjType structure at %p\n",pdt);
			doGetTypes().RemoveIndexUnordered(ix);
		}
		if( pdt->_type_id==_type_id ){
			if( SIDEBASE_TYPE_ID!=_type_id ||
				!strcmp(pdt->_type,_type) ){
				DynObjType_BpDummy();
				DO_LOG_ERR2(DOERR_TYPE_ALREADY_EXISTS,"::DynObjType: Constructing already existing DynObjType structure %s (%p)\n",pdt->_type,pdt);
				return;
			}
		}
	}
	// Register it
	doGetTypes().Push(this);
	//DO_TRACE1( "Registering type: %s\n", _type );
	
	// Does this new type resolve base for some other types?
	for( EapIter<DynObjType*> it(doGetTypesUnres()); it(); it++ ){
		const char *pc = strchr(it()->_type,':');
		if( pc ){
		   int tl = (int)strlen(pc+1);
			if( !strncmp(_type,pc+1,tl) && (!_type[tl] || _type[tl]==':') ){
				//DO_TRACE1( "Resolving type: %s\n", it()->_type );
				*(char*)pc = 0;	// Know base class, remove ':'
				it()->_base = this;
				doGetTypesUnres().RemoveIndexUnordered(it.Index());
				it--;
			}
		}
	}
}

MIBSideType::MIBSideType( DynObjType *compound, short offset, const char *type, int type_id, int ver, int size, 
								  const void *_this, int vtbl_size )
	 : DynObjType( type, type_id, ver, size, _this?((const char*)_this)+offset:NULL ), 
		_compound(compound) {_CFE_; 
    _sh.SetFlags(_sh.Flags()|DOT_SIDE_TYPE); // So that it's possible to tell we're a derived object 
	_offset = offset;     // Override 0 from DynObjType ctor
	_vtbl_size = vtbl_size;
	if( _this ){
		// Since we're a side base, we know that this VTable is ours
		_vtbl = *(void***)_this;
#if DO_USE_RUNTIME==1
		if( &DORT ) DORT.RegisterType( _vtbl, this );
#endif
	}
}


DynObjType::~DynObjType(){_CFE_; 
	_sh.m_magic = 0;	// Erase the magic marker
	delete [] _type; 
}

bool DynObjType::operator==(const char *type) const {
	if( !type ) return false;
	// Need to match also when unresolved 
	int tl = (int)strlen(type);
	if( strncmp(_type,type,tl) ) return false;
	return !_type[tl] || _type[tl]==':'; 
}

bool DynObjType::IsA( int type_id ) const {
	return _type_id==type_id ? true : (_base?_base->IsA(type_id):false);
}

bool DynObjType::CanBeAInternal( int type_id ) const {
	if( _type_id==type_id ) return true;
	if( _base && _base->CanBeAInternal(type_id) ) return true;
	if( IsSideBase() ) return false;
	for( DynObjType *psb=_base_side; psb; psb=psb->_base_side )
		if( psb->CanBeAInternal(type_id) ) return true;
	return false;
}

// This searches for the requested type from the outermost type
// (global, exhaustive)
bool DynObjType::CanBeA( int type_id ) const {
    if( !this ) return false;
    const DynObjType *pdt = GetOuterMost();
    return pdt->CanBeAInternal( type_id );
}

DynObjType* DynObjType::GetOuterMost() const {
    if( !this ) return NULL;
    const DynObjType *pdt = this;
    while( pdt->IsSideBase() ){
        pdt = ((MIBSideType*)pdt)->_compound;
        DO_ASSERT_MSG( pdt, "GetOuterMost - No outermost type found" );
    }
    return (DynObjType*)pdt;
}

DynObjType* DynObjType::GetTypeAtOffset( int off ) const {
    if( !this || off<0 ) return NULL;	// We could do it for <0 also, but no point
    if( !off ) return (DynObjType*)this;
    
    // Walk side bases until we know where to find type
    const DynObjType *pdt = this;
    while(  pdt->_base_side && off>=pdt->_base_side->_offset )
    	pdt = pdt->_base_side;
    if( !pdt ) return NULL;
    off -= pdt->_offset;
    
    /*
    // This could happen for virtual base classes (?)
    while( off>0 && pdt->_base ) {
    	off -= pdt->_offset;
    	pdt = pdt->_base;
    }
    */
    
    return !off ? (DynObjType*)pdt : NULL;
}


// Recursive casting on type string
void* DynObjType::GetObj( void *pobj, const char *type, DynObjType **found_type ){_CFE_;	
	// No checking of args, this function can only be called from inside DynObj.cpp
	DynObjType *pdt;
	
	// Self? 
	if( !strcmp(_type,type) ){ 
		if( found_type ) *found_type=this;
		return pobj;
	}
	
 	// Try all main bases
 	int off = IsSideBase() ? 0 : _offset;
 	for( pdt=_base; pdt; pdt=pdt->_base ){
		if( !strcmp(pdt->_type,type) )
			return ((char*)pobj) - off;
		off += pdt->_offset;
	}

	// Try each side type (start at its base)
	if( !IsSideBase() ){
		DynObjType *pst;
		for( pst=_base_side; pst; pst=pst->_base_side ){
			off = pst->_offset; // Side base class offset
			for( pdt=pst->_base; pdt; pdt=pdt->_base ){
				if( !strcmp(pdt->_type,type) ){
					if( found_type ) *found_type=pdt;
					return ((char*)pobj) + off;
				}
				off -= pdt->_offset;	// These are virtual base class offsets
			}
		}
	}
	
	// Done
	return NULL;
}

// Recursive casting on type string
void* DynObjType::GetObjExh( void *pobj, const char *type, DynObjType **found_type ){_CFE_;	
	// No checking of args, this function can only be called from inside DynObj.cpp
	// We try the direct bases from left to right
	if( !strcmp(_base->_type,type) ){
		if( found_type ) *found_type=this;
		return ((char*)pobj) - (IsSideBase() ? 0 : _offset);
	}

	// Try each side type 
	if( !IsSideBase() ){
		for( DynObjType *pdt=_base_side; pdt; pdt=pdt->_base_side )
			if( !strcmp(pdt->_type,type) ){
				if( found_type ) *found_type=pdt;
				return ((char*)pobj) + pdt->_offset;
			}
	}
	
	// recursively do the same iteration
	void *pco;
	if( _base->_base && (pco=_base->GetObjExh(((char*)pobj)-(IsSideBase() ? 0 : _offset),type,found_type)) )
		return pco;

	// Try each side type 
	if( !IsSideBase() ){
		DynObjType *pst;
		for( pst=_base_side; pst; pst=pst->_base_side )
			if( pst->_base && (pco=pst->GetObjExh(((char*)pobj)+pst->_offset,type,found_type)) )
				return pco;
	}
	
	// Exhausted
	return NULL;
}

// Recursive casting on type string
void* DynObjType::GetObj( void *pobj, int type_id, DynObjType **found_type ){_CFE_;	
	// No checking of args, this function can only be called from inside DynObj.cpp
	DynObjType *pdt;
	
	// Self? 
	if( _type_id==type_id ){
		if( found_type ) *found_type=this;
		return pobj;
	}
	
 	// Try all main bases
 	int off = IsSideBase() ? 0 : _offset;
 	for( pdt=_base; pdt; pdt=pdt->_base ){
		if( pdt->_type_id==type_id ){
			if( found_type ) *found_type=pdt;
			return ((char*)pobj) - off;
		}
		off += pdt->_offset;
	}

	// Try each side type (start at its base)
	if( !IsSideBase() ){
		DynObjType *pst;
		for( pst=_base_side; pst; pst=pst->_base_side ){
			off = pst->_offset; // Side base class offset
			for( pdt=pst->_base; pdt; pdt=pdt->_base ){
				if( pdt->_type_id==type_id ){
					if( found_type ) *found_type=pdt;
					return ((char*)pobj) + off;
				}
				off -= pdt->_offset;	// These are virtual base class offsets
			}
		}
	}

	// Done
	return NULL;
}

// Recursive casting on type id
void* DynObjType::GetObjExh( void *pobj, int type_id, DynObjType **found_type ){_CFE_;	
	// No checking of args, this function can only be called from inside DynObj.cpp
	// We try the direct bases from left to right
	if( _base->_type_id==type_id ){
		if( found_type ) *found_type=this;
		return ((char*)pobj) - (IsSideBase() ? 0 : _offset);
	}

	// Side bases themselves, per definition have no side bases. 
	// If ordinary base, Try each side type 
	if( !IsSideBase() ){
		for( DynObjType* pdt=_base_side; pdt; pdt=pdt->_base_side )
			if( pdt->_type_id==type_id ){
				if( found_type ) *found_type=pdt;
				return ((char*)pobj) + pdt->_offset;
			}
	}
	
	// recursively do the same iteration
	void *pco;
	if( _base->_base && (pco=_base->GetObjExh(((char*)pobj)-(IsSideBase() ? 0 : _offset),type_id,found_type)) )
		return pco;

	// Try each side type recursively
	if( !IsSideBase() ){
		DynObjType *pst;
		for( pst=_base_side; pst; pst=pst->_base_side )
			if( pst->_base && (pco=pst->GetObjExh(((char*)pobj)+pst->_offset,type_id,found_type)) )
				return pco;
	}
	
	// Exhausted
	return NULL;
}
	

// Instantiated to add a side base class to the type
void SideBaseDecl::Ctor( const char *main_cl, const char *side_base_cl, int size, int off, const void *_this, int vtbl_size, void **vtbl_o ){_CFE_;
	//BPDummy();
	if( !main_cl || !side_base_cl ) return;
	DynObjType *ptype =  doGetTypes().Find( main_cl );
	if( !ptype ){
		DO_LOG_ERR1(DOERR_SIDEBASE_NOT_FOUND,"SideBaseDecl: Main class not found: %s\n",main_cl);
		return;
	}
	if( off<=0 || off>32767){
		DO_LOG_ERR2(DOERR_SIDEBASE_OFFSET_OUT_OF_RANGE,"SideBaseDecl: [%s] Invalid offset: %d\n",side_base_cl,off);
		return;
	}
	
	// Check that there is space in main object 
	if( off+size>ptype->_size ){
		DO_LOG_ERR(DOERR_SIDEBASE_NO_SPACE_IN_MAIN,"SideBaseDecl: Not enough space in containing object.");
		return;
	}
	
	// Side types are their own types, derived from side_base_cl given above.
 	// (unless a type with no virtual functions)
	// make new name and allocate new structure for it. 
	// ## We could make a template to test for a virtual class
	int ptl = (int)strlen(ptype->_type);
	int sbl = (int)strlen(side_base_cl);
	char *str = new char[sbl+2+ptl+1+sbl+1];
	if( !str ){ DO_LOG_ERR(DOERR_NO_MEMORY,"SideBaseDecl: Mem alloc failed."); return; }
	strcpy(str,side_base_cl);
	strcat(str,"[");
	strcat(str,ptype->_type);
	// Don't want to include unresolved base of our base in name
	char *pc = strchr(str+sbl,':');
	if( pc ) *pc=0;
	strcat(str,"]:");
	strcat(str,side_base_cl);
	
	// This will auto register new type 
	// Side types don't need unique IDs since they're not addressed from outside
	MIBSideType *pst = new MIBSideType(ptype,off,str,SIDEBASE_TYPE_ID,1,size,_this,vtbl_size);
	//MIBSideType *pst = new MIBSideType(ptype,off,str,g_do_side_type_id,1,size,_this,vtbl_size);
	//g_do_side_type_id -= STEP_TYPE_ID;
	
	if( pst->_base ){
		// The VTable given as an argument is the one of the base of the side class
		if( vtbl_o && !pst->_base->_vtbl ) pst->_base->_vtbl = vtbl_o;
		// For a side type, VTable size is identical to parent VTable size
		if( vtbl_size<=0 && pst->_base->_vtbl_size>=0 )
			pst->_vtbl_size = pst->_base->_vtbl_size;
	}
	delete [] str;
	if( !pst ) { DO_LOG_ERR(DOERR_NO_MEMORY,"SideBaseDecl: Mem alloc failed."); return; } 
	DoGetMibst().Push(pst);
	
	// Add last among side bases (order does not matter, since offset is absolute)
	while( ptype->_base_side )
		ptype = ptype->_base_side;
	ptype->_base_side = pst;
}

// Main init function (manually called after global construction is done)
bool doInit( ){_CFE_;
#if defined(DO_MAIN)
    #if DO_USE_RUNTIME==1
    // Make sure DoRunTime VTable is known
	doConstruct(&DORT,DORT.doGetType(NULL),true);
    #endif

    // This will setup any registered types
    doBaseInit( NULL );
#endif
	
	return !doGetTypesUnres().Size();
}


//
// Resolve type for arbitrary object with VTable
// 
DynObjType* doGetType( const void* pobj ){_CFE_;
	if( !pobj ){
		DO_LOG( "doGetType called on NULL instance" );
		return NULL;
	}
	void **vtbl = *(void***)pobj;
	DynObjType *pdt;
#if DO_USE_RUNTIME==1
	DoRunTimeI *pdort = &DORT;
	// This should be fastest, DoRunTime has a hash table for lookup
	if( pdort && (pdt=pdort->GetTypeOf(vtbl)) )
		return pdt;
#endif
	// Look among all our types 
	pdt = doGetTypes().Find(vtbl);
	if( pdt ) return pdt;
#if DO_ENABLE_VTABLE_CORR==1
	// Look among SideBase types with modified VTables
	VTableInfo *vtp = g_do_vtbls.Find(vtbl);
	if( vtp ) return vtp->pdt;
#endif
	return pdt;
}

DynObjType* doGetType( const DynI* pdi ){_CFE_;
	if( !pdi ){
		DO_LOG( "doGetType called on NULL instance" );
		return NULL;
	}
	const DynI *pself = NULL;
	DynObjType *pdt = pdi->doGetType( &pself );
	if( !pdt ) return NULL;
	if( pself==pdi ) return pdt;
	if( !pself ){
		DO_LOG1("Warning: ::doGetType - The type %s does not fill in pself in diGetType", pdt->_type );
		return NULL;
	}
	
	// Case where we received pointer to the compound type
	DynObjType *pdt_sub = pdt->GetTypeAtOffset( int(((char*)pdi) - ((char*)pself)) );
	if( pdt_sub ) return pdt_sub;
	
	DO_LOG_ERR2( DOERR_UNKNOWN_TYPE, "::doGetType - Retrieved compound type but failed finding sub-type: (instance:%p, type:%s)", pdi, pdt->_type );
	return NULL;
}


//
// This is the casting based on DynObjType structure
//

// NOTE: We can also support a cast to an outermost main object
// by repeatedly moving from side base and doing a new type lookup
//

// Helper function for common string case
void* doGetObjCommon( void* pobj, DynObjType *pdt, const char *type, DynObjType **pdt_found ){_CFE_;
	// Request for itself?
	if( type && !strcmp(type,pdt->_type) ){
		if( pdt_found ) *pdt_found=pdt;
		return pobj;
	}
	
	// Request for compound object?
	if( !type || !strcmp(type,"COMPOUND") ){
		if( pdt->IsSideBase() ){
			if( pdt_found ) *pdt_found=pdt;
			// Adjust object pointer
			return ((char*)pobj) - pdt->_offset;
		}
        else return NULL;
	}
	
	// Request for compound object?
	if( !strcmp(type,"OUTERMOST") ){
		while( pdt && pdt->IsSideBase() ){
            pobj = ((char*)pobj) - pdt->_offset;
            pdt = ((MIBSideType*)pdt)->_compound;
		}
        if( pdt_found ) *pdt_found=pdt;
        return pobj;
	}
	
	return NULL;
}

void* doGetObj( void* pobj, const char *type, DynObjType *pdt, doCastAlgo algo, DynObjType **pdt_found ){_CFE_;
	if( !pobj ) return NULL;
	
	// No type yet?
	if( !pdt ){
		pdt = ::doGetType(pobj);
		if( !pdt ) return NULL;
	}
	
	// Sort out common simple cases
	void* pco = doGetObjCommon( pobj, pdt, type, pdt_found );
	if( pco ) return pco;
	
	// If caller is a DynI we should route to the specialized function
	if( pdt->IsA(DYNI_TYPE_ID) )
		return doGetObj( (DynI*)pobj, type, algo, pdt_found, false );

	
	// If we have base class, do a full GetObj at this level 
	if( pdt->_base )
		pco = pdt->GetObjExh(pobj,type,pdt_found);
	if( pco ) return pco;
	
	// If we're a side base in a compund object, do casting from point 
	// of view of compound object.
	if( !pdt->IsSideBase() || algo==doCastLocal )
		return NULL;
	
    // Try cross cast at next compound level
    while( true ){
        pobj = ((char*)pobj)-pdt->_offset;
        pdt = ((MIBSideType*)pdt)->_compound;
        if( !pdt ) break;
        pco = pdt->GetObjExh(pobj,type,pdt_found);
        if( pco )
            return pco;
        if( algo<doCastFull || !pdt->IsSideBase() )
            break;
    }
    
    return NULL;
}


void* doGetObj( void* pobj, int type_id, DynObjType *pdt, doCastAlgo algo, DynObjType **pdt_found ){_CFE_;
	if( !pobj ) return NULL;
	if( !pdt && !(pdt = ::doGetType(pobj)) )
        return NULL;
	
    void* pco;
    switch( type_id ){
    
        case COMPOUND_TYPE_ID:
            // If we are the compound object, return self
            if( pdt->IsSideBase() ){
                if( pdt_found ) *pdt_found=pdt;
                // Adjust object pointer
                return ((char*)pobj) - pdt->_offset;
            }
            else return NULL;
            
        case OUTERMOST_TYPE_ID:
            while( pdt && pdt->IsSideBase() ){
                pobj = ((char*)pobj) - pdt->_offset;
                pdt = ((MIBSideType*)pdt)->_compound;
            }
            if( pdt_found ) *pdt_found = pdt;
            return pobj;

        default:
            // Request for itself?
            if( type_id==pdt->_type_id ){
                if( pdt_found ) *pdt_found=pdt;
                return pobj;
            }
        
            // If we have base class, do a full GetObj at this level
            if( pdt->_base ){
	            pco = pdt->GetObjExh(pobj,type_id,pdt_found);
	            if( pco ) return pco;
            }
            
            // If we're a side base in a compund object, do casting from point 
            // of view of compound object.
            if( !pdt->IsSideBase() )
                return NULL;
            
            break;
    }
	
	
    // Try cross cast at next compound level
    while( true ){
        pobj = ((char*)pobj)-pdt->_offset;
        pdt = ((MIBSideType*)pdt)->_compound;
        if( !pdt ) break;
        pco = pdt->GetObjExh(pobj,type_id,pdt_found);
        if( pco )
            return pco;
        if( algo<doCastFull || !pdt->IsSideBase() )
            return NULL;
    }
    
    return NULL;
}


// Declare as non member function
//typedef void* docall (*doGetObjFn)(DynI* _this, const char *type);
typedef void* (*doGetObjFn)(DynI* _this, const char *type);

//
// Version for ::doGetObj with a string for DynI which gives priority 
// to DynI::doGetObj, allowing custom types to be accessed recursively.
// 
// This is also entry point for call from from DynI::doGetObj 
void* doGetObj( DynI *pdi, const char *type, doCastAlgo algo, DynObjType **pdt_found, bool internal_from_dyni ){_CFE_;
	if( !pdi ) return NULL; 
		
	DynObjType *pdt = DI_GET_TYPE(pdi); 
	if( !pdt ){
		DO_LOG_ERR1(DOERR_UNKNOWN_TYPE, "::doGetObj - failed looking up type", pdi );
		return NULL;
	}
	
#if DO_ENABLE_VTABLE_DISPATCH==1
	// Lock so only one thread can enter
    static SpinLock st_sl_getobj;
	SpinLocker sl(st_sl_getobj);
	if( !sl.IsOk() ) return NULL;
#endif    
	
	// Last caller context
	static DynI *pdi_last;
	DynI *pdi_prev = pdi_last;
	pdi_last = pdi;
	
	void *pobj = NULL;
	
	if( !internal_from_dyni ){
		// First entry into function
		// Call derived DynI::doGetObj for custom casts. That will call back to us
		pobj = pdi->doGetObj(type);
	}
	
	else{
#if DO_ENABLE_VTABLE_DISPATCH==1 
		// Recursive entry (always from inside DynI::doGetObj)
		// The derived class is responsible for calling its base
		// class version of doGetObj until DynI::doGetObj calls here
		// So we only have to check side bases now
		if( !pdt->IsSideBase() ){
			// for all bases
			for( DynObjType* pst=pdt->_base_side; pst && !pobj; pst=pst->_base_side ){
				// Only if Dyni or more derived
				if( pst->IsA(DYNI_TYPE_ID) && pst->_vtbl ){
					doGetObjFn fn = (doGetObjFn)pst->_vtbl[VTableIndex(DYNI_DOGETOBJ)];
					if( fn ){
						// This invokes the virtual function for the side base class
						// Add base offset
						DynI *pdi_rec = (DynI*)(((char*)pdi)+pst->_offset);
						pdi_last = pdi_rec;
						pobj = fn(pdi_rec,type);
					}
				}
			}
		}
#endif // DO_ENABLE_VTABLE_DISPATCH    
    }

	// Restore caller context
	pdi_last = pdi_prev;
	
	if( !pobj ){	// No result yet?
		if( !pdi_prev ){ // If this is outermost call
			// Resolve in same way as ::doGetObj
			
			// Sort out common simple cases (self, compound)
			pobj = doGetObjCommon( pdi, pdt, type, pdt_found );

			if( !pobj && type ){
				// Use DynObjType cast
				if( pdt->_base ) pobj = pdt->GetObjExh(pdi,type,pdt_found);
				// With 1st entry, we're safe to expand the call to compound type
				if( !pobj && pdt->IsSideBase() ){
                
                    // Try cross cast at compound levels
                    while( true ){
                        pdi = (DynI*)(((char*)pdi)-pdt->_offset);
                        pdt = ((MIBSideType*)pdt)->_compound;
                        if( !pdt ) break;
                        pobj = pdt->GetObjExh(pdi,type,pdt_found);
                        if( pobj )
                            break;
                        if( algo<doCastFull || !pdt->IsSideBase() )
                            break;
                    }
				}
			}
		}
	}
	
	return pobj;
}

void* doGetObj( DynI* pdi, const char *type, doCastAlgo algo, DynObjType **found_type ) {_CFE_;
	return pdi ? ::doGetObj( pdi, type, ::doGetType(pdi), algo, found_type ) : NULL;
}

void* doGetObj( DynI* pdi, int type_id, doCastAlgo algo, DynObjType **found_type ) {_CFE_;
	return pdi ? ::doGetObj( pdi, type_id, ::doGetType(pdi), algo, found_type ) : NULL;
}



//
// Logging/Error reporting functions
//
void DO_PUTS(const char *str){_CFE_;
	if( !str || !*str ) return;
#if DO_USE_RUNTIME==1
	if( &DORT ){
		DORT.GetTextIo()->puts(str);
		return;
	}
#endif	
	puts(str);
}

// Appends new line
void DO_LOG( const char *str ){_CFE_;
	if( !str ) str="";
	DO_PUTS( str );
	int sl = (int)strlen(str);
	if( sl==0 || str[sl-1]!='\n' )
		DO_PUTS( "\n" );
}

void DO_LOG_ERR(int code, const char *str ){_CFE_;
	if( !str ) str="";
#if DO_USE_RUNTIME==1
	if(&DORT){
		DORT.SetLastError(code,str);
		return;
	}
#endif
	char buf[32];
	int sl = (int)strlen(str);
	sprintf( buf, "(Error: 0x08%X) - ", code );
	fprintf(stderr, "(Error: 0x08%X) - %s%c", code, str, sl && str[sl-1]=='\n' ? ' ' : '\n' );
}



// This will do one or two things:
// 1 - For a DynI derived object, it will set soft VTable for each side 
//     base object (restore doGetType and doGetObj)
// 2 - For all objects it will register all contained 
//     types with DoRunTime (including side types with their own VTables)
// Note: If not using VTable correction nor DoRunTime, this method does no work
bool doConstruct( const VObj *pobj, DynObjType *pdt, bool is_outer ){_CFE_;
	if( !pobj ) return false;
	
	// This method goes from compound objects to sub-objects. 
	if( !pdt && !(pdt=::doGetType(pobj)) ){
		DO_LOG_ERR1(DOERR_CANT_FIND_TYPE,"doConstruct: Cannot find type (NULL in doGetType (%P))\n",pobj);
		return false;
	}
	// Non VObj:s, stop here (nothing to do)
	if( !pdt->IsA(VOBJ_TYPE_ID) ) return false;
	
	// If we've done this type before, stop here
    bool do_set_init = true;
	if( is_outer ){
#if DO_ENABLE_VTABLE_CORR==0
        // If not VTable correction, nothing to do after registering 1st instance.
		if( pdt->_flags&DOT_DID_INIT )
			return true;
#endif            
#if DO_USE_RUNTIME==1
		// Only want to set flag if DoRunTime is initialized by now
		if( !&DORT ) do_set_init=false;
#endif
	}

	// If VTable for this type not set, do so now, but not recursively on main branch
	// Also OK to do for virtual base class (offset!=0) and side bases
	if( ((!pdt->_flags&DOT_DID_INIT) || !pdt->_vtbl) && 
        pdt->_base && (is_outer || pdt->IsSideBase() || pdt->_offset) ){
		pdt->_vtbl = *(void***)pobj;
#if DO_USE_RUNTIME==1
		// The right time to register, since we run this recursively, on all 
		// base types and derived types.
		if( &DORT ) DORT.RegisterType( pdt->_vtbl, pdt );
	}
	// This catches the case when type already have a VTable set, but we encounter it
	// with yet another VTable (happens with nested multi base class)
	else if( pdt->IsSideBase() && pdt->_vtbl!=*(void***)pobj )
		if( &DORT ) DORT.RegisterType( *(void***)pobj, pdt );
#else
	}
#endif
    if( do_set_init )
        pdt->_flags |= DOT_DID_INIT;

	// Iterate over main base classes - invoke doConstruct recursively
	// But do not do it for VObj itself (nothing to setup for it)
	int off = pdt->IsSideBase() ? 0 : -pdt->_offset;
	if( pdt->_base && pdt->_base->_base && !doConstruct( (VObj*)((char*)pobj)+off,pdt->_base,false) )
		return false;

    // Everything below relates to setting up side-base VTables, so return early if possible
    if( pdt->IsSideBase() || !pdt->_base_side ) 
        return true;

	bool is_di = pdt->IsA(DYNOBJ_TYPE_ID);
	bool is_do = is_di && pdt->IsA(DYNI_TYPE_ID);
	
	// Iterate over each of the side bases
	for( MIBSideType *pst = pdt->_base_side; pst; pst=pst->_base_side ){
		// For a side type, VTable size is identical to parent VTable size
		if( pst->_vtbl_size<=0 && pst->_base->_vtbl_size>=0 )
			pst->_vtbl_size = pst->_base->_vtbl_size;
		
		void *po = ((char*)pobj)+pst->_offset;
		bool should_install = is_di && pst->IsA(DYNI_TYPE_ID);
		bool did_install = false;
		if( pst->_base->_flags&DOT_NO_VTABLE_CORR )
			should_install = false;
		
		// If known VTable size
#if DO_ENABLE_VTABLE_CORR==1
		if( should_install && pst->_vtbl_size>0 ) {
			void **vtbl = *(void***)po;
			// If actual VTable don't match type
			VTableInfo *vtp = g_do_vtbls.Find(vtbl);
			if( !vtp ){
				vtp = new VTableInfo(vtbl,pst->_base->_vtbl,pst->_vtbl_size);
				if( !vtp ) return false;
				if( !vtp->AllocSoftVTbl() ){ delete vtp; DO_LOG_ERR(DOERR_NO_MEMORY,"doConstruct: AllocSoftVTbl failed."); return false; }
				vtp->pdt = pst;	// Bind back to type
				g_do_vtbls.Push(vtp);
				
				// Restore entries in soft vtable
				/*if( pst->IsA(DynI) )*/{
					static 	DynObjSideType dost;	
                    // Restoring doGetType and doGetObj to local methods
					vtp->vtbl_r[VTableIndex(DYNI_DOGETTYPE)] = (*(void***)&dost)[VTableIndex(DYNI_DOGETTYPE)]; // doGetType for side objects
					vtp->vtbl_r[VTableIndex(DYNI_DOGETOBJ)] = vtp->vtbl_o[VTableIndex(DYNI_DOGETOBJ)];		  // doGetObj
					if( pst->_flags&DOT_DESTROY_SOFT && is_do )
                        // Set doDestroy to 'harmless' method
						vtp->vtbl_r[VTableIndex(DYNOBJ_DODESTROY)] = (*(void***)&dost)[VTableIndex(DYNOBJ_DODESTROY)]; // doDestroy for side objects
					DO_TRACE1("doConstruct: Restoring VTable DynI methods for %s", pst->_type ); 
				}
				// # We could have mechanism in type to specify which methods to restore
			}
			// Already set? (should not be)
			if( vtbl!=vtp->vtbl_r ){
				// Install soft VTable
				*(void***)po = vtp->vtbl_r;
				pst->_vtbl = vtp->vtbl_r;
			}
			did_install = true;
		}
		if( should_install && !did_install )
			DO_LOG1("doConstruct (%s): A sidebase (derived from DynI), but could not restore original methods.\nCasts FROM this type will not work.", pst->_type ); 
#endif
		
		if( !doConstruct((VObj*)po,pst,false) )
			return false;
	}
	
	return true;
}

void doReleaseDestroy( DynObj *pdo ){_CFE_;
    if( !pdo ) return;
    DynSharedI *pds = (DynSharedI*)::doGetObj(pdo,DYNSHAREDI_TYPE_ID,DI_GET_TYPE(pdo));
    if( pds ) 
        pds->doRelease();
    else
        pdo->doDestroy( );
}

#include "pi/compiler.h"
bool doIsStatic( const VObj *pvo, DynObjType *pdt ){_CFE_;
	if( !pvo ) return false;
	if( !pdt ) pdt = doGetType(pvo);
	
	// It is enough that it derives from a type that is static
	DynObjType *pdt1 = pdt->GetOuterMost();
	for( pdt1=pdt; pdt1; pdt1=pdt1->_base )
		if( pdt1->_flags&DOT_STATIC_TYPE )
			break;
	if( !pdt1 ) return false;
	
#if DO_USE_RUNTIME==1
	if( !&DORT ) return false;
	// If object type is in the same module as DORT, then accept
	if( DORT.doGetType(NULL)->_module==pdt->_module ) 
		return true;
	
	// Get callers address (return address is in callers module)
	void **_ebp;
	GET_FRAME_PTR(_ebp);
	void *caller = _ebp ? _ebp[-1] : NULL;
	// If callers module is same as module for type, then accept
	// # Need DoRunTimeI support
	//if( DORT.GetModuleFor(caller)==pdt->_module ) 
	//	return true;
	
#else
	// Accept if the type of the object is from the same module as this one
	if( pdt->_module = &g_do_mod ) 
		return true;
#endif
	
	return false;
}

bool doIsStatic( const DynI *pdi ){_CFE_; 
	return pdi ? doIsStatic((VObj*)pdi,DI_GET_TYPE(pdi)) : false; 
}

