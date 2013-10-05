#ifndef UTF8STRING_H_
#define UTF8STRING_H_


// Want 8-bit string
#define scChar char
#define SC_FORCE_ANSI	// We want Ansi version of sc.h regardles of Unicode build or not
#ifdef SC_H
	#undef SC_H // Make fresh include
#endif
#include "utils/ExpArr.hpp"
#include "utils/scString.hpp"

int utf8stricmp( const char *s1, const char *s2 );
int utf8strnicmp( const char *s1, const char *s2, int n );
// UTF8 strcmp works as ordinary strcmp
//int utf8strcmp( const char *s1, const char *s2 );
//int utf8strncmp( const char *s1, const char *s2, int n );

// This is a string class that works with UTF8 encoded strings.
// 
class u8Str : public scString {
	public:
		u8Str( const char *ps=NULL ) : scString(ps) { }
		u8Str( const scString &s ) : scString(s) { }
		u8Str( const char *ps, int byte_len ) : scString(ps,byte_len) { }
		u8Str( const wchar_t *pws, int wcs_len=-1 );
	
		u8Str& Assign(const wchar_t *s, int l);		
		// Compiler cannot see it
		u8Str& Assign(const char *s, int l){ scString::Assign(s,l); return *this; }		
		u8Str& Assign(const char *s, const char* pe){ scString::Assign(s,pe); return *this; }		
		
		// Note: have to step from the beginning at each call
		// Return Unicode character at Unicode offset ix
		int operator [] (int ix) const ;
		// Assign Unicode character at position ix value ch
		bool SetUniChar( int ix, int ch );
		
		u8Str& operator += (const wchar_t *pws);
		u8Str& operator = (const wchar_t *pws){ return Assign(pws,-1); }
		u8Str& operator += (const char *ps);	// # Need this? Yes, compiler does not see base case

		// Insert/append/prepend Unicode character
		bool Insert(int at, int ch);
		bool Append(int ch);
		bool Prepend(int ch);	
		// Compiler cannot see base cases any more
		u8Str& Insert(int at, const char *str, int len=-1){ 
			scString::Insert(at,str,len); 
			return *this;
		} 
		u8Str& Append(const char *str, int len=-1){ 
			scString::Append(str,len); 
			return *this;
		} 
		u8Str& Prepend(const char *str, int len=-1){ 
			scString::Prepend(str,len); 
			return *this;
		} 
		
		int Length() const;			// Note: have to step from the beginning
		u8Str &Trunc(int pos);
		// We cannot reserve requested length with Utf8
		// (we can still use base class MinTotalSize to reserve buffer size though)
		u8Str &SetLength( int len ){ EA_THROW_RV("u8Str::SetLength - Not available for UTF8 encoding",*this); }
		const scChar *Right( int from_pos ) const;

		// This functions returns Unicode position.
		// # Better to return Byte position and provide function to 
		// convert between Unicode and Byte index.
		int Find( const char *ps ) const;		// Returns position of found instance
		bool FindAt( const scChar *str, int pos=0/*STR_AT_END for find at end*/ ) const;
			
		int Subst( int ch_find, int ch_repl );
		int Subst( const char *find, const char *repl ){ return scString::Subst(find,repl); }
		
		u8Str Slice( int from, int to=0 ) const;
};

class u8Iter{
	public:
		u8Iter( const char *scp=NULL, const char *scp_end=NULL, const char *scp_start=NULL ) : 
			m_scp(scp), m_end(scp_end), m_start(scp_start), m_pos(0) { }
		
		// Initialize both ends, and step to offset (positive or negative)
		// STR_AT_END means to put the iterator pointing at scp_end (usually terminating NUL)
		u8Iter( const char *scp_start, const char *scp_end, int goto_pos );
		
		void Init( const char *scp, const char *scp_end=NULL, const char *scp_start=NULL ){
			m_scp=scp; m_end=scp_end; m_start=scp_start; m_pos=0; 
			if( !m_scp ) EA_THROW("u8Iter::Init - Invalid starting point (m_scp)");
		}
		void operator ++ (int);	// Step forwards, postfix: it++
		void operator -- (int); // Step backwards, postfix: it--
		int operator () ();		// Return current character, -1 if none
		int Pos( ){ return m_pos; }	// Return position (distance to 0) in string (not offset)
		const char *Base( ){ return m_scp; }
		operator const char *(){ return m_end && m_scp>=m_end ? NULL : m_scp; }
		operator char *(){ return m_end && m_scp>=m_end ? NULL : (char*)m_scp; }  		
		
		const char *m_scp, *m_start, *m_end;
		int m_pos;	// A negative value when stepping backwards
};


#endif /*UTF8STRING_H_*/

