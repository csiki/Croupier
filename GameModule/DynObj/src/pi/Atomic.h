
#ifndef ATOMIC_H
#define ATOMIC_H

#ifdef __cplusplus
extern "C" {
#endif

// Increments integer pointed to by "pi" by "v" 
// Returns updated (new) value.
int AtomicAdd( int *pi, int v );

int AtomicIncrement( int *pi );
int AtomicDecrement( int *pi );

int AtomicExchange( int *pi, int val );


#ifdef __cplusplus
};
#endif

#endif // ATOMIC_H


