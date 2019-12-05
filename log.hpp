/*
 * 一个简单的多线程日志类，基于流方式的打印
 * 除了提供正常的日志记录方式外，还提供json格式的日志存储方式
 * */
#ifndef __LOG_HPP__
#define __LOG_HPP__
#include<string>
#include "Singleton.hpp"
#include "ThreadOper.hpp"
#include<sstream>
#include<utility>
#include<set>
#include<vector>
#include<algorithm>
#include "json.hpp" //中文只支持utf-8
#include "def.h"
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
enum LogSwitchType
{
    DateChangeWay,
    SizeChangeWay,
    DefaultLogSize = 1024*1024*50,
}
struct LogFile{
    /* 日志由两种方式来决定切换 一种是日期，切日就重新打开一个新的文件，一种是有LogSize决定，文件大小超过某个值就重新记录 */
    LogFile( const std::string& filename, LogSwitchType  mode=DateChangeWay, uint64_t Loglimitsize=-1   );
    void OpenFile();
    void WriteFile( const std::string& msg  );
    inline void TryRollFile();
    std::string MakeFileName();

    private:
        std::ofstream     m_file;
        std::string       m_filename;
        LogSwitchType     m_logChangeType = DateChangeWay;
        uint64_t          m_time;
        uint64_t          m_logSize;
        uint64_t          m_logLimitSize;
}
struct LogStream:public std::ostringstream{
    template<typename T,typename...Args>
    LogStream& log(const T& t, Args...args);
    nlohmann::json    m_json;
};
class Logger
    :public Singleton<Logger>,public ThreadOper<std::shared_ptr<LogInfo>>
{
    public:
        Logger( const std::string& filename  );
        void SetLogLevel( LOG_LEVEL  log_level );
        void LoopRun()override;

        /* 普通日志 */
        template<typename ...Args>
        void Logging( LOG_LEVEL, const char*filename, const char*functionname, unsigned int line,  Args...args  );

        std::string FormatLog( std::shared_ptr<LogInfo>&  );

        /* json日志  */
        LogStream& Str( const std::string& key, const std::string& value  );
        template<typename NumberType>
        LogStream& Number( const std::string& key, NumberType value ); 
        template<typename ArrayType>//vector  array set ... 
        LogStream& Array( const std::string& key, const ArrayType& array );
        template<typename T,size_t Size> // C style array
        LogStream& Array( const std::string& key, const T(&array)[Size] );

        /* json日志由这个函数来处罚 flush */
        LogStream& Msg( const std::string& value   );
        LogStream& Msg();

    private:
        LogFile                          m_file;
        LOG_LEVEL                        m_level;
        static thread_local LogStream    m_logstream; 
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
#endif
