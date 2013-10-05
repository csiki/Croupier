// Copyright (c) 2007, Arne Steinarson
// Licensing for DynObj project - see DynObj-license.txt in this folder


#ifndef DOERROR_H
#define DOERROR_H

#include <stdio.h>

// Put string, either to DoRunTime or to stdout
void DO_PUTS(const char *str);

// As above, but new line terminate (if not already)
void DO_LOG(const char *str);

// Print error code and error message. If DoRunTime enabled,
// use it, otherwise to stderr.
void DO_LOG_ERR(int code, const char *str );

#if defined(DO_DEBUG) || defined(__WX_DEBUG__)
	#define DO_TRACE(str) DO_LOG(str)
	#define DO_TRACE1(str,a1) DO_LOG1(str,a1)
	#define DO_TRACE2(str,a1,a2) DO_LOG2(str,a1,a2)
#else
	#define DO_TRACE(str) /*str*/
	#define DO_TRACE1(str,a1) /*str a1*/
	#define DO_TRACE2(str,a1,a2) /*str a1 a2*/
#endif


// These are OK if error string is of known (short) length and a1, a2 are primitives (non-strings)
#define DO_MAX_MSG_LEN 256
#define DO_LOG1(str,a1) { \
	char lbuf[DO_MAX_MSG_LEN]; \
	sprintf(lbuf,str,a1); \
	DO_LOG(lbuf); \
	}
#define DO_LOG_ERR1(code,str,a1) { \
	char lbuf[DO_MAX_MSG_LEN]; \
	sprintf(lbuf,str,a1); \
	DO_LOG_ERR(code,lbuf); \
	}
#define DO_LOG2(str,a1,a2) { \
	char lbuf[DO_MAX_MSG_LEN]; \
	sprintf(lbuf,str,a1,a2); \
	DO_LOG(lbuf); \
	}
#define DO_LOG_ERR2(code,str,a1,a2) { \
	char lbuf[DO_MAX_MSG_LEN]; \
	sprintf(lbuf,str,a1,a2); \
	DO_LOG_ERR(code,lbuf); \
	}

// Error codes (sent to DoRunTime and/or stderr

// From DynObj.cpp
#define DOERR_TOO_SMALL_REQUESTED_OBJECT_SIZE	0x10000	// Too small object_size specified
#define DOERR_UNKNOWN_TYPE				0x10001	// Unrecognized object type
#define DOERR_NO_MEMORY					0x10002	// Failed allocating memory
#define DOERR_FAILED_CONSTRUCTION	    0x10003	// Failed object construction
#define DOERR_FAILED_REGISTER_LIB       0x10004
#define DOERR_TYPE_NOT_REGISTERED       0x10005
#define DOERR_NO_FLEX_OBJECT_SIZE       0x10006
#define DOERR_DELETE_ON_WRONG_OBJECT    0x10007
#define DOERR_FAILED_REGISTER_INSTANTIATE 0x10008
#define DOERR_INCORRECT_TYPE_ID         0x10009

// From DynObjLib.cpp
#define DOERR_LIB_LOAD_FAILED		   0x10200
#define DOERR_LIB_VTABLE_INCOMPATIBLE  0x10201
#define DOERR_LIB_FAILED_INIT          0x10202
#define DOERR_LIB_DATA_SIZE_MISMATCH   0x10203
#define DOERR_LIB_FAILED_DESTROY       0x10204
#define DOERR_LIB_TRACKER              0x10205



#define DOERR_CANT_FIND_TYPE		    0x10101
#define DOERR_EMPTY_TYPE_STRING         0x10102
#define DOERR_EXPECTED_ONLY_ONE_BASE    0x10103
#define DOERR_TYPE_USES_SELF_AS_BASE    0x10104
#define DOERR_INAVALID_STRUCT_MAGIC     0x10105
#define DOERR_TYPE_ALREADY_EXISTS       0x10106
#define DOERR_SIDEBASE_NOT_FOUND        0x10107
#define DOERR_SIDEBASE_OFFSET_OUT_OF_RANGE 0x10108
#define DOERR_SIDEBASE_NO_SPACE_IN_MAIN 0x10109

// doConstruct
#define DOERR_SIDEBASE_DYNI_GETTYPE_UNSAFE 0x10120

// DynShared
#define DOERR_DYNSHARED_INVALID_DODESTROY  0x10130
#define DOERR_DYNSHARED_DESTROY_ON_NON_ZERO_REF 0x10131

// Errors for individual classes
#define DOERR_CLASS_DYNSTR					0x11000
#define DOERR_CLASS_DYNARR					0x11001
#define DOERR_CLASS_DYNOBJHOLDER			0x11002
#define DOERR_CLASS_NAMEDREF                0x11003

// Errors for script languages
#define DOERR_SQUIRREL                      0x12000

#endif // DOERROR_H


