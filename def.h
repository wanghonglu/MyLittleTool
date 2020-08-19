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
#include<sstream>
#ifdef __linux__ 
#include<sys/time.h>
#include<sys/syscall.h>
#include<unistd.h>
#elif defined(WIN32)
#include<windows.h>
#endif
//#include <boost/thread/locks.hpp>
//#include <boost/thread/shared_mutex.hpp>
unsigned int Random(unsigned int max=0,unsigned int min=0 );
int32_t GetRandomNumber();
std::string GetRandomString();
uint64_t now_sec();
uint64_t now_ms();
uint64_t C_now_sec();
uint64_t C_now_ms();
std::string DateFmt( char split='-');
uint32_t GetBeginOfDay();
std::string DateTimeFmtBySeconds(uint64_t millisec, char split='-',bool WithMillSeconds=false );
unsigned int GetThreadId();
std::string GetProcessName();


//using WRMUTEX=boost::shared_mutex;
//using W_LOCK=boost::unique_lock< WRMUTEX >;
//using R_LOCK=boost::shared_lock< WRMUTEX >;
#endif
