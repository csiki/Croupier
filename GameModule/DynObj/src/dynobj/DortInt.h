// Copyright (c) 2007, Arne Steinarson
// Licensing for DynObj project - see DynObj-license.txt in this folder

#ifndef DORT_INT_H
#define DORT_INT_H

// %%DYNOBJ section general
// This section is auto-generated and manual changes will be lost when regenerated!!
#ifdef DO_IMPLEMENT_DORTINT
    #define DO_IMPLEMENTING     // If app has not defined it already
#endif
#include "dynobj/DynObj.h"

// --- Integer type ids for defined interfaces/classes ---

// --- Forward class declarations ---
struct DortInternalI;

// --- For each declared class, doTypeInfo template specializations ---
DO_DECL_TYPE_INFO(DortInternalI,DORTINTERNALI_TYPE_ID);

// %%DYNOBJ section end

// %% DYNOBJ class(vobj) flags(notypeid)
struct DortInternalI {
    virtual void docall OnObjectDestroy( DynObj *pdo ) = 0;
    virtual void docall TempDisableCreate( bool disable ) = 0;
};

// %%DYNOBJ section implement
// This section is auto-generated and manual changes will be lost when regenerated!!
#ifdef DO_IMPLEMENT_DORTINT

// Generate type information that auto-registers on module load
DynObjType g_do_vtype_DortInternalI("DortInternalI:VObj",DORTINTERNALI_TYPE_ID,1,sizeof(DortInternalI));
#endif //DO_IMPLEMENT_...
// %%DYNOBJ section end

#endif // DORT_INT_H 

