// Copyright (c) 2007, Arne Steinarson
// Licensing for DynObj project - see DynObj-license.txt in this folder


// Select which do_new to use
#if defined(DO_USE_RUNTIME)
    #if defined(DO_MAIN)
        // Use DoRunTime.h for optimal do_new
        #include "DoRunTime.h"
    #else // DO_MODULE
        // Alternative implememntation
        #include "DynObjLib.h"
    #endif
#endif

