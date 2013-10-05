
#ifndef UTILS_H
#define UTILS_H

class CharBuf;

int ReadLine(FILE *pf, char *buf, int len);
int ReadLine(FILE *pf, CharBuf &cb, int &len);


#ifdef __GNUC__
inline volatile long long RDTSC();
#endif

#endif 

