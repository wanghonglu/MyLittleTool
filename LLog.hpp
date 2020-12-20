#ifndef _L_LOG_H_
#define _L_LOG_H_
#include "windows.h"
#include "def.h"
#include "json.hpp"
#include <iostream>
#include<chrono>
#include<sstream>
#include<functional>
#include<fstream>
#include<memory>
#include<algorithm>
#include<vector>
#include<map>
#include<set>
#include<chrono>
#include<cstdio>
#include<cstdint>
#include<mutex>
#include<thread>
#include<atomic>
#include<condition_variable>
#include<list>
#include<unordered_map>
#include <WinDef.h>
//#include "Common.h"
enum LLOG_LEVEL
{
	LLOG_LEVEL_NULL = 0,//不输入任何的log
	LLOG_LEVEL_FATAL, // 致命log，需要预警的
	LLOG_LEVEL_ERROR,//最高级别的log
	LLOG_LEVEL_INFO,//普通级别的log
	LLOG_LEVEL_DEBUG,//debug级别的log，这个适用于所有调试模式
};
class LLog;
struct StreamOutLog;
extern LLog g_log;
extern LLOG_LEVEL g_loglevel;
extern thread_local StreamOutLog g_logstreamPerThread;
/*
windows下实现染色
*/
#if ((defined(WIN32) || defined(WIN64)) && defined(_DEBUG))
class WinColorOutLog {
public:
	WinColorOutLog();
	void OutToConsole(std::string&fmtLog);
private:
	WORD set_color(WORD attribs);
	void printLog(const std::string& fmtLog, size_t start, size_t end);
	HANDLE out_handle_;
	std::unordered_map<std::string, WORD>  log_level_color_;
};
#endif
//消除一些定义了未使用的告警 ，略烦。。。
#define ERASE_NOTUSED_WARNING(V)  ((void)V)
//增加流方式的写日志方式
/*
针对 指针的输出 或者字符串没有\0结尾的字符串的输出
*/
template<typename T>
class LogView {
public:
	LogView(const T* ptr, size_t len) :m_data(ptr), m_len(len) {}
	const T*         m_data;
	size_t			 m_len;
};
using LogStringView = LogView<char>;
using LogIntView = LogView<int>;
struct StreamOutLog {
public:
	bool    m_haveWrite = true;
	std::ostringstream  m_stream;
	StreamOutLog();
	const char* GetCurrentTimeStr();
private:
	//缓存当前时间戳秒级别 比较如果时间戳相等 就不用调用logictime_r，减少系统调用
	time_t              m_currentTime = 0;
	char                m_timeStr[64];
};

class LLog
{
public:
	LLog();
	~LLog();

	LLog(const LLog&) = delete;
	LLog& operator=(const LLog&) = delete;
	LLog(LLog&&) = delete;
	LLog& operator=(LLog&&) = delete;
	
	//不调用这个 则默认输出到stdout中
	void SetLogFileNameAndStart(const std::string& filename);
	void Stop();
	void Join();
	void AppendLog(StreamOutLog& log );
private:
	std::string MakeLogFileName(time_t timestamp);
	void Run();
	void WriteFile(const std::string& content);
private:
	std::string			m_fileName;
	std::ofstream		m_file;
	time_t              m_nextChangeTime=0;
	std::shared_ptr<std::thread> m_thread;
	std::atomic<bool>   m_stop;

	std::list<std::string> m_list;
	std::mutex			m_mutex;
	std::condition_variable_any m_cond;
#if ((defined(WIN32) || defined(WIN64)) && defined(_DEBUG))
	WinColorOutLog      m_winOutLog;
#endif
};

//增加流方式记录日志
template<class T>
inline StreamOutLog& operator<<(StreamOutLog& out, T const& v)
{
	if (!out.m_haveWrite)
		return out;
	out.m_stream << v;
	return out;
}
inline StreamOutLog& operator<<(StreamOutLog&out, const std::vector<int>& v)
{
	if (!out.m_haveWrite)
		return out;
	out.m_stream << " [ ";
	std::for_each(v.begin(), v.end(), [&out](int val) { out.m_stream << val << " "; });
	out.m_stream << " ] ";
	return out;
}
inline StreamOutLog& operator<<(StreamOutLog&out, const std::set<int>& v)
{
	if (!out.m_haveWrite)
		return out;
	out.m_stream << " [ ";
	std::for_each(v.begin(), v.end(), [&out](int val) { out.m_stream << val << " "; });
	out.m_stream << " ] ";
	return out;
}
template<>
inline StreamOutLog& operator<<(StreamOutLog&out, const nlohmann::json& json)
{
	if (!out.m_haveWrite)
		return out;
	out.m_stream << json.dump();
	return out;
}
template<typename T>
inline StreamOutLog& operator<<(StreamOutLog&out, const LogView<T>& log)
{
	if (!out.m_haveWrite)
		return out;
	out.m_stream << " [ ";
	for (int i = 0; i < log.m_len; ++i)
		out.m_stream << log.m_data[i]<<" ";
	out.m_stream << " ] ";
	return out;
}
template<typename T>
inline StreamOutLog& operator<<(StreamOutLog&out, const std::vector<T>& log)
{
	if (!out.m_haveWrite)
		return out;
	out.m_stream << " [ ";
	for (int i = 0; i < log.size(); ++i)
		out.m_stream << log[i] << " ";
	out.m_stream << " ] ";
	return out;
}

inline StreamOutLog& operator<<(StreamOutLog&out, char const*v)
{
	if (!out.m_haveWrite)
		return out;
	if (v)
		out.m_stream << v;
	else
		out.m_stream << "char const* is NULL";
	return out;
}

inline StreamOutLog& operator<<(StreamOutLog&out, char *v)
{
	if (!out.m_haveWrite)
		return out;
	if (v)
		out.m_stream << v;
	else
		out.m_stream << "char * is NULL";
	return out;
}
//重载输出各种类似C数组的类型
template<typename T, size_t Size>
inline StreamOutLog& operator<<(StreamOutLog&out, const T(&ary)[Size])
{
	if (!out.m_haveWrite)
		return out;
	out.m_stream << "[ ";
	for (int loop = 0; loop < Size; ++loop)
		out.m_stream << ary[loop] << " ";
	out.m_stream << " ] ";
	return out;
}
//这个只支持基础类型
#define LVAL(v) " "#v" = "<<(v)
#define NAME2STR(var) (#var)

struct LogPrint {
public:
	explicit LogPrint(const char*filename, const char* function,size_t line, LLOG_LEVEL level )noexcept;
	~LogPrint()noexcept;
	static StreamOutLog& Stream();
	static size_t ThreadId();
};
#define LOG_DEBUG LogPrint(__FILE__,__FUNCTION__,__LINE__,LLOG_LEVEL_DEBUG).Stream()
#define LOG_INFO  LogPrint(__FILE__,__FUNCTION__,__LINE__,LLOG_LEVEL_INFO).Stream()
#define LOG_ERROR LogPrint(__FILE__,__FUNCTION__,__LINE__,LLOG_LEVEL_ERROR).Stream()
#define LOG_FATAL LogPrint(__FILE__,__FUNCTION__,__LINE__,LLOG_LEVEL_FATAL).Stream()

#define Log_SetFileNameAndStart(filename ) g_log.SetLogFileNameAndStart(filename)
#define Log_SetLogLevel(level) g_loglevel = level;
#define Log_Stop() g_log.Stop()
#define Log_Join() g_log.Join()

#endif