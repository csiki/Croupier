// Copyright (c) 2007, Arne Steinarson
// Licensing for DynObj project - see DynObj-license.txt in this folder


#ifndef DYNOBJ_METHODS_H
#define DYNOBJ_METHODS_H

// Method offsets for DynI
#define DYNI_DOGETTYPE    0
#define DYNI_DOGETOBJ     1
#define DYNI_DOGETERROR   2
#define DYNI_DOCLEARERROR 3
#define DYNI_METHOD_COUNT 4

// Method offsets for DynObj
#define DYNOBJ_DODESTROY  (DYNI_METHOD_COUNT+0)
#define DYNOBJ_DODTOR     (DYNI_METHOD_COUNT+1)

// Method offsets fro DynSharedI
#define DYNSHAREDI_DOADDREF    (DYNI_METHOD_COUNT+0)
#define DYNSHAREDI_DORELEASE   (DYNI_METHOD_COUNT+1)

#endif // DYNOBJ_METHODS_H
