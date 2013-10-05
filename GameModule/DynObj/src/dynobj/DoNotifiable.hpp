// Copyright (c) 2007, Arne Steinarson
// Licensing for DynObj project - see DynObj-license.txt in this folder

#ifndef DONOTIFIABLE_H
#define DONOTIFIABLE_H

#include "DoError.h"

#if !defined(NOTIF_TRACE)
	#if defined(DO_DEBUG) && !defined(NOTIF_TRACE) 
		#define NOTIF_TRACE(str) DO_TRACE(str)
	#else
		#define NOTIF_TRACE(str)
	#endif
#endif


// Get GenericCaster from here
#include "utils/TemplUtils.hpp"

// Get the DoCaster function from here
#include "DoBase.hpp"

// This is the cast operator that uses ::doGetObj. Together with the
// WeakRef<T> and NotifierI it allows for tracking object life
// times over SDO boundaries.
template <class T, class U>
struct GenericCaster<T,U,1> {
    static T Cast( U u ) {	
        //return do_cast<T,U>(u,doCastCross);
        return do_cast<T>(u,doCastCross);
    }
};

// Notfier and Notifiable interfaces here 
#include "utils/Notifiable.hpp"

// Can set this define now, since explicitely included
#ifndef DO_USE_NOTIFIER
    #define DO_USE_NOTIFIER 1
#endif

#endif  // DONOTIFIABLE_H


