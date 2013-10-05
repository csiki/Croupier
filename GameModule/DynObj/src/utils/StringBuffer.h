#ifndef STRINGBUFFER_H_
#define STRINGBUFFER_H_

#ifndef EA_ASSERT
	#define EA_ASSERT assert
#endif
 
#define EA_IS_CLASS
#include "ExpArr.hpp"
#include "CharBuf.h"

// An array of strings (CharBuf) owned by the object
//class StringBuffer : public ExpArrObj<CharBuf,int,SBAlloc,SBFree> {
class StringBuffer : public ExpArrObj<CharBuf,int> {
public:
	StringBuffer( ){ }
	StringBuffer( const ExpArrObj<CharBuf,int>& other) : ExpArrObj<CharBuf,int>(other) { }
	StringBuffer( const ExpArrObj<CharBuf,int>& other, bool is_move ) : ExpArrObj<CharBuf,int>(other,is_move) { }
	
	StringBuffer& Push( const char *str ){
		ExpArrObj<CharBuf,int>::Push(/*cb*/);
		Top() = str;
		return *this;
	}
	StringBuffer& Push( const char *str, int sl ){
		ExpArrObj<CharBuf,int>::Push(/*cb*/);
		Top().Assign(str,sl);
		return *this;
	}

	StringBuffer& Push( const StringBuffer& other){
		ExpArrObj<CharBuf,int>::Push(other);
		return *this;
	}

	const char *Pop( ){
		// Cannot do ordinary Pop with CharBuf
		if( !Size() ) throw "StringBuffer::Pop, Empty stack";
		static CharBuf st_pop;
		st_pop = Top();
		ExpArr<CharBuf,int>::RemoveIndexUnordered( cur_size-1 );
		return st_pop;
	}

protected:
};

// A shorter version
typedef StringBuffer StrBuf;

// Split and join
StrBuf sbSplit( const char *str, const char *seps );
//StrBuf sbSplit( const char *str, const char **seps );
CharBuf sbJoin( const StrBuf &strs, const char sep=0 );

// Type registration, if typeof.h included before us
#ifdef TYPEOF_H
    #undef TYPE_REG_FILE
    #define TYPE_REG_FILE 102
    template<>
    struct IterTypes<StringBuffer> {
        typedef CharBuf e_type;
        typedef int ix_type;
    };
    TYPE_REG(StringBuffer)
#endif


#endif /*STRINGBUFFER_H_*/


