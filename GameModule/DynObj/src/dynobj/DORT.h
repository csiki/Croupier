// Copyright (c) 2007, Arne Steinarson
// Licensing for DynObj project - see DynObj-license.txt in this folder

#ifndef DORT_H
#define DORT_H

#if DO_USE_RUNTIME==1
    #define DORT doGetRunTime()
#else
	#define DORT (*(DoRunTimeI*)NULL)
#endif

/*
// We want to setup a way to get a referece to the global DoRunTimeI from anywhere.
// This has been simplified.
#if DO_USE_RUNTIME==1
	//class DoRunTimeI;
	#include "DoBaseI.h"
	#ifdef DO_MAIN
        // A proto
		DoRunTimeI& MainGetDort();	// In DoRunTime.cpp
		#define DORT MainGetDort()
	#else
        // Another proto
        DoRunTimeI& ModuleGetDort();            // In DynObj.cpp
		#define DORT ModuleGetDort()
	#endif
#else
    // No global DORT around
	#define DORT (*(DoRunTimeI*)NULL)
#endif
*/

#endif // DORT_H

