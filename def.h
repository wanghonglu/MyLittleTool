#ifndef __DEF_H__
#define __DEF_H__
#if (defined(__GNUC__) && __GNUC__ >= 3) || defined(__clang__)
#define  unlikely(x)  __builtin_expect(!!(x), 0)
#define  likely(x)    __builtin_expect(!!(x), 1) 
#else
#define  unlikely(x)  (x)
#define  likely(x) (x)
#endif
#include<iostream>
#include<ctime>
#include<cstdlib>
unsigned int Random(unsigned int max=0,unsigned int min=0 )
{
    static bool  seed_init = false;
    if( !seed_init  )
    {
        srandom( time(NULL) );
        seed_init=true;
    }
    if( max ==0 ||   max<min )
        return random();
    
    return min+random()%(max-min);
}


#endif

