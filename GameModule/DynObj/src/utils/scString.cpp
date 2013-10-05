
// There's a small problem here. If we would like to have 
// two scChar classes in the project (one with char, one with
// wchar_t), then we still only have the one implementation
// below.

#ifndef scChar 
    #define scChar char
#endif

#include "utils/scString.hpp"


scString operator + (const scString &s1, const scChar *ps2){_CFE_;
	return scString(s1,ps2,s1.Length());
}

scString operator + (const scString &s1, const scString &s2){_CFE_;
	return scString(s1,s2,s1.Length(),s2.Length());
}

