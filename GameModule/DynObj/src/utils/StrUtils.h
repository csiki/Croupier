
#ifndef STRUTILS_H
#define STRUTILS_H

#include "pi/platform.h"
#ifndef __WINDOWS__
	#define stricmp   strcasecmp
	#define strnicmp  strncasecmp
#endif

// As strstr but case insensitive
const char *stristr(const char *sbig, const char *ssmall);
// Not reentrant
const char *Int2Str( int i, const char *fmt=NULL );
const char *TruncStr( const char *str, int max_len );

// Look for keyword at str (skips space). If keyword length know, pass along in kwl.
bool keywordieq(const char *str, const char *kw, int kwl=-1);
bool keywordeq(const char *str, const char *kw, int kwl=-1);

// Search for keyword after str
const char *keywordifind(const char *str, const char *kw, const char *pend=NULL, bool advance_past_keyw=true);
const char *keywordfind(const char *str, const char *kw, const char *pend=NULL, bool advance_past_keyw=true);

const char *SkipSpace( const char *pc );
const char *SkipSpaceRev( const char *pc );
const char *SkipToSpace( const char *pc );

// Accept token at str, consume whitespace before and after
const char *StrAccept( const char* str, const char *token );

// Get word before pointer, NOTE!: 0-terminates
char *GetWordRev( char *pc );

// Get word after pointer, NOTE!: 0-terminates
char *GetWordFwd( char *pc );

// Substitute in string, can only do same length or shorter substitutions
int strsubst_shorter( char *str, const char *find, const char *repl, bool do_all=false );


// As strpbrk but reversed
const char *strrpbrk( const char *str, const char *seps );

// Check if string ends/starts with certain word
bool strendswith( const char *str, const char *word );
bool strendswithi( const char *str, const char *word );
bool strstartswith( const char *str, const char *word );
bool strstartswithi( const char *str, const char *word );

// Bounded version of strchr, strstr, stristr 

// Look for ch between str and pend (bounded)
const char *strchr_b( const char *str, char ch, const char *pend );

// Look for ps between str and pend (bounded)
const char *strstr_b( const char *str, const char *ps, const char *pend );

// Look for ps between str and pend (bounded)
const char *stristr_b( const char *str, const char *ps, const char *pend );


// Search for an unescaped version of the character
const char *strchr_noesc( const char *str, const char ch, const char *pend=NULL );
const char *strrchr_noesc( const char *str, const char ch );

// Search for an unqoted version of the character
const char *strchr_nostr( const char *str, char ch, const char *pend=NULL );
const char *strrchr_nostr( const char *str, char ch );

// Search for an unqoted version of the string
const char *strstr_nostr( const char *str, const char *substr, const char *pend=NULL );

// Search for an unqoted version of the string (case insensitive)
const char *stristr_nostr( const char *str, const char *substr, const char *pend=NULL );

#endif // STRUTILS_H

