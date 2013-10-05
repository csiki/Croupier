// Copyright (c) 2007, Arne Steinarson
// Licensing for DynObj project - see DynObj-license.txt in this folder

#ifndef DYNSTR_H
#define DYNSTR_H

// %%DYNOBJ section general
// This section is auto-generated and manual changes will be lost when regenerated!!
#ifdef DO_IMPLEMENT_DYNSTR
    #define DO_IMPLEMENTING     // If app has not defined it already
#endif
#include "dynobj/DynObj.h"

// --- Forward class declarations ---
class DynStr;

// --- For each declared class, doTypeInfo template specializations ---
DO_DECL_TYPE_INFO(DynStr,DYNSTR_TYPE_ID);

// %%DYNOBJ section end

// Helper to get a NULL reference
#define DynStrNull ((DynStr&)*(DynStr*)0)


// A string interface that is Unicode aware. It stores strings 
// in UTF8 format. A character is handled as 'int', except for
// in operator = where wchar_t is used for combined Unix/Win32 
// compatibility.

// This points to the character after the end of a string (the 
// NUL character). Index -1 will point to the last character, 
// but sometimes, we need to point after that one.
#define STR_AT_END ((int)0x80000000)	

/** @ingroup DynI
  * Interface to a string class that handles Unicode. Internally it 
  * stores strings in UTF8 format. It does on-the-fly conversion
  * from wchar_t in constructor and in assignments. The array of 
  * charcters can be accessed both as a Unicode characters and as
  * 8-bit char:s. A Unicode character has the int type, except for
  * in assignments (for compatibility with 16-bit wchar_t on Windows). */
// %%DYNOBJ class(dyni) flags(notypeid)
class DynStr : public DynObj {
public:
    /** @name Virtuals
     *  Interface members */
    //@{ 
	virtual DynObjType* docall doGetType( const DynI **pself=0 ) const;
	
    /** Assign function for char string */
	virtual DynStr& 	docall 	AssignA(const char *s, int l=-1){ return *this; }
    /** Assign function for wchar_t string */
	virtual DynStr& 	docall 	AssignW(const wchar_t *s, int l=-1){ return *this; }
	//virtual bool 		docall 	operator == (const char *s) const { return false; }
    /** Compare with another string, 0 on equal. */
	virtual int         docall  Strcmp(const char *s) const { return 0; }
    /** Compare (case insensitive) with another string, 0 on equal. */
	virtual int 		docall 	Stricmp(const char *s) const { return false; }
	
    /** Return pointer to char data. */
	virtual const char* docall  Get( ) const { return NULL; }
    /** Return 8-bit char at offset ix. Use negative index to access from the end. */
	virtual int 		docall 	GetByte(int ix) const { return -1; }	// Returns one byte (8-bit char)
    /** Return Unicode character at offset ix. Use negative index to access from the end. */
	virtual int 		docall 	GetUniChar(int ix) const { return -1; }	// Returns one Unicode code-point (32-bit)
    /** Set 8-bit char at offset ix. */
	virtual bool 		docall  SetByte( int ix, char ch ) { return false; }
    /** Set Unicode character at offset ix. */
	virtual bool 		docall  SetUniChar( int ix, int ch ) { return false; }
	
    /** Length of string in Unicode characters. */
	virtual int 		docall  Length() const { return -1; };			// Length in Unicode characters
    /** Number of bytes needed to hold string. */    
	virtual int 		docall  ByteSize() const { return -1; };		// Nr of bytes before NUL
	
    /** Insert string at offset. */
	virtual DynStr& 	docall  Insert(int at, const char *s, int len) { return *this; }
    /** Insert character at offset. */
	virtual DynStr& 	docall  Insert(int at, int ch){ return *this; }
    /** Remove characters at position. cnt=1 => remove all after */
	virtual DynStr& 	docall 	Remove(int pos, int cnt){ return *this; }
	
    /** Return the right hand side string starting at position from_pos. */
	virtual const char* docall  Right(int from_pos) const { return 0; };
    /** Strip space at either lor both of left/right sides of the string. */
	virtual DynStr& 	docall 	Strip(bool left=true, bool right=true) { return *this; }
	
    /** Set string to empty. */
	virtual DynStr& 	docall 	Reset() { return *this; }
    /** Query if empty. */
	virtual bool 		docall 	IsEmpty() const { return false; }

    /** Substitute a Unicode character in the string. */
	virtual int 		docall 	Subst(int ch_find, int ch_repl) { return -1; }
    /** Substitute a full string in the string. */
	virtual int 		docall 	Subst(const char *s_find, const char *s_repl) { return -1; }
    /** Search for a string in the string. */
	virtual int 		docall 	Find(const char *str) const { return -1; }
	/** Looks for given string at offset 'pos' (Unicode offset).
	  * Use pos=STR_AT_END to check at at the end */
	virtual bool 		docall 	FindAt(const char *str, int pos=0) const { return false; }
    //@}

    /** @name Inlines
     *  Inline convenience functions */
     
    /** Assign operator, for UTF8 char* */
	DynStr& operator = (const char *s){ return AssignA(s,-1); }
    /** Assign operator, for wchar_t* */
	DynStr& operator = (const wchar_t *s){ return AssignW(s,-1); }
    /** Test for equality */
    bool operator == (const char *s) const { return !Strcmp(s); }
    /** Test for non equality */
	bool operator !=(const char *s) const { return Strcmp(s)!=0; }
    /** Return pointer to internal string */
	operator const char* () const { return Get(); } 
    /** Return pointer to internal string */
	const char* c_str() const { return Get(); } 
    /** Insert a string at the beginning of the string */
	DynStr& Prepend(const char *s){ return Insert(0,s,-1); }
    /** Insert a string at the end of the string */
	DynStr& Append(const char *s){ return Insert(STR_AT_END,s,-1); }
    /** Prepend a charcter at the beginning of the string */
	DynStr& Prepend(int ch){ return Insert(0,ch); return *this; }
    /** Prepend a character at the end of the string */
	DynStr& Append(int ch){ return Insert(STR_AT_END,ch); return *this; }
    /** Insert a string at arbitrary position in the string */
	DynStr& Insert(int at, const char *s){ 
        if(s) Insert(at,s,(int)strlen(s)); 
        return *this; 
    }
    /** Append a string */
	DynStr& operator += (const char *s) { return Append(s); }
    /** Append a character */
	DynStr& operator += (int ch) { Append(ch); return *this; }	// NOTE: Does not react on incorrect Unicode char
    /** Check if the string starts with given string */
	bool 	StartsWith(const char *str) const { return FindAt(str,0); }
    /** Check if the string ends with given string */
	bool 	EndsWith(const char *str) const { return FindAt(str,STR_AT_END); }
    /** If the string does not end with the given suffix, then append it */
	DynStr& Suffix(const char *str){ if(!FindAt(str,STR_AT_END)) Append(str); return *this; }
    /** If the string does not start with the given suffix, then prepend it */
	DynStr& Prefix(const char *str){ if(!FindAt(str,0)) Prepend(str); return *this; }
    /** Truncate at position */
    DynStr& Trunc( int at ){ return Remove(at,-1); }
    /** Return Unicode character at index ix. Use -1 to access last character. */
	int operator [] (int ix) const { return GetUniChar(ix); }
};

// %%DYNOBJ section implement
// This section is auto-generated and manual changes will be lost when regenerated!!
#ifdef DO_IMPLEMENT_DYNSTR

// Generate type information that auto-registers on module load
DynObjType g_do_vtype_DynStr("DynStr:DynObj",DYNSTR_TYPE_ID,1,sizeof(DynStr));
DynObjType* DynStr::doGetType( const DynI **pself ) const {
   if(pself) *pself=(const DynI*)(const void*)this;
   return &g_do_vtype_DynStr;
}
#endif //DO_IMPLEMENT_...
// %%DYNOBJ section end

#endif // DYNSTR_H


