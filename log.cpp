#include "log.h"
static const char* LEVEL_MSG[]={
     "DEBUG",
     "INFO",
     "WARNING",
     "ERROR",
     "FATAL"
};
LogFile::LogFile( const std::string& file_name, LogSwitchType mod, uint64_t LogLimitSize  )
    :m_filename(file_name),m_logChangeType(mod),m_logLimitSize(LogLimitSize)
{
    if( m_logChangeType == SizeChangeWay && m_logLimitSize==0 )
        m_logLimitSize = DefaultLogSize;
    OpenFile();
}
LogFile::~LogFile()
{
    m_file.close();
}
#include<iostream>
using namespace std;
void LogFile::OpenFile()
{
    std::string filename = MakeFileName(); 
    try{
        m_file.close();
        m_file.open( filename.c_str(), std::ios::app|std::ios::out );
    }
    catch( std::exception& err )
    {
        std::cerr<<err.what()<<std::endl;
    }
    m_time = GetBeginOfDay() + 24*60*60;
}
std::string LogFile::MakeFileName()
{
    return m_filename+"_"+DateFmt()+".log";
}
void LogFile::TryRollFile()
{
    if( m_logChangeType == SizeChangeWay )
    {
        if( unlikely( m_logSize > m_logLimitSize ) )
           OpenFile();
        return ;
    }
    if( unlikely(  now_sec() > m_time  ) )
            OpenFile();

}
void LogFile::WriteFile( const std::string& msg  )
{
    TryRollFile();
    if( !m_file.is_open() )
        OpenFile();
    m_file<<msg<<"\n";
    m_file.flush();
}

void Logger::Init( const std::string& filename  )
{
    m_file.reset( new LogFile(filename) );

}
void  Logger::SetLogLevel( LOG_LEVEL log_level )
{
    m_level = log_level;
}
void Logger::LoopRun()
{
    SignNameForThread( "LoggerThread" );
    while( !IsStoped() )
    {
        std::shared_ptr< std::list< std::shared_ptr<LogInfoBase>> > ptr = Pop();
        if( ptr == nullptr  )
            continue;
        cout<<"fuck"<<endl;
        while( !ptr->empty() && m_file )
        {
            auto ptrMsg = ptr->front();
            m_file->WriteFile( ptrMsg->FmtLog() );
            ptr->pop_front();
        }
    }
}
bool Logger::ShouldLog( LOG_LEVEL log_level  )
{
    return log_level>=m_level;
}


thread_local   LogStream       g_log;
thread_local   JsonLog         g_jsonlog; 

std::string LogInfo::FmtLog()
{
    std::stringstream ss;
    ss<<m_threadId<<"\t"<<LEVEL_MSG[m_level]<<"\t";
    ss<<DateTimeFmtBySeconds(m_time)<<"\t";
    auto pos = m_functionname.find_last_of("/");
    if( std::string::npos !=pos  )
        m_functionname = m_functionname.substr( pos+1 );
    ss<<"[  "<<m_filename<<" "<<m_functionname<<"-"<<m_line<<" ]\t";
    ss<<m_msg;
    return std::move(ss.str());
}
JsonLog& JsonLog::LogBegin(LOG_LEVEL level, const char* filename, const char*functionname, unsigned int line )
{
    m_bshouldlog = gLog.ShouldLog(level);
    if( m_bshouldlog  )
        m_json={{"file",filename},{"level",LEVEL_MSG[level] },{"func", functionname}, {"line", line}};
    return *this;
}
void JsonLog::Msg( const std::string&value )
{
    if(!m_bshouldlog)
        return ;
    if( !value.empty() )
        m_json["Msg"]=value;
    std::shared_ptr<JsonLogInfo> log_ptr =std::make_shared<JsonLogInfo>();
    log_ptr->m_json.swap(m_json);
    gLog.Push(log_ptr);
}
void JsonLog::Msg()
{
    if( !m_bshouldlog )
        return;
    Msg("");
}
std::string JsonLogInfo::FmtLog()
{
    return std::move( m_json.dump() );
}