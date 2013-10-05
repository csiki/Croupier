// Copyright (c) 2007, Arne Steinarson
// Licensing for DynObj project - see DynObj-license.txt in parent folder

/*
	This is a utility to parse through DynObj header files and generate type 
	related declarations, definitions and functions.
	
	It triggers on the keyword %%DYNOBJ in comments:
	
      %%DYNOBJ class(dyni) bases(MainBase,SideBase1,...,SideBaseN)
      
   which informs it that an exported class, derived from DynI, comes below.
   The bases part can be omitted and then it parses the declaration of the 
   class to retrieve those (taking all bases). 
   
   The "bases" attribute allows for filtering what types one makes visible.
   Sometimes the direct base class should not be visible, but rather one 
   of its ancestors.
   
   For types not derived from DynI (we call them VObj), the declaration is:
   
      %%DYNOBJ VObj bases(MainBase,SideBase1,...,SideBaseN)

   Three sections in the header file are generated:
     
      %%DYNOBJ section general
      %%DYNOBJ section use
      %%DYNOBJ section implement
   
   which each end with:
   
      %%DYNOBJ section end
   
	When the file has been generated once, you can move the auto generated 
	sections around, and they will stay there, when redone. 

	The general section defines the integer type IDs. If a file is remade, they 
	will stay the same. It also adds prototypes for type -> name and type -> id
	functions.
	
	The use section is included from the main program that uses the library.
	
	The implement section is should be included from the library, it provides
	type declarations and default bodies for doGetType functions.
*/

/*
#include "malloc.h"
#include "utils/sqmem.h"
void * operator new (size_t size){
	void *pv = sq_vm_malloc( size );
	return pv;
}

void * operator new[] (size_t size){
	return operator new(size);
}

void operator delete (void * pv){
	sq_vm_free( pv );
}

void operator delete[] (void *pv){
	operator delete(pv);
}
*/


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#define EXPARRP_IN_EXPARR
#include "utils/ExpArr.hpp"
//#include "utils/utf8String.h"
#include "utils/StringBuffer.h"
#include "utils/utils.h"
#include "utils/StrUtils.h"
#include "utils/FileSections.h"

#include "../DynObj.h"

//#define StrBuf StringBuffer 

void PDOH_BP_Dummy(){
	puts("BPDummy\n");
}
// Only looks at the beginning for name
bool GetAValIAt( const char *pc, StrBuf &sb, const char *name, const char** pc_next=NULL, bool name_found=false) {
	sb.Empty();
	if( !pc || !name ) return false;
	if( !name_found ){
		pc = SkipSpace(pc);
		if( !keywordieq(pc,name) ) return false;
		pc = SkipSpace(pc+strlen(name));
	}
	if( *pc!='(' ){
		// When not find (...) treat as an empty array
		if( pc_next ) *pc_next=pc;
		return true;
	}
	pc = SkipSpace(pc+1);
	
	// Loop through until we find ')'
	const char *pc1;
	char ch;
	int br_cnt;
	while( 1 ){
		// For recursive behaviour, count '(' and consume ')'
		br_cnt = 0;
		for( pc1=pc; ch=*pc1; pc1++ ){
			if( !ch ) break;
			if( !br_cnt && (isspace(ch) || ch==',' || ch==')') )
				break;
			if( ch=='(' ) br_cnt++;
			else if( ch==')' ) br_cnt--;
		}
		sb.Push(pc,pc1-pc);
		pc = SkipSpace(pc1);
		if( !*pc ) return false;
		if( *pc==')' ){
			if(pc_next) *pc_next = pc+1;
			return true;
		}
		pc = SkipSpace(pc+1);
	}
	
	// Never come here
	return false;
}

// Searches the full line for name
bool GetAValI( const char *pc, StrBuf &sb, const char *name) {
	pc = keywordifind(pc,name);
	if( !pc ) return false;
	return GetAValIAt(pc,sb,name,NULL,true);
	//return GetAValIAt(pc+strlen(name),sb,name,NULL,true);
}

// Read the first word as an AVal
bool GetAValIAtAny( const char *pc, StrBuf &sb, CharBuf &name, const char** pc_next=NULL) {
	if( !pc ) return false;
	pc = SkipSpace(pc);
	const char *pe = pc;
	while( isalnum(*pe) ) pe++;
	name.Assign(pc,pe-pc);
	return GetAValIAt(pe,sb,name,pc_next,true);
}

void ToDefineSymbol( char *pc ){
	for( ; *pc; pc++ ){
		*pc = toupper(*pc);
		if( *pc=='/' || *pc=='\\' || *pc=='-' ) *pc='_';
		if( *pc=='.' ){ *pc=0; break; }
	}
}

void RemoveSlashDots( char *pc ){
	for( ; *pc; pc++ ){
		if( *pc=='/' || *pc=='\\' || *pc=='-' ) *pc='_';
		if( *pc=='.' ){ *pc=0; break; }
	}
}

void UnTemplate( char *pc, bool is_define ){
	for( ; *pc; pc++ ){
		if( *pc=='<' || *pc=='>' ) *pc=is_define?'t':'T';
		if( *pc=='*' ) *pc='p';
	}
}

const char *FileNamePart( const char *path ){
	const char *pc = strrchr(path,'/');
	if( !pc ) pc=strrchr(path,'\\');
	return pc ? pc+1 : path;
}


// Will remove comments from buffer and return true if we're still 
// in a commented line
bool StripComment( char *buf, bool in_comment ){
	char *pc;
	if( in_comment ){
		// Look for end of block comment
		if( pc=strstr(buf,"*/") ){
			memmove(buf,pc+2,strlen(pc+1));
			return StripComment(buf,false);
		}
		else return true;
	}
	else{
		// Look for start of block comment
		if( pc=strstr(buf,"/*") )
			return StripComment(pc,true);
		else if( pc=strstr(buf,"//") )
			*pc = 0;
		return false;
	}
}

class StrBufKwi : public StringBuffer {
public:
	// Look for string, case insensitive
	int IndexOfI( const char *kw ){
		for( int ix=0; ix<Size(); ix++ ){
			if( !Elem(ix) ){
				if( !kw ) return ix;
			}
			else if(kw && !stricmp(Elem(ix).Get(),kw))
				return ix;
		}
		return -1;
	}
	// Look for keyword 
	int IndexOfKwi( const char *kw ){
		int kwl = kw ? strlen(kw) : 0;
		for( int ix=0; ix<Size(); ix++ ){
			if( !Elem(ix) ){
				if( !kw ) return ix;
			}
			else if(kw && keywordieq(Elem(ix).Get(),kw,kwl))
				return ix;
		}
		return -1;
	}
};


enum SectType {
	sNone=0,
	sGen,
	sUse,
	sImpl,
	sLib
};

class FileSect {
public:
	FileSect(SectType sect) : m_sect(sect) { }
	StrBuf m_sb;
	SectType m_sect;
	bool operator == (SectType sect){ return m_sect==sect; } 
};

// This is for checking type and/or id on creation (when generating library code)
enum CheckType {
	ctName = 1,
	ctId = 2,
	ctEither = 3,
	ctBoth = 4
};

class Type {
public:
	Type(const char* name, bool is_di, bool only_in_lib) : m_name(name), 
		m_is_di(is_di), m_is_vobj(is_di), m_skip_tid(false), /*m_keep_I(false),*/ 
		m_user_type(false), m_inst2reg(false), m_novcorr(false), m_static_type(false), 
		m_only_in_lib(only_in_lib), m_is_struct(false), m_no_proto(false), 
        m_is_template(false), m_version(1) { 
			m_chk_der=ctEither; m_chk_base=ctBoth; 
		}
	CharBuf m_name;
	StrBuf  m_bases;
	StrBuf  m_defines;	// All the defines this one is inside
	bool m_is_struct;
	bool m_is_di;
	bool m_is_vobj;
	bool m_skip_tid;
	//bool m_keep_I;
	bool m_user_type;
	bool m_inst2reg;	// Instantiate to register
    bool m_novcorr;     // Skip VTable correction for this type
    bool m_static_type; // All instances are static
	bool m_only_in_lib;
	bool m_no_proto;
    bool m_is_template;
	CheckType m_chk_der;
	CheckType m_chk_base;
	int  m_version;
	bool operator == (const char *name){ return m_name==name; }
};



const char *GetOpt(int argc, const char **argv, const char *opt ){
	if( !opt ) return NULL;
	int ol = strlen(opt);
	for( int i=0; i<argc; i++ ){
		if( argv[i] && !strncmp(argv[i],opt,ol) )
			return argv[i]+ol;
	}
	return NULL;
}

// Update checksums 
void CalcCheckSum( const char *pc, int& chk_sum1, int& chk_sum2 ){
    int l = strlen(pc);
    while( l>=4 ){
        chk_sum1 >>= 1;
        chk_sum1 += *(int*)pc;
        pc += 4;
        if( (l-=4)<4 ) break;
        chk_sum2 *= 371;
        chk_sum2 += *(int*)pc;
        pc += 4;
        l -= 4;
    }
    // Trailing bytes
    while( *pc )
        chk_sum1 = chk_sum1*3 + *pc++;
}


int main( int argc, const char **argv ){
	void *pv = malloc(1);
	free(pv);
	
	if( argc<=1 || !strncmp(argv[1],"-?",2) || strstr(argv[1],"-help") ){
		printf( "pdoh file.h [options] \n" );
		printf( "  Option -o  overwrites input file.\n");
		printf( "  Option -oFileName.h Writes output to Filename.h.\n");
		printf( "  Option -sN selects sections to generate (N=1..15: 1-gen, 2-use, 4-implement, 8-library).\n");
		printf( "  Option -fN section to separate file (N=1..15: 1-gen, 2-use, 4-implement, 8-library).\n");
		printf( "  Option -b  brief output (minimize comments).\n");
		printf( "  Option -c  treat as source file.\n");
		printf( "  Option -h  treat as header file.\n");
		printf( "  Option -p  Path where to find c++ template (sect-lib.fs.cpp) files. Use -p../ for parent directory..\n");
		return -1;
	}
	
	int tm = (int)time(NULL); 
	srand( (tm<<16) + tm + (argc*1000) + (int)argv[0] );	// For generating type IDs
	
	FILE *pf = fopen(argv[1],"rt");
	if(!pf){
		printf("failed open file \'%s\' for reading\n", argv[1] );
		return -1;
	}
	ExpArrP<FileSect*> afs;
	ExpArrP<Type*> acl;
	StrBuf type_ids;

	bool is_header = true;
	if( strstr(argv[1],".cc") || strstr(argv[1],".cpp") || strstr(argv[1],".cxx") || strstr(argv[1],".c++") )
		is_header = false;
	if( GetOpt(argc,argv,"-c") )
		is_header = false;
	if( GetOpt(argc,argv,"-h") )
		is_header = true;
	
	char *pc;
	FileSect *pfs = new FileSect(sNone);
	afs.Push(pfs);
	bool brief = GetOpt(argc,argv,"-b") ? true : false;
	bool in_auto = false;
	bool do_next_cl = false;
	bool in_comment = false;
	bool is_struct = false;
	//bool do_keep_I = false;
	bool vtab_corr = false;
    bool adv_lib = false;   // Generate library with placement new
	int  sections = is_header ? 5 : 5;	// Output sections, general and implement for both
	int  file_sections = 0;
	int skip_if_space = 0;
	int rl, rl_sum=0;
	StrBuf defines;
	const char *last_endif = NULL;	// To really put it at the end of the file (header guard)
	int line = 0;
    
	//char buf[512];
	CharBuf cb_in;
	char *buf;
	int cb_len = cb_in.Length();
	
    int chk_sum1=0, chk_sum2=0;   // Checksums to detect if we can skip file update safely
	
	while( (rl=ReadLine(pf,cb_in,cb_len))>=0 ) {
		buf = cb_in.Str();
		line++;
		pc = buf;
		rl_sum += rl;
		if( skip_if_space>0 ){
			// When generating sections, we insert space after, 
			// this allows for not growing that
			while( *pc && isspace(*pc) ) pc++;
			if( !*pc ){
				skip_if_space--;
				continue;
			}
		}
		// Find tag
		pc = strstr(buf,"%%"); 
        //printf( "Found tag %s\n", buf );
		if( pc ){
            // If inside a comment, skip
            if( in_comment && (!strstr(buf,"*/") || strstr(buf,"*/")>pc) )
                pc = NULL;
            else{
                pc = (char*)SkipSpace(pc+2);
                if( !strnicmp(pc,"DYNOBJ",6) ) pc+=6;
                else pc=NULL;
            }
		}
		if( pc ){
            CalcCheckSum( pc, chk_sum1, chk_sum2 );
			char *pc1 = (char*)SkipSpace(pc);
			if( keywordieq(pc1,"section") ){
				// Starting or ending a section, do not keep this line
				pc = pc1+7;
				in_auto = true;
				SectType ns = (SectType)-1;
				if( strstr(pc,"implement") ) ns = sImpl;
				else if( strstr(pc,"use") ) ns = sUse;
				else if( strstr(pc,"general") ) ns = sGen;
				else if( strstr(pc,"library") ) ns = sLib;
				else if( strstr(pc,"end") ){
					ns = sNone;
					in_auto = false; 
					skip_if_space = 1;
				}
				if( ns==-1 ){
					printf("Error (%d): Unknown %%DYNOBJ section found: %s\n",line,pc);
					fclose(pf);
					return -2;
				}
				if( pfs->m_sect!=ns ){
					pfs = new FileSect(ns);
					afs.Push(pfs);
				}
			}
			else{ 
				// This does not jump a section, so we add the line
				StrBufKwi aval;
				pfs->m_sb.Push(buf);
				do_next_cl = false;
				bool got_word = false;
				// Global file options?
				if( GetAValIAt(pc1,aval,"options") ){
					got_word = true;
					//if( aval.IndexOfKwi("keepi")!=-1 )
					//	do_keep_I = true;
					if( aval.IndexOfKwi("vtabcorr")!=-1 )
						vtab_corr = true;
					if( aval.IndexOfKwi("brief")!=-1 )
						brief = true;
				}
				if( GetAValI(pc1,aval,"generate") ){
					got_word = true;
					sections = 0;
					if( aval.IndexOfKwi("general")!=-1 )
						sections |= (1<<(sGen-1));
					if( aval.IndexOfKwi("use")!=-1 )
						sections |= (1<<(sUse-1));
					if( aval.IndexOfKwi("implement")!=-1 )
						sections |= (1<<(sImpl-1));
					if( aval.IndexOfKwi("library")!=-1 )
						sections |= (1<<(sLib-1));
					if( aval.Size() && isdigit(aval[0][0]) )
						sections = atoi(aval[0]);
				}
				if( GetAValI(pc1,aval,"tofile") ){
					got_word = true;
					file_sections = 0;
					if( aval.IndexOfKwi("general")!=-1 )
						file_sections |= (1<<(sGen-1));
					if( aval.IndexOfKwi("use")!=-1 )
						file_sections |= (1<<(sUse-1));
					if( aval.IndexOfKwi("implement")!=-1 )
						file_sections |= (1<<(sImpl-1));
					if( aval.IndexOfKwi("library")!=-1 )
						file_sections |= (1<<(sLib-1));
					if( aval.Size() && isdigit(aval[0][0]) )
						file_sections = atoi(aval[0]);
				}
				
				if( GetAValIAt(pc1,aval,"library",(const char**)&pc1) ){
                    printf("Found library insertion point\n");
					got_word = true;
					// Make a new library section to follow immediately here 
					pfs = new FileSect(sLib);
					afs.Push(pfs);
					// Include library section be default
					sections |= 1<<(sLib-1);
					
                    if( aval.IndexOfKwi("advanced")!=-1 )
                        adv_lib = true;
                    
					// Insert library code here
					if( GetAValIAt(pc1,aval,"types",(const char**)&pc1) ){
						StrBufKwi type;
						CharBuf name;
						Type *pt;
						// The value of types is an array of strings like 
						// DerCl1(both):BaseCl1(name),...
						for( int ix=0; ix<aval.Size(); ix++ ){
							const char *pc = aval[ix];
							// This reads the derived class name
							if( GetAValIAtAny( pc, type, name, &pc) ){
								//if( !(pt=EapFind(acl,name.Get())) )
								if( !(pt=acl.Find(name.Get())) )
									acl.Push( pt = new Type(name,false,true) );
								if( type.IndexOfKwi("both")!=-1 )
									pt->m_chk_der = ctBoth; 
								else if( type.IndexOfKwi("either")!=-1 )
									pt->m_chk_der = ctEither;
								else if( type.IndexOfKwi("name")!=-1 )
									pt->m_chk_der = ctName; 
								else if( type.IndexOfKwi("id")!=-1 )
									pt->m_chk_der = ctId;
									
								pc = (char*)SkipSpace(pc);
								if( *pc==':' ){
									pc++;
									// This reads the base class name
									if( GetAValIAtAny( pc, type, name, &pc) ){
										if( !pt->m_bases.Size() )
											pt->m_bases.Push(name);
										if( strcmp(pt->m_bases[0],name) )
											printf( "(%d) Conflicting base class name found in library declaration: %s, %s\n", line, pt->m_bases[0].Get(), name.Get() );
										if( type.IndexOfKwi("both")!=-1 )
											pt->m_chk_base = ctBoth; 
										else if( type.IndexOfKwi("either")!=-1 )
											pt->m_chk_base = ctEither;
										else if( type.IndexOfKwi("name")!=-1 )
											pt->m_chk_base = ctName; 
										else if( type.IndexOfKwi("id")!=-1 )
											pt->m_chk_base = ctId;
									}
								}
							}
						}
					}
				}
				
				if( GetAValIAt(pc1,aval,"class",(const char**)&pc1) ){
                    //printf( "Found class %s\n", pc1 );
					got_word = true;
					// Class declaration to follow
					bool is_di = (aval.IndexOfKwi("dyni")!=-1) || (aval.IndexOfKwi("dynobj")!=-1); 
					do_next_cl = true;
					acl.Push(new Type(NULL,is_di,false));
					if( aval.IndexOfKwi("vobj")!=-1 ) 
						acl.Top()->m_is_vobj = true;
					acl.Top()->m_user_type = (aval.IndexOfKwi("usertype")!=-1);
					
					if( GetAValI(pc1,aval,"name") && aval.Size()>0 )
						acl.Top()->m_name = aval[0];
					if( GetAValI(pc1,aval,"bases") )
						acl.Top()->m_bases.MoveFrom(aval);
					if( GetAValI(pc1,aval,"version") )
						acl.Top()->m_version = atoi(aval[0]);
					if( GetAValI(pc1,aval,"flags") && aval.Size()>0 ){
						if( aval.IndexOfKwi("notypeid")!=-1 )
							acl.Top()->m_skip_tid = true;
						//if( aval.IndexOfKwi("keepi")!=-1 )
						//	acl.Top()->m_keep_I = true;
						if( aval.IndexOfKwi("usertype")!=-1 )
							acl.Top()->m_user_type = true;
						if( aval.IndexOfKwi("inst2reg")!=-1 )
							acl.Top()->m_inst2reg = true;
						if( aval.IndexOfKwi("novcorr")!=-1 )
							acl.Top()->m_novcorr = true;
						if( aval.IndexOfKwi("static")!=-1 )
							acl.Top()->m_static_type = true;
						if( aval.IndexOfKwi("noproto")!=-1 )
							acl.Top()->m_no_proto = true;
						if( aval.IndexOfKwi("template")!=-1 )
							acl.Top()->m_is_template = true;
					}
				}

				if( !got_word ){
					printf("Error (%d): Unknown %%DYNOBJ keyword found: %s\n",line,pc);
					fclose(pf);
					return -2;
				}
			}
		}
		else{
			if( strstr(buf,"_TYPE_ID") && strstr(buf,"#define") )
				type_ids.Push(buf);
			if( !in_auto ){
				// Only keep lines when not in auto section
				pfs->m_sb.Push(buf);
				in_comment = StripComment(buf,in_comment);
				if( !in_comment ){
					if( strstr(buf,"#endif") )
						last_endif = pfs->m_sb.Top();
						
					// Is it an ifdef line?
					pc = (char*)SkipSpace(buf);
					if( !strnicmp(pc,"#if",3) )
						defines.Push(buf);
					else if( !strnicmp(pc,"#endif",6) ){
						if( defines.Size() )
							defines.Pop();
					}
					else if( (!strnicmp(pc,"#define",7) || !strnicmp(pc,"#undef",6)) && defines.Size() ){
						// See if we're just defining last #ifndef
						const char *pc1 = SkipSpace(SkipToSpace(pc));
						static CharBuf cb;
						cb.Assign(pc1,SkipToSpace(pc1)-pc1);
						if( strstr(defines.Top(),cb) )
							defines.Top() = "";	// Set to nothing
					}
				}
					
				if( !in_comment && do_next_cl && 
					(!(is_struct=!((pc=strstr(buf,"class")))) || (pc=strstr(buf,"struct"))) ){
					char *pcol = strchr(pc,':');
					if( !pcol ) pcol = strchr(pc,'{');
					if( pcol ){
						// Find class name
						Type *pt = acl.Top();
						pt->m_is_struct = is_struct;
						pt->m_defines = defines; 
						if( !pt->m_name.Length() )
							pt->m_name = GetWordRev(pcol-1);
						// Only read bases if not given in tag  
						if( !pt->m_bases.Size() ){
							// Find each base class - before ' or { or eol
							pc = pcol;
							if( *pcol==':' ){	// Must have : for base classes
								while( 1 ){
									char ch, *pc1 = strchr(pc,',');
									if( !pc1 ) pc1 = strchr(pc,'{');
									if( !pc1 ) pc1 = pc+strlen(pc);
									ch = *pc1;
									pt->m_bases.Push(GetWordRev(pc1-1));
									if( ch!=',' ) break;
									pc = pc1+1;
								}
							}
						}
					}
				}
			}
            else{
                // In Auto, 
            }
			if( !in_comment )
				do_next_cl = false;
		}
	}
	fclose(pf);

	// If no class declarations found, quit 
	if( !acl.Size() ){
		printf("No class declarations found, nothing to do, exiting.\n");
		return 0;
	}
	
	CharBuf cb;
	// Caching only makes sense when writing to a file
    bool on_file = GetOpt(argc,argv,"-o")!=NULL;
    if( on_file ){
	    //printf( "%x %x\n", chk_sum1, chk_sum2 );
	    // See if checksum same as last. Then quit.
	    CharBuf cb_cache = argv[1];
	    pc = (char*)strrchr(cb_cache,'/');
	    if( !pc ) pc = (char*)strrchr(cb_cache,'\\');
	    int name_pos = pc ? pc+1-cb_cache : 0;
	    if( pc ) pc[1]=0;
	    else cb_cache.Reset();
	    int nl = strlen(argv[1]+name_pos);
	    bool is_modified = true, found_entry=false;
	    cb_cache += ".pdoh-cache.txt";
	    //printf("%s\n",(const char*)cb_cache);
	    if( cb.LoadFromFile( cb_cache ) ){
	        const char *pc1 = strstr(cb,argv[1]+name_pos);
	        if( pc1 ){
	            // Found entry
	            found_entry = true;
	            int chk1, chk2;
	            sscanf(pc1+nl+1,"%x",&chk1);
	            sscanf(pc1+nl+1+9,"%x",&chk2);
	            //printf( "old: %08x %08x\n", chk1, chk2 );
	            if( chk1==chk_sum1 && chk2==chk_sum2 ){
	                // Same check sum, no need to update anything
	                //printf( "Checksum match, exiting\n" );
	                return 0; 
	            }
	            // Update checksums in contents
	            char buf[32];
	            sprintf( buf,"%08x", chk_sum1 );
	            memcpy( (char*)pc1+nl+1, buf, 8 );
	            sprintf( buf,"%08x", chk_sum2 );
	            memcpy( (char*)pc1+nl+1+9, buf, 8 );
	        }
	    }
	    if( !found_entry )
	        cb += cbprintf( "%s:%08x:%08x\n", argv[1]+name_pos, chk_sum1, chk_sum2 );
	    cb.SaveToFile( cb_cache );
	    printf("Found changes, updating sections - %s\n", argv[1] );
    }
	
	// Make sure we have each section
	int n_new_sect = 0;
	//if( !EapFind(afs,sGen) ){
	if( !afs.Find(sGen) ){
		afs.Push(new FileSect(sGen));
		n_new_sect++;
	}
	//if( !EapFind(afs,sUse) ){
	if( !afs.Find(sUse) ){
		afs.Push(new FileSect(sUse));
		n_new_sect++;	
	}
	//if( !EapFind(afs,sImpl) ){
	if( !afs.Find(sImpl) ){
		afs.Push(new FileSect(sImpl));
		n_new_sect++;
	}
	//if( !EapFind(afs,sLib) ){
	if( !afs.Find(sLib) ){
		afs.Push(new FileSect(sLib));
		n_new_sect++;
	}
	
	
	// Generate sections
	// Get a good random seed (2nd time)
	srand( (rand()<<16) + (rl_sum<<8) + *(int*)(buf+(rand()%48)) + acl.Size()*7261612 );	// For generating type IDs
	
	// We need this string in a couple of places
	cb = "DO_IMPLEMENT_";
	cb += FileNamePart(argv[1]);
	ToDefineSymbol( (char*)cb.Get() );
	CharBuf cb_ifdef_impl = "#ifdef "+cb;

	pfs = afs.Find(sGen);
	StrBuf &sb(pfs->m_sb);
	sb.Push("// %%DYNOBJ section general");
	sb.Push("// This section is auto-generated and manual changes will be lost when regenerated!!");
    if( is_header ){
        sb.Push(cb_ifdef_impl);
        sb.Push("    #define DO_IMPLEMENTING     // If app has not defined it already");
        sb.Push("#endif");
    }
    else
        // Source file
        sb.Push("#define DO_IMPLEMENTING     // Source file, always implementing");

	sb.Push("#include \"dynobj/DynObj.h\"");
	if( is_header && !brief ){
		sb.Push("//");
		sb.Push("// These are general definitions & declarations used on both the user side [main program]");
		sb.Push("// and the implementor [library] side. ");
	}
	
	sb.Push("");
	if( !brief ) sb.Push("//");
	sb.Push("// --- Integer type ids for defined interfaces/classes ---");
	int prev_sz = sb.Size();
	// For each class, define TYPE_ID
	StrBuf type_id_defs;
	int max_id_len = 0;
	for( EapIter<Type*> it(acl); it(); it++ ){
		if( !it()->m_name ){
			// Undefined class - remove
			delete it();
			acl.RemoveIndexUnordered(it.Index());
			it--;
			continue;
		}
		//strcpy(buf,it()->m_name);
		cb = it()->m_name;
		for( pc=cb.Str(); *pc; pc++){
			*pc=toupper(*pc);
			//if( *pc=='<' || *pc=='>' ) printf("Warning: Template types not supported: %s (use typedef for it)\n",buf);
		}
		UnTemplate( cb.Str(), true );
		
        /*
		// Take version number from interface name? - NO GOOD
		if( it()->m_version==1 && isdigit(pc[-1]) ){
			while( isdigit(pc[-1]) ) pc--;
			it()->m_version = atoi(pc);
			while( isdigit(pc[0]) ) pc++;
		}
        */
		//strcat(buf,"_TYPE_ID");
		cb += "_TYPE_ID";
		type_id_defs.Push(cb);
		if( (int)strlen(cb)>max_id_len )
			max_id_len = strlen(cb);
	}

	if( max_id_len>0 ){
		for( EapIter<Type*> it(acl); it(); it++ ){
			if( it()->m_skip_tid || it()->m_only_in_lib ) 
				continue;
			CharBuf cb_tid = type_id_defs[it.Index()];
			// Align the defines
			cb.cbprintf( "%s                              ", cb_tid.Get() );
			if( max_id_len+1<cb.Length() ) cb.Str()[max_id_len+1]= 0;
			pc = NULL;
			// Look among already defined type ids
			for( int ix=0; ix<type_ids.Size(); ix++ )
				if( pc=(char*)strstr(type_ids[ix],cb_tid) )
					break;
			if( pc ){
				// Resuse old ID
				pc += strlen(cb_tid);
				cb += GetWordFwd((char*)pc);
			}
			else{
				// Generate new ID
				// We leave the 12 low bits clear for general purpose type information
				// The MSB bit (31) is set for local/app-global type ID
				int r = 0x80000000 | ((rand()&0x7FFFF)<<12);
				//int r = 0;
				//while( r<BASE_CUSTOM_TYPE_ID )
				//	r = ((rand()<<16)+rand())&0xFFFFFF00;
				//sprintf(buf+strlen(buf),"0x%08X", r );
				cb += cbprintf("0x%08X",r);
			}
			sb.Push("#define "+cb);
		}
	}

	if( sb.Size()==prev_sz ) sb.Pop();
	else sb.Push("");
	
 	if( is_header ){
 		if( !brief ) sb.Push("//");
 		sb.Push("// --- Forward class declarations ---");
 		int prev_sz = sb.Size();
 		for( EapIter<Type*> it(acl); it(); it++ ){
 			if( it()->m_no_proto ) continue; 
 			cb.cbprintf( "%s %s;", it()->m_is_struct?"struct":"class", it()->m_name.Get() );
 			sb.Push(cb);
 		}
 		if( sb.Size()==prev_sz ) sb.Pop();
 		else sb.Push("");
		
 		prev_sz = sb.Size();
		sb.Push("// --- For each declared class, doTypeInfo template specializations ---");
		if( !brief ) sb.Push("// This allows creating objects from a C++ types and in run-time casts");
		for( EapIter<Type*> it(acl); it(); it++ ){
			cb.cbprintf( "DO_DECL_TYPE_INFO(%s,%s);", it()->m_name.Get(), type_id_defs[it.Index()].Get() );
			sb.Push(cb);
		}
 		if( sb.Size()==prev_sz ) sb.Pop();
 		else sb.Push("");
	}
 	
	sb.Push("// %%DYNOBJ section end");
	sb.Push("");

	
	// Generate the use section 
	//pfs = EapFind(afs,sUse);
	pfs = afs.Find(sUse);
	StrBuf &sb1(pfs->m_sb);
	sb1.Push("// %%DYNOBJ section use");
	sb1.Push("// This section is auto-generated and manual changes will be lost when regenerated!!");
	if( !brief ){
		sb1.Push("//");
		sb1.Push("// Define the symbol below from -only one place- in the project using these ");
		sb1.Push("// interfaces/classes [the client] (to avoid multiple definitions of functions).");
		sb1.Push("");
	}
	cb = "#ifdef DO_USE_";
	cb += FileNamePart(argv[1]);
	ToDefineSymbol( (char*)cb.Get()+10 );
	if(is_header) sb1.Push(cb);
	
	sb1.Push("");
	if( !brief )sb1.Push("//");
	sb1.Push("// --- DoTypeName function bodies for declared interfaces/classes ---");
	// For each class, implement DoTypeName function 
	for( EapIter<Type*> it(acl); it(); it++ ){
		if( it()->m_only_in_lib ) continue;
		cb = "const char* DoTypeName( ";
		cb += it()->m_name;
		cb += "* p ){";
		sb1.Push((const char*)cb);
		cb = "   return \"";
		cb += it()->m_name;
		cb += "\";";
		sb1.Push((const char*)cb);
		sb1.Push("}");
	}
	
	sb1.Push("");
	if( !brief ) sb1.Push("//");
	sb1.Push("// --- DoTypeId function bodies for declared interfaces/classes ---");
	// For each class, implement DoTypeId function 
	for( EapIter<Type*> it(acl); it(); it++ ){
		if( it()->m_only_in_lib ) continue;
		cb = "int DoTypeId( ";
		cb += it()->m_name;
		cb += "* p ){";
		sb1.Push((const char*)cb);
		cb = "   return ";
		cb += type_id_defs[it.Index()];
		cb += ";";
		sb1.Push((const char*)cb);
		sb1.Push("}");
	}

	if( !brief ) sb1.Push("");
	if( is_header ) cb = "#endif //DO_USE_...";
	sb1.Push(cb);
	sb1.Push("// %%DYNOBJ section end");
	sb1.Push("");

	// Generate the implements section 
	//pfs = EapFind(afs,sImpl);
	pfs = afs.Find(sImpl);
	StrBuf &sb2(pfs->m_sb);
	sb2.Push("// %%DYNOBJ section implement");
	sb2.Push("// This section is auto-generated and manual changes will be lost when regenerated!!");
	if( !brief ){
		sb2.Push("//");
		sb2.Push("// This section is for setting up the type system at run-time using global or ");
		sb2.Push("// static variable constructors. For types deriving from DynI, the doGetType ");
		sb2.Push("// member function is implemented. For other types, global declarations are used.");
		if( is_header ){
			sb2.Push("//");
			sb2.Push("// Define the symbol below from -only one place- in the project implementing ");
			sb2.Push("// these interfaces/classes [the library/module].");
			sb2.Push("");
		}
	}
	if( is_header ) sb2.Push(cb_ifdef_impl);
	sb2.Push("");
	sb2.Push("// Generate type information that auto-registers on module load");
	// For each class, make global type structures and possibly doGetType function 
	int sb_cnt = 0;
	CharBuf cb_type_var;
	int id_ix = 0;	// #ifdef index
	for( EapIter<Type*> it(acl); it(); it++ ){
		if( it()->m_only_in_lib ) continue;
		// if( it()->m_is_di ) sb2.Push("");

		// Generate #ifdefs around implementation statements that were ifdeffed previously  
		// Remove old ifdefs
		bool new_line=true;
		while( id_ix>it()->m_defines.Size() ){
			if( *(acl[it.Index()-1]->m_defines[id_ix-1].Get()) ){
				if( new_line ) { sb2.Push(""); new_line=false; }
				sb2.Push("#endif");
			}
			id_ix--;
		}
		while( id_ix>0 && strcmp(acl[it.Index()-1]->m_defines[id_ix-1],it()->m_defines[id_ix-1]) ){
			if( *(acl[it.Index()-1]->m_defines[id_ix-1].Get()) ){
				if( new_line ) { sb2.Push(""); new_line=false; }
				sb2.Push("#endif");
			}
			id_ix--;
		}
		// Add new ones
		new_line = true;
		while( id_ix<it()->m_defines.Size() ){
			if( *(it()->m_defines[id_ix].Get()) ){
				if( new_line ) { sb2.Push(""); new_line=false; }
				sb2.Push(it()->m_defines[id_ix].Get());
			}
			id_ix++;
		}
	
		// It's a general VTable type. Emit global variable for type.
		// DynObjType g_do_vtype_file_3("Main:Base",MAIN_TYPE_ID,sizeof(Main),NULL,NULL);
		const char *mb = it()->m_name;
		const char *fb;
		if( it()->m_bases.Size()>0 )
			fb = it()->m_bases[0].Get();
		else if( it()->m_is_vobj )
			fb = "VObj"; 
		else fb = NULL;
		if( fb && !strcmp(mb,fb) ) fb = NULL;
		
		// Generate name without template brackets in it
		CharBuf cb_mb = mb;
		UnTemplate( cb_mb.Get(), false );
		
		/*
		if( it()->m_bases.Size()<2 )
			cb = "// Type declaration for: ";
		else 
			cb = "// Type and side type declarations for: ";
		cb += mb;
		if( !brief ) sb2.Push(cb);
		*/
		
		cb = FileNamePart(argv[1]);
		RemoveSlashDots((char*)cb.Get());
		
		//sprintf( buf, "g_do_vtype_%s_%d", cb.Get(), it.Index() );
		//sprintf( buf, "g_do_vtype_%s", cb_mb.Get() );
		//cb_type_var = buf;
		cb_type_var.cbprintf( "g_do_vtype_%s", cb_mb.Get() );
        
        CharBuf cb1;
        bool sep = false;
        if( it()->m_user_type ) cb1 += "DOT_USER_TYPE", sep=true;
        if( it()->m_static_type ) cb1 += CharBuf(sep?"|":"")+"DOT_STATIC_TYPE", sep=true;
        if( it()->m_novcorr ) cb1 += CharBuf(sep?"|":"")+"DOT_NO_VTABLE_CORR", sep=true;
        if( cb1.Length() ) cb1 = "((" + cb1 + ")<<16)|";
        cb1 += cbprintf( "%d", it()->m_version );
        
        const char *base = !fb || !strcmp(fb,"VObj") ? "void" : fb;
        CharBuf cb_sizeof;
        if( !strcmp(mb,"VObj") ) cb_sizeof = "VOBJ_SIZE";
        else cb_sizeof.cbprintf( "sizeof(%s)", mb );
            
		if( it()->m_inst2reg || it()->m_user_type ){
			// This is when we instantiate an object to get its type
            const char *is_do = it()->m_user_type ? "true" : "false";
            cb.cbprintf( "DynObjTypeI2R<%s,%s,%s> %s(\"%s:%s\",%s,%s);", mb, base, is_do, cb_type_var.Get(), mb, fb, type_id_defs[it.Index()].Get(), cb1.Get() );
		}
		else{
			if( fb )
				cb.cbprintf( "DynObjType %s(\"%s:%s\",%s,%s,%s);", cb_type_var.Get(), mb, fb, type_id_defs[it.Index()].Get(), cb1.Get(), cb_sizeof.Str() );
				//sprintf( buf, "DynObjType %s(\"%s:%s\",%s,%s,sizeof(%s));", cb_type_var.Get(), mb, fb, type_id_defs[it.Index()].Get(), cb1.Get(), mb );
			else
				cb.cbprintf( "DynObjType %s(\"%s\",%s,%s,%s);", cb_type_var.Get(), mb, type_id_defs[it.Index()].Get(), cb1.Get(), cb_sizeof.Str() );
				//sprintf( buf, "DynObjType %s(\"%s\",%s,%s,sizeof(%s));", cb_type_var.Get(), mb, type_id_defs[it.Index()].Get(), cb1.Get(), mb );
		}
		sb2.Push(cb);
		
		// Emit for each side base class
		for( int ix=1; ix<it()->m_bases.Size(); ix++ ){
			const char *sb = it()->m_bases[ix];
			cb = FileNamePart(argv[1]);
			RemoveSlashDots((char*)cb.Get());
			//sprintf(buf, "SideBaseDecl g_do_vtype_sbd_%s_%s_%d(\"%s\",\"%s\",sizeof(%s),", cb.Get(), mb, ix, mb, sb, sb );
			cb.cbprintf("SideBaseDecl g_do_vtype_sbd_%s_%d(\"%s\",\"%s\",sizeof(%s),", mb, ix, mb, sb, sb );
			
			CharBuf cb1;
			if( vtab_corr )
				// This forces instantiation of sidebase class (GetVTable...)
				cb1.cbprintf("sidebase_offset<%s,%s>(),NULL,GetVTableSizeOf<%s>(),GetVTableOf<%s>());", mb, sb, sb, sb);
			else
				// Better do without that in the general case
				cb1.cbprintf("sidebase_offset<%s,%s>(),NULL,-1,NULL);", mb, sb);
			cb += cb1;
			sb2.Push(cb);
		}
		
		// Is it a DynI ?
		if( it()->m_is_di ){
			cb.cbprintf( "// DynI::doGetType implementation for: %s", it()->m_name.Get() );
			if( !brief ) sb2.Push(cb);
            // If template type, The function will be a specialization
            if( strchr(it()->m_name, '<') || it()->m_is_template )
                sb2.Push("template<>");
			cb.cbprintf( "DynObjType* %s::doGetType( const DynI **pself ) const {", it()->m_name.Get() );
			sb2.Push(cb);
			sb2.Push("   if(pself) *pself=(const DynI*)(const void*)this;");
			cb.cbprintf( "   return &%s;", cb_type_var.Get() );
			sb2.Push(cb);
			sb2.Push( "}" );
		}
	}
	while( id_ix>0 ){
		if( *acl.Top()->m_defines[id_ix-1].Get() )
			sb2.Push("#endif");
		id_ix--;
	}

	// Make public type declaration for classes tagged with usertype
	bool found_ut = false;
	for( EapIter<Type*> it(acl); it(); it++ ){
		if( it()->m_user_type ){
			if( !found_ut ){
				found_ut = true;
				sb2.Push("");
				sb2.Push("// Declarations to make types visible to library users");
			}
			cb = FileNamePart(argv[1]);
			RemoveSlashDots((char*)cb.Get());
			CharBuf cb1 = it()->m_name;
			if( it()->m_bases.Size() ){
				cb1 += ":";
				cb1 += it()->m_bases[0];
			}
			cb.cbprintf( "doUserTypeDecl g_do_ut_decl_%s_%d(\"%s\",%s);", cb.Get(), it.Index(), cb1.Get(), type_id_defs[it.Index()].Get() );
			sb2.Push(cb);
		}
	}
	if( !brief && found_ut )
		sb2.Push("");
	
	if(is_header){
		cb = "#endif //DO_IMPLEMENT_...";
		sb2.Push(cb);
	}
	if( !brief ) sb2.Push("//");
	sb2.Push("// %%DYNOBJ section end");
	sb2.Push("");


	// Should we generate all sections?
	// Command line overrides file settings 
	const char *po = GetOpt(argc,argv,"-s");
	if( po ) sections = atoi(po);
	po = GetOpt(argc,argv,"-f");
	if( po ) file_sections = atoi(po);
	

	// Generate the library section if requested
    if( sections&(1<<(sLib-1)) ){
        FileSections fs;	// Insertion code loaded here
        bool b_ok = true;
        int sl;
        if( !fs.AddFile("sect-lib.fs.cpp") ){
            cb = GetOpt(argc,argv,"-p");	
            if( sl=cb.Length() ){
                if( cb.Get()[sl-1]!='/' && cb.Get()[sl-1]!='\\' )
                    cb += "/";
                cb += "sect-lib.fs.cpp";
                if( !fs.AddFile(cb) )
                    b_ok = false;
            }
            else
                b_ok = false;
        }
        if( !b_ok ){
            printf( "Failed loading \"sect-lib.fs.cpp\", cannot generate library section\n" );
            printf( "Use option -p<path> to specify where \"sect-lib.fs.cpp\" can be found\n" );
            exit( -1 );
        }
            
        if( b_ok ){
            pfs = afs.Find(sLib);
            StrBuf &sb3(pfs->m_sb);
            StrBuf sb_ob, sb_sr;
            sb3.Push("// %%DYNOBJ section library");
            sb3.Push("// This section is auto-generated and manual changes will be lost when regenerated!!");
            sb3.Push("");
            sb3.Push("// The code below implements the library part in a loadable class library (plugin). ");
            sb3.Push("");
            
            // Init section for library
            sb3.Push( fs.GetSection("InitDynLib", b_ok) );
            if( !b_ok ) printf("sLib: No InitDynLib section found\n");
            sb3.Push("");

            if( adv_lib ){
                sb3.Push( fs.GetSection("Init-Adv", b_ok) );
                if( !b_ok ) printf("sLib: No Init-Adv section found\n");
                else sb3.Push("");
            
                // A NewClass function for each class
                for( EapIter<Type*> it(acl); it(); it++ ){
                    sb_sr.Empty(); sb_sr.Push("$CLASS$"); sb_sr.Push(it()->m_name.Get());
                    // Fix for template types
                    cb = it()->m_name;
                    UnTemplate( cb.Str(), false );
                    sb_sr.Push("$CLASSFUNC$"); sb_sr.Push(cb);
                    fs.SubstSection("NewClass",sb_sr,sb_ob);
                    sb3.Push(sb_ob);
                    sb3.Push("");
                }
                sb3.Push("");
            }

            // Create DynObj function
            if( !adv_lib )
                sb3.Push( fs.GetSection("CreateDynObj-Top", b_ok) );
            else
                sb3.Push( fs.GetSection("CreateDynObj-TopAdv", b_ok) );
            sb3.Push("");
                
            // For each class, a section to recognize class & base name / type id
            int sl;
            for( EapIter<Type*> it(acl); it(); it++ ){
                cb = "    if( (";
                sb_sr.Empty(); sb_sr.Push("$CLASS$").Push(it()->m_name);
                sb_sr.Push("$ID$").Push(type_id_defs[it.Index()].Get());
                if( it()->m_chk_der==ctBoth )
                    fs.SubstSection("CreateDynObj-PerType-NameAndId",sb_sr,sb_ob);
                else if( it()->m_chk_der==ctEither )
                    fs.SubstSection("CreateDynObj-PerType-NameOrId",sb_sr,sb_ob);
                else if( it()->m_chk_der==ctId )
                    fs.SubstSection("CreateDynObj-PerType-Id",sb_sr,sb_ob);
                else if( it()->m_chk_der==ctName )
                    fs.SubstSection("CreateDynObj-PerType-Name",sb_sr,sb_ob);
                if( !sb_ob.Size() ){
                    printf("sLib: Seems we have no CreateDynObj-PerType-... sections, skipping\n");
                    break;	// In this case, have no sections
                }
                sl = sb_ob[0].Length();
                if( strchr("\r\n",sb_ob[0][sl-1]) ) sb_ob[0].Str()[sl-1] = 0;
                cb += sb_ob[0];
                cb += ") ";
                if( it()->m_bases.Size() ) cb += "|| ";
                else cb += "){";
                sb3.Push(cb);
                
                if( it()->m_bases.Size() ){
                    sb_sr.Empty(); sb_sr.Push("$CLASS$").Push(it()->m_bases[0]);
                    CharBuf cb_base_id = it()->m_bases[0];
                    ToDefineSymbol( (char*)cb_base_id.Get() );
                    cb.cbprintf( "%s_TYPE_ID", cb_base_id.Get() );
                    sb_sr.Push("$ID$").Push(cb);
                    cb = "        (";
                    if( it()->m_chk_base==ctBoth )
                        fs.SubstSection("CreateDynObj-PerType-NameAndId",sb_sr,sb_ob);
                    else if( it()->m_chk_base==ctEither )
                        fs.SubstSection("CreateDynObj-PerType-NameOrId",sb_sr,sb_ob);
                    else if( it()->m_chk_base==ctId )
                        fs.SubstSection("CreateDynObj-PerType-Id",sb_sr,sb_ob);
                    else if( it()->m_chk_base==ctName )
                        fs.SubstSection("CreateDynObj-PerType-Name",sb_sr,sb_ob);
                    sl = sb_ob[0].Length();
                    if( strchr("\r\n",sb_ob[0][sl-1]) ) sb_ob[0].Str()[sl-1] = 0;
                    cb += sb_ob[0];
                    cb += ") ){";
                    sb3.Push(cb);
                }
                    
                sb_sr.Empty(); sb_sr.Push("$CLASS$").Push(it()->m_name);
                if( !adv_lib )
                    fs.SubstSection("CreateDynObj-PerType-Body",sb_sr,sb_ob);
                else{
                    // Fix for template types
                    cb = it()->m_name;
                    UnTemplate( cb.Str(), false );
                    sb_sr.Push("$CLASSFUNC$").Push(cb);
                    fs.SubstSection("CreateDynObj-PerType-Body-Adv",sb_sr,sb_ob);
                    printf("PerType-Body adv-lib\n");
                }
                sb3.Push(sb_ob);
            }

            if( !adv_lib )
                sb3.Push( fs.GetSection("CreateDynObj-End",b_ok) );
            else
                sb3.Push( fs.GetSection("CreateDynObj-EndAdv",b_ok) );
            sb3.Push("");
            
            // Win32 specifics (DllMain)
            sb3.Push( fs.GetSection("WIN32",b_ok) );
            sb3.Push("");

            // The ExitDynLib function
            sb3.Push( fs.GetSection("Exit",b_ok) );

            // Close section		
            sb3.Push("// %%DYNOBJ section end");
            sb3.Push("");
        }
    }

	
	// Print result to stdout or file
	pf = stdout;
	po = GetOpt(argc,argv,"-o");
	if( po ){
		if( *po )pf = fopen(po,"w");
		else pf = fopen(argv[1],"w");
		if( !pf ){
			printf("Failed creating output file");
			return -2;
		}
	}
	// Put the general section first, it gives a good overview
	if( n_new_sect>=3 ){
		int ix = afs.FindIndexOf(sGen);
		if( ix>0 ){
			pfs = afs[ix];
			afs.RemoveIndexOrdered(ix);
			afs.InsertOrdered(pfs,0);
		}
	}
	
	// We want to catch the last #endif and put it last in the file
	// Write to main output file
	int ix_last_endif = -1;
	for( int ix=0; ix<afs.Size(); ix++ ){
		// Skip section?
		if( afs[ix]->m_sect!=sNone && !(sections&(1<<(afs[ix]->m_sect-1))) ) continue;
		for( int jx=0; jx<afs[ix]->m_sb.Size(); jx++ ){
			const char *pc = afs[ix]->m_sb[jx];
			if( last_endif!=pc || !is_header )
				fprintf( pf, "%s\n", pc );
			else {
				ix_last_endif = jx;
				pfs = afs[ix];
				break;
			}
		}
	}
	if( ix_last_endif>=0 ){
		for( int jx=ix_last_endif; jx<pfs->m_sb.Size(); jx++ ){
			const char *pc = pfs->m_sb[jx];
			fprintf( pf, "%s\n", pc );
		}
	}
	if( pf!=stdout ) fclose(pf);

	// Write to separate files if requested
	if( file_sections ){
		po = GetOpt(argc,argv,"-o");
		cb = po ? po : argv[1];	// Initial filename
		if( pc=(char*)strchr(cb,'.') ) *pc=0;
		int fnl = strlen(cb);
		for( int ix=0; ix<afs.Size(); ix++ ){
			// Skip section?
			if( afs[ix]->m_sect==sNone || !(file_sections&(1<<(afs[ix]->m_sect-1))) ) continue;
			if( afs[ix]->m_sect==sGen )
				cb += "-gen.h";
			else if( afs[ix]->m_sect==sUse )
				cb += "-use.cpp";
			else if( afs[ix]->m_sect==sImpl )
				cb += "-impl.cpp";
			else if( afs[ix]->m_sect==sLib )
				cb += "-lib.cpp";
			pf = fopen(cb,"w");
			cb.Get()[fnl] = 0;
			if( !pf ){
				printf("Failed creating output file: %s", cb.Get());
				return -2;
			}

			// These are used as locks in header file, but are not needed in cpp file
			cb = "DO_IMPLEMENT_";
			cb += FileNamePart(argv[1]);
			ToDefineSymbol( (char*)cb.Get()+10 );
			CharBuf cb1 = "DO_USE_";
			cb1 += FileNamePart(argv[1]);
			ToDefineSymbol( (char*)cb1.Get()+10 );
			// Iterate 
			for( int jx=0; jx<afs[ix]->m_sb.Size(); jx++ ){
				const char *pc = afs[ix]->m_sb[jx];
				if( !strstr(pc,cb) && !strstr(pc,cb1) )
					fprintf( pf, "%s\n", pc );
			}
			fclose(pf);
		}
	}

	//int nerr = sq_checkmem( );
	
	// Done!
	afs.DeleteAll();
	acl.DeleteAll();
	
	return 0;
}


