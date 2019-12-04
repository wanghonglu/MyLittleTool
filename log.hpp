/*
 * 一个简单的多线程日志类，基于流方式的打印
 * */
#include<string>
#include "Singleton.hpp"
#include "ThreadOper.hpp"
#include<sstream>
#include<utility>
#include<set>
#include<vector>
#include<algorithm>
enum LOG_LEVEL {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    FATAL,
};
static const char* LEVEL_MSG[]={
    "DEBUG",
    "INFO",
    "WARNING",
    "ERROR",
    "FATAL"
};
struct LogInfo{
    const char*   m_filename;
    const char*   m_functionname;
    unsigned int  m_line; 
    LOG_LEVEL     m_level;
    std::string   m_msg;
    uint32_t      m_time;
    unsigned      m_threadId;
};
struct LogStream:public std::ostringstream{
    template<typename T,typename...Args>
    LogStream& log(const T& t, Args...args);
};
class Logger
    :public Singleton<Logger>,public ThreadOper<std::shared_ptr<LogInfo>>
{

    private:
        std::string       m_filename;//file_name_xxxx_xx_xx.log
        LOG_LEVEL         m_level;
};

//可变模板参数展开
template<typename T>
LogStream& operator<<( LogStream& logstream, const T& t )
{
    logstream<<t;
    return logstream;
}
template<typename T>
LogStream& operator<<( LogStream& logstream, const std::vector<T>v )
{
    logstream<<"[ ";
    std::for_each( v.begin(),v.end(),[&](const T& e ){ logstream<<e<<" "; });
    logstream<<"]";
}
template<typename T>
LogStream& operator<<( LogStream& logstream, const std::set<T>v )
{
    logstream<<"[ ";
    std::for_each( v.begin(),v.end(),[&](const T& e ){ logstream<<e<<" "; });
    logstream<<"]";
}
template<typename T,size_t Size>
LogStream& operator<<( LogStream& logstream, const  T(&array)[Size] )
{
    logstream<<"[";
    for( int i=0;i<Size;++i )
        logstream<<array[i]<<" ";
    logstream<<"]";
}
template<typename T,typename...Args>
LogStream& LogStream::log( const T& t, Args...args )
{
    int initlise[]={( *this<<args,0 )...};
}
