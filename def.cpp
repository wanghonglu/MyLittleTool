#include "def.h"
unsigned int Random(unsigned int max,unsigned int min )
{
#ifdef __linux__
    static bool  seed_init = false;
    if( !seed_init  )
    {
        srandom( time(NULL) );
        seed_init=true;
    }
    if( max ==0 ||   max<min )
        return random();
    
    return min+random()%(max-min);
#else
	if (max <= min)
		return GetRandomNumber();
	return min + GetRandomNumber() % (max - min);
#endif

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
#ifdef __linux__
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec;
#else 
	return now_sec();
#endif
};
uint64_t C_now_ms()
{
#ifdef __linux__
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_usec/1000;
#else
	return now_ms();
#endif
}
std::string DateFmt( char split )
{
	struct tm _tm;
#ifdef __linux__
    struct timeval tv;
    gettimeofday(&tv,NULL);
    localtime_r( &tv.tv_sec, &_tm );
#else //ÔÝÊ±¾ÍÊÇwindows
	time_t  nows = now_sec();
	localtime_s(&_tm, &nows);
#endif

    std::stringstream ss;
    ss<<_tm.tm_year+1900<<split<<_tm.tm_mon+1<<split<<_tm.tm_mday;
    return ss.str(); 
}
std::string DateTimeFmtBySeconds(uint64_t millisec, char split,bool WithMillSeconds )
{
    struct tm _tm;
    time_t sec = millisec/1000;
#ifdef __linux__
    localtime_r( &sec, &_tm );
#else 
	localtime_s(&_tm, &sec);
#endif
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
#ifdef __linux__
    localtime_r( &nowSec, &now  );
#else
	localtime_s(&now, &nowSec);
#endif
    now.tm_hour= now.tm_min = now.tm_sec =0;
    return mktime(&now);
}
unsigned int GetThreadId()
{
#if defined(_MSC_VER) && (_MSC_VER < 1900) || defined(__clang__) && !__has_feature(cxx_thread_local)
    return _thread_id();
#else
    static thread_local const size_t tid = _thread_id();
    return tid;
#endif
}
inline size_t _thread_id()
{
#ifdef _WIN32
    return  static_cast<size_t>(::GetCurrentThreadId());
#elif __linux__
    return  static_cast<size_t>(syscall(SYS_gettid));
#elif __FreeBSD__
    long tid;
    thr_self(&tid);
    return static_cast<size_t>(tid);
#elif __APPLE__
    uint64_t tid;
    pthread_threadid_np(nullptr, &tid);
    return static_cast<size_t>(tid);
#else //Default to standard C++11 (other Unix)
    return static_cast<size_t>(std::hash<std::thread::id>()(std::this_thread::get_id()));
#endif
}
std::string GetProcessName()
{
	std::string _exeName = "/proc/self/exe";
	char exeName[256] = {0};
#ifdef __linux__
	if(readlink("/proc/self/exe" , exeName, sizeof(exeName) ) !=-1 )
	{
		_exeName = exeName;
	}
#endif
	return _exeName;
}


