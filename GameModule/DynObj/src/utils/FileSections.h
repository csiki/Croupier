
#ifndef FILESECTIONS_H
#define FILESECTIONS_H

#include "utils/ExpArr.hpp"
#include "utils/StringBuffer.h"

// Read a text file, looking for sections:
// ##%% ASectionName %%##
// text to read here
// ##%% end %%##


class FileSections {
public:
	FileSections( const char *path=NULL );
	~FileSections( );
	
	bool AddFile( const char *path );
	bool SubstSection( const char *name, const StrBuf &sb_fr, StrBuf &out );
	StrBuf& GetSection( const char *name, bool &b_ok ){
		int ix = m_names.FindIndexOf(name);
		if( ix<0 ){
			static StrBuf sb_dummy;
			b_ok = false;
			return sb_dummy;
		}
		b_ok = true;
		return *m_sections[ix];
	}

protected:	
	StrBuf m_names;
	ExpArrP<StrBuf*> m_sections;
};

#endif // FILESECTIONS_H


