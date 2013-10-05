
#ifdef WXWIDGETS
    #include "wx/wx.h"
    #include "utils/unicode-fd32.h"
#endif

// Mainly for NULL
#include <stdlib.h>

// Simple functions to handle conversion to and from Unicode/UTF8
// and also to do folding (for case insensitive string comparing)
// 
// The file fold_tbl.c is needed for case folding. 

int unicode_utf8_len_(char lead_byte){_CFE_;
    int lb = lead_byte;
    if( !(lb&0x80) ) return 1;
    if( (lb&0xE0)==0xC0 ) return 2;
    if( (lb&0xF0)==0xE0 ) return 3;
    if( (lb&0xF8)==0xF0 ) return 4;
    if( (lb&0xFC)==0xF8 ) return 5;
    if( (lb&0xFE)==0xFC ) return 6;
    return -1;
}

int unicode_utf8_to_wchar_(int *result, const char *string){_CFE_;
    int res=-1;
    #ifdef WXWIDGETS
    unicode_utf8_to_wchar( (wchar_t*)&res, string, 4 );
    #endif
    
    // Assume pointers to be OK
    unsigned char ch = *string;
    int l = unicode_utf8_len_(ch);
    
    if( l<1  ) return -1;
    int wc = l>1 ? (ch&(0x7F>>l)) : ch;
    while( l>1 ){
        wc = (wc<<6) + (*++string & 0x3F);
        l--;
    }
    *result = wc;
    
    #ifdef WXWIDGETS
    wxASSERT( res==wc );
    #endif
    
    return 0;
}

// Checks if output fits in 1/4/6 bytes buffer
int unicode_wchar_to_utf8_(char *s, int wc, int size){_CFE_;
    if( size<1 ) return -1;
    if( (unsigned int)wc>=0x80000000 ) return -2;
    
    // Single byte case
    if( wc<0x80 ){
        *s = (char)wc;
        //*s = (char)wc&0x7F;
        return 1;
    }
    if( size<4 ) return -3;
    // Two or more bytes
    int p = 1;
    if( wc>0x7FF ){ // 11 bits
        // Three or more
        p++; // p=>2
        if( wc>0xFFFF ){    // 16 bits
            // Four or more
            p++; // p=>3
            if( wc>0x1FFFFF ){  // 21 bits
                // Five or more
                if( size<6 ) return -3;
                p++; // p=>4
                if( wc>0x3FFFFFF ){ // 26 bits
                    // Six chars
                    p++; // p=>5
                    if( wc>0x1FFFFF ){  // 31 bits
                        s[p-4] = 0x80 | ((wc>>24)&0x3F);    // Bit 24..29
                    }
                s[p-3] = 0x80 | ((wc>>18)&0x3F);    // Bit 18..23
                }
            s[p-2] = 0x80 | ((wc>>12)&0x3F);    // Bit 12..17
            }
        s[p-1] = 0x80 | ((wc>>6)&0x3F);    // Bit 6..11
        }
    }
    s[p] = 0x80 | (wc&0x3F);    // Bit 0..5
    s[0] = (0xFC << (5-p)) | (wc>>(p*6));
    
    
    #ifdef WXWIDGETS
    char buf[8];
    int l = unicode_wchar_to_utf8( buf, wc, 8 );
    wxASSERT( !strncmp(buf,s,p+1) && p+1==l );
    #endif
    
    return p+1;
}


extern const unsigned short *g_uc_fold_pages [];
extern const unsigned int g_uc_fold_page_260 [];

int unicode_simple_fold_( int wc ){_CFE_;
    int page = wc>>8;
    const unsigned short *ps = NULL;
    if( page<256 ){
        ps = g_uc_fold_pages[page];
        return page ? (int)ps[wc&0xFF] : wc;
    }
    else if( page==260 )
        return g_uc_fold_page_260[wc&0xFF];
    else
        return wc;
}

#include "fold_tbl.c"

