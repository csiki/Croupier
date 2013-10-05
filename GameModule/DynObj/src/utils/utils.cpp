
#include <stdio.h>
#include "CharBuf.h"


int ReadLine(FILE *pf, char *buf, int len){_CFE_;
	char ch;
	int p=0;
	while( (ch=fgetc(pf))!=EOF && ch ){
		if( ch=='\n' ){
            if( !p || buf[p-1]!='\\') break;
        }
		if( p==len ) return -1;
		buf[p++] = ch;
	}
	buf[p] = 0;
	return p ? p : (ch==EOF ? -1 : 0);
}

// Resizes CharBuf for long lines
int ReadLine(FILE *pf, CharBuf &cb, int &len){_CFE_;
	char ch;
	int p=0;
	if( cb.Get() ) *cb.Get()=0;
	else { cb=""; len=0; }
	char *buf = cb.Get();
	while( true ){
		if( p==len ){
			int nlen = len ? len*2 : 32;
			if( !cb.Resize( nlen ) )
				return -2;
			len = nlen;
			buf = cb.Get();
		}
		if( (ch=fgetc(pf))==EOF || !ch || ch=='\n' ) break;
		buf[p++] = ch;
	}
	buf[p] = 0;
	return p ? p : (ch==EOF ? -1 : 0);
}


inline volatile long long RDTSC() {_CFE_;
#ifdef __GCC__
   register long long TSC asm("eax");
   asm volatile (".byte 15, 49" : : : "eax", "edx");
   return TSC;
#else
	return 0;
#endif
}


