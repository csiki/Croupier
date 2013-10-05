#ifndef STRUCT_HEADER_H
#define STRUCT_HEADER_H

#ifndef SH_ERROR
	#include <stdio.h>
	#include <assert.h>
	#define SH_ERROR(str) {puts(str); assert(0); }
#endif

// To identify a structure (the leading data descriptor)
#define SH_MAGIC           0x3AD6
#define SH_VERSION_MASK    0x07  // Version bits, 0..7, User fields
#define SH_FLAG_MASK       0xF0  // Flag mask
#define SH_FLAG1           0x10  // Bits 4..7 flags for app use
#define SH_FLAG2           0x20
#define SH_FLAG3           0x40
#define SH_FLAG4           0x80

// A structure with 4 bytes to identify a data structure as such
struct StructHeader {
public:
	StructHeader( int size, char flags=0, char version=0 ) :
	  m_magic(SH_MAGIC), m_size((char)size/4), m_flags(flags|version) {
	 	if( size>1023 || size<4 ) SH_ERROR("Size too large\n");
	 	if( flags&SH_VERSION_MASK || version&(0xFF^SH_VERSION_MASK) )
	 		SH_ERROR("Version/flags mixed up");
	}
	short m_magic;		// 0x3AD6
	char  m_size;		// Size of structure divided by 4 - Limited to 1024 bytes max!
	char  m_flags;		// Flag field
	
	int  Size() const { return m_size*4; }
	int  Flags() const { return m_flags&SH_FLAG_MASK; }
	void SetFlags( int flag ) { m_flags&=~SH_FLAG_MASK; m_flags|=(flag&SH_FLAG_MASK); }
	int  Version() const { return m_flags&SH_VERSION_MASK; }
	/*
	void SetVersion( int ver ){ 
		if( ver&~SH_VERSION_MASK ) SH_ERROR("ShSetVersion, version out of range");
		m_flags &= (char)~SH_VERSION_MASK;
		m_flags |= (char)ver;
	}
	*/
	bool Matches(int size, int ver) const { return m_magic==SH_MAGIC && Size()==size && Version()==ver; } 
	bool IsOk(int size, int ver) const { return m_magic==SH_MAGIC && Size()>=size && Version()>=ver; } 
};	


#endif // STRUCT_HEADER_H

