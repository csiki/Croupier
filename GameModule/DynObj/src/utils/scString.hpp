
#ifndef SCSTR_HPP
#define SCSTR_HPP

// A simple string type for Ansi/Unicode
// Expects a type scChar to be defined
// Also expects that ExpArr.hpp is included at this point

#include <string.h>
#include <stdio.h>
#include <ctype.h>


#ifndef SCCHAR_TYPEDEF
	#ifndef scChar
		#warning scChar not defined, defaulting to char
		#define scChar char
	#endif	
#endif

#ifndef scInt 
	// For string length
	#define scInt int
#endif

/*
#ifndef EA_ASSERT
	#define EA_ASSERT assert
	#include <assert.h>
#endif
*/
#include "ExpArr.hpp"

#include "sc.h"

#ifndef STR_AT_END 
	#define STR_AT_END ((int)0x80000000)
#endif



class scString : public ExpArr<scChar,scInt> {
public: 
	scString( const scChar *ps=NULL ) : ExpArr<scChar,scInt>(ps,ps?(int)scstrlen(ps):0) { 
        Push((scChar)0);
	}
	scString( const scString &s ) : ExpArr<scChar,scInt>(s) { }
	scString( const scChar *ps, int len ) : ExpArr<scChar,scInt>( ps, len<0? (ps?scstrlen(ps):0) :len ) { Push(0); }
	// Concatenation constructor
	scString( const scChar *ps1, const scChar *ps2, int l1=-1, int l2=-1 ) { 
		if(l1==-1) l1 = ps1?(int)scstrlen(ps1):0; 
		if(l2==-1) l2 = ps2?(int)scstrlen(ps2):0; 
		MinTotalSize(l1+l2+1);
		Push( ps1,l1 );
		Push( ps2,l2 );
		Push(0);
	}
	// Construct from an interval (begin end)
	scString( bool dummy, const scChar *ps, const scChar *pe) : ExpArr<scChar,scInt>() { Assign(ps,pe); }
	
	// To allow quick formatting of an int
	scString( bool dummy, int i, const char *fmt=NULL ){ char buf[64]; sprintf(buf,fmt?fmt:"%d",i); Assign(buf); } 
	
	// To allow quick formatting of a double
	scString( bool dummy, double d, const char *fmt=NULL ){ char buf[64]; sprintf(buf,fmt?fmt:"%e",d); Assign(buf); } 
	
	scString &Assign(const scChar *ps, int l=-1){ 
	    ExpArr<scChar,scInt>::Empty();
	    Push(ps, ps ? (l<0?(int)scstrlen(ps):l) : 0);
	    Push((scChar)0);
	    return *this;
	}

	scString &Assign(const scChar *ps, const scChar *pend ){ 
		return Assign( ps, ps && pend ? pend-ps : -1 );
	}

	scString &operator = (const scString &s){ 
		return Assign(s,s.Length());
	    //ExpArr<scChar,scInt>::Empty();
	    //Push(s.Base(),s.ExpArr<scChar,scInt>::Size());
	    //return *this;
	}

	scString &operator = (const scChar *ps){
		return Assign(ps,-1);
	    //ExpArr<scChar,scInt>::Empty();
	    //Push(ps,ps?scstrlen(ps):0);
	    //Push((scChar)0);
	    //return *this;
	}

	operator const scChar*() const { return ta; }

	scString &operator += (const scChar *ps ){ 
		return Append(ps,-1);
	    //if(!ps) return *this;
	    //Pop();
	    //Push(ps,scstrlen(ps)+1);
	    //return *this;
	}

	scString &operator += (const scString &s ){ 
	    Pop();
	    Push(s.Base(),s.ExpArr<scChar,scInt>::Size());
	    return *this;
	}

	scString &Insert(int at, const scChar *ps, int len=-1){ 
	    if(!ps || !len) return *this;
	    if( at==(int)STR_AT_END ) at = cur_size-1;
	    else{
		    at %= cur_size;	// Including NUL
	    	if( at<0 ) at += cur_size-1;
	    }
	    ExpArr<scChar,scInt>::Insert( ps, len<0?(int)scstrlen(ps):len, at );
	    return *this;
	}
	
	scString &Append(const scChar *ps, int len=-1){ 
		return Insert(STR_AT_END,ps,len);
	}

	scString &Append(scChar ch){ 
		ta[cur_size-1] = ch;
	    Push((scChar)0);
	    return *this;
	}

	scString &Prepend(const scChar *ps, int len=-1){
		return Insert(0,ps,len);
	}

	scString &Prepend(scChar ch){ 
	    InsertOrdered( ch,0 );
	    return *this;
	}

	scString &operator += (const scChar ch ){ 
		return Append(ch);
	}

/*
   scString operator + (const scChar *ps) {
   	return scString(ta,ps,cur_size-1);
   }
*/
	bool operator == (const scChar *ps ) const { 
	    if(!ps) return !Length(); 
	    return !scstrcmp(ta,ps);
	}
	bool operator == (scChar *ps ) const { 
	    if(!ps) return !Length(); 
	    return !scstrcmp(ta,ps);
	}

	bool operator != (const scChar *ps ) const { 
	    return ! operator == (ps);
	}

	// Use base case
	//scChar &operator [] (int ix) { EA_ASSERT(ix<Length()); return Elem(ix); } 

	int Length() const { return cur_size-1; }
	int Size() const { return cur_size-1; }   // Basically to hide ExpArr::Empty
	// Basically to hide ExpArr::SetSize
	bool SetSize( int len ) { 
		bool b = ExpArr<scChar,scInt>::SetSize(len);
		if( !b ) return false;
		Push(0);
		return true;
	}   


	scString &operator += (int i ){
		scChar buf[16]; 
		sprintf(buf,_SC("%d"),i); 
		return *this+=buf; 
	} 

	scString &operator += (double d ){
		scChar buf[32]; 
		sprintf(buf,_SC("%f"),d); 
		return *this+=buf;
	} 

	scString &operator = (int i ){
		scChar buf[16]; 
		sprintf(buf,_SC("%d"),i); 
		return *this=buf; 
	} 

	scString &operator = (double d ){
		scChar buf[32]; 
		sprintf(buf,_SC("%f"),d); 
		return *this=buf;
	} 
	
	scString &Trunc(int pos){ 
	    if( pos<0 ) pos = Length()+pos;
	    if( pos<0 || pos>=Length() ) EA_THROW_RV("scString::Trunc - Invalid pos", *this);
	    //EA_ASSERT(pos>=0 && pos<Length()); 
	    cur_size = pos;
	    Push((scChar)0);
	    return *this;
	}

	scString &SetLength( int len ) { 
	    if(len<=Length()) return Trunc(len);
	    MinTotalSize( len+1 );
	    cur_size = len;
	    Push((scChar)0);
	    return *this;
    }

	scChar *Right( int from_pos ){
	    if( from_pos<0 ) from_pos = Length()+from_pos;
	    if( from_pos<0 || from_pos>=Length() ) return NULL;
		return ta+from_pos;
	}

	scString &Strip( bool left=true, bool right=true ) { 
	    int p_left=0, p_right=cur_size-2;
	    if( left )
	    	for( ; p_left<cur_size && scisspace(ta[p_left]); p_left++ );
	    if( right )
	    	for( ; p_right>=0 && scisspace(ta[p_right]); p_right-- );
	    if( p_left>p_right )
	        return Reset();
        if( p_left )
            memmove(ta,ta+p_left,p_right+1-p_left);
        ta[p_right+1-p_left] = (scChar)0;
        cur_size = p_right+2-p_left;
        return *this;
    }

	scString &Reset( ){ 
	    ExpArr<scChar,scInt>::Empty();
	    Push((scChar)0);
	    return *this;
	}

	scString &Empty( ){ // Basically to hide ExpArr::Empty
	    return Reset();
	}
	
	int Subst( scChar find, scChar repl ){
		int n_found = 0;
        for( int ix=cur_size-2; ix>=0; ix-- )
            if( ta[ix]==find ){
            	ta[ix]=repl;
            	n_found++;
            }
        return n_found;
	}

	int Subst( const char *find, const char *repl ){
		int n_found = 0;
	    if( find!=repl && find && *find ){
	    	int pos = 0;
	    	int rl = repl?(int)strlen(repl):0;
	    	int dl = rl - (int)strlen(find);
	    	while( 1 ){
	    		char *pc = strstr(ta+pos,find);
	    		if( !pc ) break;
	    		pos = pc-ta;
	    		if( dl<0 )
	    			// Remove space from array
	    			ExpArr<char>::Remove( -dl, pos );
	    		else if( dl>0 )
	    			// Insert space
	    			//ExpArr<char>::Insert(repl,dl,pos);
	    			ExpArr<char>::MakeSpaceAt( pos, dl );
	    		if( rl )
	    			memcpy(ta+pos,repl,rl);
	    		n_found++;
	    		pos += rl;
	    	}
	    }
        return n_found;
	}

	bool FindAt( const scChar *str, int pos=0/*STR_AT_END for find at end*/ ){
		if( !str || !ta || pos>=cur_size ) return false;
		int sl = (int)strlen(str);
		if( pos>=0 )
			return !strncmp(ta+pos,str,sl);
		else{
			if( pos==STR_AT_END ) pos = 0;
			int dp = cur_size+pos-1-sl;
			if( dp<0 ) return false;
			return !strncmp(ta+dp,str,sl);
		}
	}
	
	bool StartsWith( const scChar *psc ){
		return FindAt( psc,0 );
	}
	
	bool EndsWith( const scChar *psc ){
		return FindAt( psc,STR_AT_END );
	}
	
	int Find( const scChar *psc, bool from_end=false ){
		if( !psc ) return -1;
		scChar *ploc = NULL;
		if( !from_end )
			ploc = scstrstr(ta,psc);
		else{
			int sl = scstrlen(psc);
			ploc = ta + cur_size-sl;
			while( ploc>=ta && scstrncmp(psc,ploc,sl) )
				ploc--;
		}
		return ploc && ploc>=ta? int(ploc-ta) : -1;
	}

	scString Slice( int from, int to=STR_AT_END ){
		if( from<0 ) from=cur_size-1+from;
	   if( from<0 || from>=Length() ) EA_THROW_RV("scString::Slice - Invalid from", *this);
		if( to<0 ){
			if( to==STR_AT_END ) to = cur_size-1;
			else to = cur_size+to-1;
		}
	   if( to!=STR_AT_END && (to<0 || to>=Length()) ) EA_THROW_RV("scString::Slice - Invalid to", *this);
	    
	    if( to<=from ) 
	    	return scString();
	    else  
	    	return scString( ta+from, to-from );
	}
	
	scString &Suffix( const scChar *psc ){
		if( !FindAt(psc,STR_AT_END) ) Append(psc);
		return *this;
	}

	scString &Suffix( scChar ch ){
		if( !ta || cur_size<2 || ta[cur_size-2]!=ch ) Append(ch);
		return *this;
	}

	scString &Prefix( const scChar *psc ){
		if( !FindAt(psc,0) ) Prepend(psc);
		return *this;
	}
	
	scString &Prefix( scChar ch ){
		if( !ta || *ta!=ch ) Prepend(ch);
		return *this;
	}

	bool SaveToFile( const scChar *path ){
		FILE *pf = fopen(path,"w");
		if( !pf ) return false;
		fwrite(Base(),scString::Size(),1,pf);
		fclose(pf);
		return true;
	}
			
	bool LoadFromFile( const scChar *path ){
		FILE *pf = fopen(path,"r");
		if( !pf ) return false;
		char buf[256];
		int rl;
		Empty();
		while( (rl=fread(buf,1,256,pf))>0 )
			Append( buf, rl );
		fclose(pf);
		return true;
	}
};

//scString operator + (const scChar *ps1, const scString &s2);
scString operator + (const scString &s1, const scChar *ps2 );
scString operator + (const scString &s1, const scString &s2 );
	
#endif // SCSTR_HPP


