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
#include<atomic>
#include<fstream>
enum LOG_LEVEL {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    FATAL,
};
class LogStream;
template<size_t log_level>
class JsonLog; 
extern thread_local   LogStream       g_log;
extern thread_local   JsonLog<DEBUG>  g_jsonlogDebug;
extern thread_local   JsonLog<INFO>   g_jsonlogInfo;
extern thread_local   JsonLog<WARNING>g_jsonlogWaring;
extern thread_local   JsonLog<ERROR>  g_jsonlogError;
extern thread_local   JsonLog<FATAL>  g_jsonlogFatal;
static const char* LEVEL_MSG[]={
    "DEBUG",
    "INFO",
    "WARNING",
    "ERROR",
    "FATAL"
};
struct LogInfoBase{
    virtual std::string  FmtLog()=0;
};

struct LogInfo :public LogInfoBase {
    std::string   m_filename;
    std::string   m_functionname;
    unsigned int  m_line; 
    LOG_LEVEL     m_level;
    std::string   m_msg;
    uint64_t      m_time;
    unsigned      m_threadId;
    virtual std::string FmtLog()override;
};
struct JosnLogInfo:public LogInfoBase{
    std::unique_ptr<nlohmann::json> m_json;
    virtual std::string FmtLog()override;
};
enum LogSwitchType
{
    DateChangeWay,
    SizeChangeWay,
    DefaultLogSize = 1024*1024*50,
};
struct LogFile{
    /* 日志由两种方式来决定切换 一种是日期，切日就重新打开一个新的文件，一种是有LogSize决定，文件大小超过某个值就重新记录 */
    LogFile( const std::string& filename, LogSwitchType  mode=DateChangeWay, uint64_t Loglimitsize=-1   );
    ~LogFile();
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
};
struct LogStream:public std::ostringstream{
    template<typename T,typename...Args>
    LogStream& log(const T& t, Args...args);
};
template< size_t  log_level>
struct JsonLog{
    std::unique_ptr<nlohmann::json> m_json;
    /* json日志  */
    JsonLog& Str( const std::string& key, const std::string& value  );
    template<typename NumberType>
    JsonLog& Number( const std::string& key, NumberType value ); 
    template<typename ArrayType>//vector  array set ... 
    JsonLog& Array( const std::string& key, const ArrayType& array );
    template<typename T,size_t Size> // C style array
    JsonLog& Array( const std::string& key, const T(&array)[Size] );

    /* json日志由这个函数来处罚 flush */
    JsonLog& Msg( const std::string& value   );
    JsonLog& Msg();
};
class Logger
    :public Singleton<Logger>,public ThreadOper<std::shared_ptr<LogInfoBase>>
{
    public:
        void Init( const std::string& filename  );
        void SetLogLevel( LOG_LEVEL  log_level );
        void LoopRun()override;
        bool ShouldLog( LOG_LEVEL log_level );
    private:
        std::shared_ptr<LogFile>         m_file=nullptr;
        std::atomic<unsigned int>        m_level;
};
#define gLog Logger::Instance()

//可变模板参数展开
#define LogS(out) static_cast<std::ostream&>(out)
template<typename T>
LogStream& operator<<( LogStream& out, const T& t )
{
    LogS(out)<< t;
    return out;
}
LogStream& operator<<( LogStream& out,  char const * ptr  )
{
    if(  ptr )
        LogS(out)<<ptr;
    else
        LogS(out)<<"nullptr";
    return out;
}
LogStream& operator<<( LogStream& out, char*ptr  )
{
    if(  ptr )
        LogS(out)<<ptr;
    else
        LogS(out)<<"nullptr";
    return out;
}
template<typename T>
LogStream& operator<<( LogStream& logstream, const std::vector<T>v )
{
    LogS(logstream)<<"[ ";
    std::for_each( v.begin(),v.end(),[&](const T& e ){ LogS(logstream)<<e<<" "; });
    LogS(logstream)<<"]";
    return logstream;
}
template<typename T>
LogStream& operator<<( LogStream& logstream, const std::set<T>v )
{
    LogS(logstream)<<"[ ";
    std::for_each( v.begin(),v.end(),[&](const T& e ){ LogS(logstream)<<e<<" "; });
    LogS(logstream)<<"]";
    return logstream;
}
template<typename T,size_t Size>
inline LogStream& operator<<( LogStream& out, const  T(&array)[Size] )
{
    static_cast<std::ostream&>(out)<<"[ ";
    for( size_t i=0;i<Size;++i )
        LogS(out)<<array[i]<<" ";
    LogS(out)<<"]";
    return out;
}
#if 0
template<typename T,typename...Args>
LogStream& LogStream::log( const T& t, Args...args )
{
    int initlise[]={( *this<<args,0 )...};
}
#endif

 #define  StreamLog( LOG_LEVEL, filename, FUNCTION_NAME, LINE, ... )  \
     do{                                         \
     g_log.str("");                              \
     auto log_info = std::make_shared<LogInfo>();\
     assert(log_info.get() !=nullptr );          \
     log_info->m_functionname=FUNCTION_NAME;     \
     log_info->m_filename=filename;              \
     log_info->m_line=LINE;                      \
     log_info->m_level=LOG_LEVEL;                \
     log_info->m_time = now_ms();                \
     log_info->m_threadId = GetThreadId();       \
     g_log<<__VA_ARGS__;                         \
     log_info->m_msg =  g_log.str() ;            \
     gLog.Push( log_info );        \
 }while(0)

#define SLOG_DEBUG(...)   StreamLog(DEBUG, __FILE__, __FUNCTION__, __LINE__,__VA_ARGS__ )
#define SLOG_INFO(...)    StreamLog(INFO, __FILE__, __FUNCTION__, __LINE__,__VA_ARGS__ )
#define SLOG_ERROR(...)   StreamLog(ERROR, __FILE__, __FUNCTION__, __LINE__,__VA_ARGS__ )
#define SLOG_WARNING(...) StreamLog(WARNING, __FILE__, __FUNCTION__, __LINE__,__VA_ARGS__ )
#define SLOG_FATAL(...)   StreamLog(FATAL, __FILE__, __FUNCTION__, __LINE__,__VA_ARGS__ )





#endif
