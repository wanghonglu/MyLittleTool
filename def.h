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
#include<random>
#include<chrono>
#include<algorithm>
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
int32_t GetRandomNumber()
{
    static std::default_random_engine e(
            std::chrono::system_clock::now().time_since_epoch().count()
            );
    static std::uniform_int_distribution<int32_t>u;

    return u(e);
}
std::string GetRandomString()
{
    static thread_local std::string str="abcdefghijklmnopqrsjuvwxyz@!*&$%+-=;:/.,()[]{}|";
    static std::random_device rd;
    static std::mt19937 engine(rd());

    std::shuffle( str.begin(),str.end(),engine );

    return str;
}



#endif

