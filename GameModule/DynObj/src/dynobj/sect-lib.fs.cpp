// Copyright (c) 2007, Arne Steinarson
// Licensing for DynObj project - see DynObj-license.txt in this folder

--## Init-Adv ##-- 
// Setup so we can use placement new
// (if we want to support creating object in preallocated space)
#include "dynobj/DoError.h"
#ifndef DO_NO_PLACEMENT_NEW
    #ifdef new
        #undef new	// We use placement new below (if we want to support creating object in preallocated space)
    #endif
    #include <new> // Include if we have problems with placement new
#endif

// Function type for generating DynObj:s
typedef DynObj* (*NewDynObjFn)(void *pat, const DynI *pdi_init);
--## end ##--


--## NewClass ##--
DynObj* New$CLASSFUNC$( void* pat, const DynI* pdi_init ){
#ifdef DO_NO_PLACEMENT_NEW
    if( pat ){ DO_LOG_ERR( DOERR_NO_FLEX_OBJECT_SIZE, "Custom object allocation size not supported" ); return 0; }
    return new $CLASS$(pdi_init);
#else
    // If NULL pointer given, create object default way
    if( !pat ) return new $CLASS$(pdi_init);
    // Use placement new. Assume caller has allocated enough memory
    // Size info available in DynObjType::_size
    return new (pat) $CLASS$(pdi_init);
#endif
}
--## end ##--


--## InitDynLib ##--
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
--## end ##--


--## CreateDynObj-Top ##--
// The object creation function for this module
extern "C" SYM_EXPORT DynObj* CreateDynObj( const char *type, int type_id, const DynI *pdi_init, int object_size ){
    // This library don't support creating objects with other sizes
    if( object_size>0 ){ DO_LOG_ERR( DOERR_NO_FLEX_OBJECT_SIZE, "Custom object size not supported" ); return 0; }
    
    // Match type names and type IDs, then return a new object
--## end ##--


--## CreateDynObj-End ##--
    DO_LOG_ERR1( DOERR_UNKNOWN_TYPE, "CreateDynObj(%s): Unknown object type", type );
    return 0;
}
--## end ##-- 



--## CreateDynObj-TopAdv ##--
// The object creation function for this module
extern "C" SYM_EXPORT DynObj* CreateDynObj( const char *type, int type_id, const DynI *pdi_init, int object_size ){
#ifdef DO_NO_PLACEMENT_NEW
    if( object_size>0 ){ DO_LOG_ERR( DOERR_NO_FLEX_OBJECT_SIZE, "Custom object size not supported" ); return 0; }
#endif
    NewDynObjFn ndofn = 0;
    int actual_obj_size = -1;
--## end ##--


--## CreateDynObj-PerType-Name ##--
!strcmp(type,"$CLASS$")
--## end ##--

--## CreateDynObj-PerType-Id ##--
type_id==$ID$
--## end ##--

--## CreateDynObj-PerType-NameAndId ##--
(!strcmp(type,"$CLASS$") && type_id==$ID$)
--## end ##--

--## CreateDynObj-PerType-NameOrId ##--
(!strcmp(type,"$CLASS$") || type_id==$ID$)
--## end ##--


--## CreateDynObj-PerType-Body ##-- 
        return new $CLASS$(pdi_init);
    }
--## end ##--

--## CreateDynObj-PerType-Body-Adv ##-- 
        actual_obj_size = sizeof($CLASS$);
        ndofn = New$CLASSFUNC$;
    }
--## end ##--


--## CreateDynObj-EndAdv ##--
    if( !ndofn ){
        DO_LOG_ERR1( DOERR_UNKNOWN_TYPE, "CreateDynObj(%s): Unknown object type", type );
        return 0;
    }
    if( object_size && object_size<actual_obj_size ){
        DO_LOG_ERR1( DOERR_TOO_SMALL_REQUESTED_OBJECT_SIZE, "CreateDynObj(%s)", type );
        return 0;
    }
    void *ptmp = object_size ? new char[object_size] : 0;
    if( object_size && !ptmp){
        DO_LOG_ERR1( DOERR_NO_MEMORY, "CreateDynObj(%s)", type );
        return 0;
    }
    DynObj *pdi = ndofn(ptmp,pdi_init);
    if( !pdi ) DO_LOG_ERR1( DOERR_FAILED_CONSTRUCTION, "CreateDynObj(%s)", type );
    return pdi;
}
--## end ##--


--## Exit ##--
// Library specific exit code here
extern "C" SYM_EXPORT bool ExitDynLib( bool is_query ){
    return true;
}
#endif // DO_MODULE
--## end ##--

--## WIN32 ##--
#ifdef _WIN32
// Every DLL needs this on Win32
extern "C" bool __stdcall DllMain( void* hModule,
                       int  ul_reason_for_call,
                       void* lpReserved )
{
    return true;
}
#endif
--## end ##--

