
#include <ctype.h>

#define DEFINE_WCHAR_T_AS_INT

#include "utils/utf8String.h"
#include "utils/unicode.h"

int utf8strnicmp( const char *s1, const char *s2, int n ){_CFE_;
	signed char ch1, ch2;
	int dl, wc1, wc2, fwc1, fwc2;
	
	while( true ){
		// Have to do this way since we need both of ch1 and ch2 after loop
		ch1=*s1; ch2=*s2;
		if( !n || !ch1 || !ch2 ) break;
		n--;
		
		// The ascii case (or when one is Ascii)
		if( ch1>0 ){
			if( ch2>0 ){
				dl = tolower(ch1)-tolower(ch2);
				if( dl ) return dl; 
				s1++, s2++;
				continue;
			}
			else return -1;
		}
		else if( ch2>0 )
			return 1;
		
		// Wide char case
		unicode_utf8_to_wchar_(&wc1,s1);
		unicode_utf8_to_wchar_(&wc2,s2);
		if( wc1!=wc2 ){
			fwc1 = unicode_simple_fold_(wc1);
			fwc2 = unicode_simple_fold_(wc2);
			if( fwc1!=fwc2 )
				return fwc1-fwc2;
		}
		dl = unicode_utf8_len_(ch1);
		s1 += dl;
		s2 += dl;
	}
	// One or both ended
	if( !n ) return 0;
	return ch1 ? 1 : (ch2?-1:0);
}

int utf8stricmp( const char *s1, const char *s2 ){_CFE_;
	return utf8strnicmp( s1, s2, 0x7FFFFFFF );
}


// Init and step to given offset (positive or negative)
u8Iter::u8Iter( const char *scp_start, const char *scp_end, int goto_pos ){_CFE_;
	int l = scp_end-scp_start;
	//EA_ASSERT(scp_start && scp_end && l>=0);
	if( !scp_start || !scp_end || l<0 ) EA_THROW("u8Iter::u8Iter - Invalid args");
	
	m_start = scp_start;
	m_end = scp_end;
	
	if( goto_pos>=0 ){
		m_scp = m_start;
		while( goto_pos>0 ){
			(*this)++;
			goto_pos--;
		}
	}
	else{
		m_scp = m_end;
		if( goto_pos!=STR_AT_END )
			while( goto_pos<0 ){
				(*this)--;
				goto_pos++;
			}
	}
}

void u8Iter::operator ++ (int){
	if( !m_end || m_scp<m_end ){
		char ch = *m_scp;
		if( !(ch&0x80) ) m_scp++;
		else m_scp += unicode_utf8_len_(ch);
		m_pos++;
	}
	else
		; // Can check for step after end 
}

void u8Iter::operator -- (int){
	if( !m_start || m_scp>m_start ){
		for( --m_scp; ((*m_scp)&0xC0)==0x80 && (!m_start || m_scp>m_start);  )
			m_scp--; 
		m_pos--;
	}
	else
		m_scp = NULL;	// Detect step before beginning through this
}


int u8Iter::operator () () {_CFE_;
	if( m_scp && (!m_end || m_scp<m_end) ){
		int wc = 0;
		unicode_utf8_to_wchar_( &wc, m_scp );
		return wc; 
	}
	else return -1;
}

// Constructing from a wide char string
u8Str::u8Str( const wchar_t *pws, int wcs_len ) : scString(NULL) {_CFE_;
	Assign( pws, wcs_len );
}

u8Str &u8Str::Assign(const wchar_t *pws, int wcs_len){_CFE_;
	Reset( );
	if( pws ){
		char buf[8];
		int l;
		Pop();	// Trailing zero remove
		while( *pws && wcs_len-- ){
			l = unicode_wchar_to_utf8_(buf,(int)*pws,8);
			if( l<1 || l>6 ) throw "u8Str::u8Str: Failed converting wchar_t to utf8";
			Push(buf,l);
			pws++;
		}
		Push(0);
	}
	return *this;
}

int u8Str::operator [] (int ix) const {
	u8Iter ui(ta,ta+cur_size,ix);
	return ui();
	/*
	if( ix>=0 ){
		u8Iter ui(ta,ta+cur_size);
		if( ix>0 )
			while( ix-- )
				ui++;
		return ui();
	}else{
		u8Iter ui(ta+cur_size,ta+cur_size,ta);
		while( ix++ )
			ui--;
		return ui();
	}
	*/
	/*
	u8Iter u8it(Base());
	while( u8it()!=-1 && u8it.Pos()<ix ) 
		u8it++;
	return u8it();
	*/
} 

bool u8Str::SetUniChar(int ix, int ch){_CFE_;	// Returns one Unicode code-point (32-bit)
	u8Iter u8it(ta,ta+cur_size,ix);
	int ch_old = u8it(); 
	if( ch_old==-1 ) return false;
	if( ch==ch_old ) return true;
	char buf[8];
	int ln = unicode_wchar_to_utf8_(buf,ch,8);
	if( ln<1 || ln>6 ) return false;
	int lo = unicode_utf8_len_(ch_old);
	if( lo<1 || lo>6 ) return false;
	int dl = ln - lo;
	int arr_off = u8it.Base()-ta;
	if( dl<0 )
		// Remove space from array
		ExpArr<char>::Remove( -dl, arr_off );
	else if( dl>0 )
		// Insert space
		ExpArr<char>::Insert(buf,dl,arr_off);
	// Copy new UTF8 char
	memcpy( ta+arr_off, buf, ln );
	return true;
}


// Concatenating a wide char string
u8Str &u8Str::operator += ( const wchar_t *pws ){_CFE_;
	if( pws ){
		char buf[16];
		int l;
		Pop();
		while( *pws ){
			l = unicode_wchar_to_utf8_(buf,(int)*pws,16);
			if( l<1 || l>6 ) throw "u8Str::operator +=: Failed converting wchar_t to utf8";
			Push(buf,l);
			pws++;
		}
		Push(0);
	}
    return *this;
}

u8Str &u8Str::operator += ( const char *ps ){_CFE_;
	return (u8Str&)scString::operator+=(ps);
}

bool u8Str::Insert(int at, int ch){_CFE_;
	char buf[8];
	int l = unicode_wchar_to_utf8_(buf,ch,8);
	if( l<1 || l>6 ) return false;
	scString::Insert(at,buf,l);
	return true;
}

bool u8Str::Prepend(int ch){_CFE_;
	return Insert(0,ch);
}

bool u8Str::Append(int ch){_CFE_;
	return Insert(STR_AT_END,ch);
}


int u8Str::Length() const {
	// Make fast algorithm
	const char *pc = ta, *pc_end = ta+cur_size-1;
	int tl = 0;
	while( pc<pc_end ){
		if( !(*pc&0x80) )
			pc++;
		else{
			int ch = (*pc)<<1;
			pc += 2;
			while( (ch&0xC0)==0xC0 ){
				ch<<=1;
				pc++;
			}
		}
		tl++;
	}
	return tl;
}

const scChar *u8Str::Right( int from_pos ) const {
	u8Iter u8it(ta,ta+cur_size,from_pos);
	return (const scChar*)u8it;
}

u8Str &u8Str::Trunc(int pos){_CFE_;
	if( pos>=cur_size ) return *this;
	
	u8Iter it(ta,ta+cur_size,pos);
	const char *pc = (const char*)it; 
	if( !pc ) return *this;
	cur_size = pc-ta+1;
	ta[cur_size-1] = 0;
	return *this;
}

int u8Str::Find( const char *ps ) const {
	if( !ps ) return -1;
	scChar *ploc = scstrstr(Base(),ps);
	if( !ploc ) return -1;
	
	u8Iter u8it(Base());
	while( u8it()!=-1 && u8it.Base()<ploc ) 
		u8it++;
	return u8it.Pos();
}

bool u8Str::FindAt( const char *str, int pos ) const {
	if( !str || !ta || pos>=Length() ) return false;
	u8Iter u8it(ta,ta+cur_size,pos);
	const char *pc = (const char*)u8it;
	return pc ? !strcmp(pc,str) : false;
}

int u8Str::Subst( int ch_find, int ch_repl ){_CFE_;
	// ...this is awkward 
	
	// Se if ordinary length 1 char case
	int lf = unicode_utf8_len_(ch_find);
	int lr = unicode_utf8_len_(ch_repl);
	if( lf<1 || lr<1 || lf>6 || lr>6 )
		return -1;
	if( lf==lr && lf==1 )
		return scString::Subst((char)ch_find,(char)ch_repl);
	
	// Convert to two char buffers
	char find_buf[8];
	char repl_buf[8];
	unicode_wchar_to_utf8_(find_buf,ch_find,8);
	unicode_wchar_to_utf8_(repl_buf,ch_repl,8);
	find_buf[lf] = 0;
	repl_buf[lr] = 0;
	return scString::Subst(find_buf,repl_buf);
}


u8Str u8Str::Slice( int from, int to ) const {
	u8Iter it_fr(ta,ta+cur_size,from);
	u8Iter it_to(ta,ta+cur_size,to);
	if( it_fr()==-1 || it_fr.m_scp>=it_to.m_scp ) 
		return u8Str();
	else
		return u8Str( it_fr.m_scp, it_to.m_scp-it_fr.m_scp );
}
