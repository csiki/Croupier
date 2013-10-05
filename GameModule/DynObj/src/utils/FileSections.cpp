
#include "FileSections.h"

#include <stdio.h>
#include "utils/utils.h"
#include "utils/StrUtils.h"
#define scChar char
#include "utils/scString.hpp"

#define StrBuf StringBuffer 

#define SECT_TAG_BEGIN "--##"
#define SECT_TAG_END "##--"


// Read a text file, looking for sections:
// ##%% ASectionName %%##
// text to read here
// ##%% end %%##

FileSections::FileSections( const char *path ){_CFE_; AddFile( path ); }
FileSections::~FileSections( ){_CFE_; 
	m_sections.DeleteAll(); 
}
	
bool FileSections::AddFile( const char *path ){_CFE_;
		FILE *pf = fopen( path, "rt" );
		if( !pf ) return false;
		int rl;
		bool in_sect = false;
		char *pc, *pc1; 
		CharBuf cbin;
		int l = 0;
		while( rl=ReadLine(pf,cbin,l)>=0 ){
			char *buf = cbin.Get();
			if( (pc=strstr(buf,SECT_TAG_BEGIN)) && (pc1=strstr(pc,SECT_TAG_END)) ){
				pc = (char*)SkipSpace(pc+4);
				pc1 = (char*)SkipSpaceRev(pc1);
				*pc1 = 0;
				if( in_sect ){
					if( !stricmp(pc,"end") )
						in_sect = false;
					else 
						printf( "Expected: %s end %s\n", SECT_TAG_BEGIN, SECT_TAG_END );
				}
				else{
					if( !strlen(pc) || m_names.IndexOf(pc)>=0 )
						printf( "Incorrect or no section name: %s\n", pc );
					else{
						m_names.Push(pc);
						in_sect = true;
						m_sections.Push( new StrBuf );
					}
				}
			}
			else if( in_sect )
				m_sections.Top()->Push( buf );
		}
		fclose( pf );
		return true;
	}
	
bool FileSections::SubstSection( const char *name, const StrBuf &sb_fr, StrBuf &out ){_CFE_;
		int ix_sect = m_names.FindIndexOf(name);
		if( ix_sect<0 ) return false;
		scString s;
		out.Empty();
		StrBuf *psect = m_sections[ix_sect];
		for( int ix=0; ix<psect->Size(); ix++ ){
			s = psect->Elem(ix);
			for( int jx=0; jx<sb_fr.Size()-1; jx+=2 )
				s.Subst(sb_fr[jx],sb_fr[jx+1]);
			out.Push(s);
		}
		return true;
}

#ifdef FILESECTIONS_TEST
int main(){_CFE_;
	FileSections fs;
	printf( "AF:%d\n", fs.AddFile( "utils/1.fs.cpp" ) );
	
	StrBuf sb_fr;
	sb_fr.Push( "_CLASS_" ).Push( "NamedI" );
	sb_fr.Push( "_BASE_" ).Push( "DynI" );
	
	StrBuf ob;
	printf( "SS2:%d\n\n", fs.SubstSection( "fstest", sb_fr, ob ) );
	for( int ix=0; ix<ob.Size(); ix++ )
		printf( "%s\n", ob[ix].Get() );

	sb_fr.Empty();	
	sb_fr.Push( "$$TYPE$$" ).Push( "INT" );
	sb_fr.Push( "$$VALUE$$" ).Push( "0x4ac981CF" );
	printf( "\nSS2:%d\n\n", fs.SubstSection( "another", sb_fr, ob ) );
	for( int ix=0; ix<ob.Size(); ix++ )
		printf( "%s\n", ob[ix].Get() );
		
	return 0;
}
#endif // FILESECTIONS_TEST

