#include "def.h"
unsigned int Random(unsigned int max,unsigned int min )
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
std::string DateFmt( char split )
{
    struct timeval tv;
    struct tm _tm;
    gettimeofday(&tv,NULL);
    localtime_r( &tv.tv_sec, &_tm );

    std::stringstream ss;
    ss<<_tm.tm_year+1900<<split<<_tm.tm_mon+1<<split<<_tm.tm_mday;
    return ss.str(); 
}
std::string DateTimeFmtBySeconds(uint64_t millisec, char split,bool WithMillSeconds )
{
    struct tm _tm;
    time_t sec = millisec/1000;
    localtime_r( &sec, &_tm );
    std::stringstream ss;
    ss<<_tm.tm_year+1900<<split<<_tm.tm_mon+1<<split<<_tm.tm_mday<<" ";
    ss<<_tm.tm_hour<<":"<<_tm.tm_min<<":"<<_tm.tm_sec;
    if( WithMillSeconds )
       ss<<" "<<millisec%1000;
     return ss.str();
}
uint32_t GetBeginOfDay()
{
    struct tm now;
    time_t nowSec = time(NULL);
    localtime_r( &nowSec, &now  );
    now.tm_hour= now.tm_min = now.tm_sec =0;
    return mktime(&now);
}
unsigned int GetThreadId()
{
    static thread_local  unsigned int s_threadId=0;
    if( s_threadId !=0  ) return s_threadId;
    return s_threadId = syscall(SYS_gettid);
}
std::string GetProcessName()
{
	std::string _exeName = "/proc/self/exe";
	char exeName[256] = {0};
	if(readlink("/proc/self/exe" , exeName, sizeof(exeName) ) !=-1 )
	{
		_exeName = exeName;
	}
	return _exeName;
}


