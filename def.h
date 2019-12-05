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
#include<sys/time.h>
#include<sstream>
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
uint64_t now_sec()
{
    return std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
}
uint64_t now_ms()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
}
uint64_t C_now_sec()
{
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec;
};
uint64_t C_now_ms()
{
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_usec/1000;
}
std::string DateTimeFmt( char split='-',bool WithMillSeconds=false )
{
    struct timeval tv;
    struct tm _tm;
    gettimeofday(&tv,NULL);
    localtime_r( &tv.tv_sec, &_tm );

    std::stringstream ss;
    ss<<_tm.tm_year+1900<<split<<_tm.tm_mon+1<<split<<_tm.tm_mday<<" ";
    ss<<_tm.tm_hour<<":"<<_tm.tm_min<<":"<<_tm.tm_sec;
    if( WithMillSeconds )
        ss<<" "<<(tv.tv_usec/1000)%1000;
    return ss.str(); 




}



#endif

