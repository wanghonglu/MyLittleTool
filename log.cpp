#include "log.hpp"
LogFile::LogFile( const std::string& file_name, LogSwitchType mod, uint64_t LogLimitSize  )
    :m_filename(file_name),m_logChangeType(mod),m_logLimitSize(LogLimitSize)
{
    if( m_logChangeType == SizeChangeWay && m_logLimitSize==-1 )
        m_logLimitSize = DefaultLogSize;
    OpenFile();
}
void LogFile::OpenFile()
{
    std::string filename = MakeFileName(); 
    try{
        m_file.open( filename, std::ios_base::app|std::ios_base::out );
    }
    catch( std::exception& err )
    {
        std::cerr<<err.what()<<std::endl;
    }
}
std::string LogFile::MakeFileName()
{
    return m_filename+"_"+DateTimeFmt();
}
void LogFile::TryRollFile()
{
    if( m_logChangeType == SizeChangeWay )
    {
        if( unlikely( m_logSize > m_logLimitSize ) )
        {
            m_file.close();
            OpenFile();
            return ;
        }
    }
    else 
    {

    }
}
