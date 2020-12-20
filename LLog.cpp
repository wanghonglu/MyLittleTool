#include "LLog.hpp"

LLog::LLog()
{
	m_thread = nullptr;
	m_stop.store(false);
	m_nextChangeTime = 0;
}

LLog::~LLog()
{

}


std::string LLog::MakeLogFileName(time_t timestamp)
{
	struct tm now_tm;
#ifdef _LINUX
	localtime_r(&timestamp, &now_tm);
#else 
	localtime_s(&now_tm, &timestamp);
#endif
	char buf[128] = { 0 };
	snprintf(buf,sizeof(buf), "%04d-%02d-%02d_%s.log", now_tm.tm_year + 1900,now_tm.tm_mon+1,
		now_tm.tm_mday, m_fileName.c_str());
	return std::string(buf);
}
void LLog::SetLogFileNameAndStart(const std::string& filename)
{
	m_fileName = filename;
	time_t  sec = now_ms() / 1000;
	std::string full_filename = MakeLogFileName(sec);
	if (m_file.is_open())
		m_file.close();
	m_file.open(full_filename.c_str(), std::ios::app | std::ios::out);
	if (!m_thread)
		m_thread.reset(new std::thread([this]() {Run();}));
	//计算下一个凌晨的时间
	struct tm now_tm;
#ifdef _LINUX
	localtime_r(&sec, &now_tm);
#else 
	localtime_s(&now_tm, &sec);
#endif
	now_tm.tm_hour = now_tm.tm_min = now_tm.tm_sec = 0;
	m_nextChangeTime = mktime(&now_tm) + 24 * 60 * 60;
}
void LLog::WriteFile(const std::string& content)
{
	time_t  sec = now_ms() / 1000;
	if (sec >= m_nextChangeTime )//切日了
	{
		std::string filename = MakeLogFileName(m_nextChangeTime);
		m_nextChangeTime += 24 * 60 * 60;
		m_file.close();
		m_file.open(filename.c_str(), std::ios::app | std::ios::out);
	}
	if (!m_file.is_open())
	{
		std::string filename = MakeLogFileName(m_nextChangeTime- 24 * 60 * 60);
		m_file.close();
		m_file.open(filename.c_str(), std::ios::app | std::ios::out);
	}
	m_file << content << "\n";
	m_file.flush();
#ifdef WIN32
#if ((defined(WIN32) || defined(WIN64)) && defined(_DEBUG))
	//m_winOutLog.OutToConsole(const_cast<std::string&>(content));
#endif
#endif
	return;
}
void LLog::Stop()
{
	m_stop.store(true);
	m_cond.notify_one();//唤醒等待
}
void LLog::Join()
{
	if (m_thread && m_thread->joinable())
		m_thread->join();
}
uint64_t inline now()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch()
		).count();
}
void LLog::AppendLog(StreamOutLog& log)
{
	std::unique_lock<std::mutex> l(m_mutex);
	std::string content(log.m_stream.str());
#ifdef _LINUX
	if (content.size() > 1024)
	{
		content = content.substr(0, 1024) + " --OutOfSize--- " + std::to_string(content.size());
	}
#endif
	m_list.emplace_back(std::move(content));
	m_cond.notify_one();
}
void LLog::Run()
{
	//SignNameForThread("LogThread");
	std::string content;
	while (!m_stop)
	{
		{
			std::unique_lock<std::mutex> l(m_mutex);
			m_cond.wait(l, [this]() ->bool { return m_stop || !m_list.empty(); });
			if (!m_list.empty())
			{
				content.swap(m_list.front());
				m_list.pop_front();
			}
		}
		if (!content.empty())
			WriteFile(content);
	}
	std::unique_lock<std::mutex> l(m_mutex);
	std::for_each(m_list.begin(), m_list.end(), [this](const std::string& content) {WriteFile(content); });
}

StreamOutLog& LogPrint::Stream()
{
	return g_logstreamPerThread;
}
#ifdef _LINUX
#include<sys/time.h>
#include<sys/syscall.h>
#include<unistd.h>
#elif defined(WIN32)
#include<windows.h>
#endif
size_t LogPrint::ThreadId()
{
#ifdef _LINUX
	static thread_local long threadId = static_cast<size_t>(syscall(SYS_gettid));
#elif _WIN32
	static thread_local long threadId = static_cast<size_t>(::GetCurrentThreadId());
#endif
	return threadId;
}
LogPrint::LogPrint(const char*filename, const char* function, size_t line, LLOG_LEVEL level)noexcept
{
	g_logstreamPerThread.m_haveWrite = true;
	do 
	{
		if (g_loglevel < level)
		{
			g_logstreamPerThread.m_haveWrite = false;
			break;
		}
		g_logstreamPerThread.m_stream.str("");
		static std::map<int, std::string> levelname = {
			{ LLOG_LEVEL_NULL,"[NULL]" },
			{ LLOG_LEVEL_FATAL,"[FATAL]" },
			{ LLOG_LEVEL_INFO,"[INFO]" },
			{ LLOG_LEVEL_ERROR,"[ERROR]" },
			{ LLOG_LEVEL_DEBUG,"[DEBUG]" },
		};
		g_logstreamPerThread << "Tid:[" << ThreadId() << "] "<< \
			g_logstreamPerThread.GetCurrentTimeStr()<<"\t" << levelname[level] << "\t";
#ifdef _LINUX
		static const char split = '/';
#else
		static const char split = '\\';
#endif
		const char*data = strrchr(filename, split);
		if (data)
			filename = data + 1;
		//function name 里面有模板参数的时候不太好看
		std::string funtionName = function;
		auto pos = funtionName.find_first_of('<');
		if (pos != std::string::npos)
		{
			auto pos_end = funtionName.find_last_of('>');
			if (pos_end != std::string::npos && pos_end > pos)
			{
				funtionName.erase(pos, pos_end - pos + 1);
			}
		}
		g_logstreamPerThread << "[" << filename << "-" << funtionName << "-" << line << "]  ";
	} while (false);

}
LogPrint::~LogPrint()noexcept
{ 
	if(g_logstreamPerThread.m_haveWrite)
		g_log.AppendLog(g_logstreamPerThread);
}
//全局的日志级别 日志对象 流输出对象
LLOG_LEVEL g_loglevel = LLOG_LEVEL_DEBUG;
LLog g_log;
thread_local StreamOutLog g_logstreamPerThread;
StreamOutLog::StreamOutLog()
{
	m_currentTime = 0;
	memset(m_timeStr, 0x00, sizeof(m_timeStr));
}
const char* StreamOutLog::GetCurrentTimeStr()
{
	auto currentTime = now_ms();
	if (currentTime / 1000 != m_currentTime)
	{
		m_currentTime = currentTime / 1000;
		struct tm now_tm;
#ifdef _LINUX
		localtime_r(&m_currentTime, &now_tm);
#else
		localtime_s(&now_tm, &m_currentTime);
#endif
		snprintf(m_timeStr, sizeof(m_timeStr), "[%04d-%02d-%02d %02d:%02d:%02d %03d]", \
			now_tm.tm_year + 1900, \
			now_tm.tm_mon + 1, now_tm.tm_mday, now_tm.tm_hour, \
			now_tm.tm_min, now_tm.tm_sec, static_cast<int>(currentTime % 1000));
	}
	else
	{
		int millSec = static_cast<int>(currentTime % 1000);
		m_timeStr[21] = '0', m_timeStr[22] = '0';
		if (millSec < 10)
			m_timeStr[23] = '0' + millSec;
		else if (millSec < 100)
			m_timeStr[22] = '0' + millSec / 10, m_timeStr[23] = '0' + millSec % 10;
		else
			m_timeStr[21] = '0' + millSec / 100, m_timeStr[22] = '0' + (millSec % 100) / 10, m_timeStr[23] = '0' + millSec % 10;
	}
	return m_timeStr;
}
#if ((defined(WIN32) || defined(WIN64)) && defined(_DEBUG))
WinColorOutLog::WinColorOutLog() {
	out_handle_ = ::GetStdHandle(STD_OUTPUT_HANDLE);

	//设置各种级别日志的颜色
	const WORD BOLD = FOREGROUND_INTENSITY;
	const WORD RED = FOREGROUND_RED;
	const WORD GREEN = FOREGROUND_GREEN;
	const WORD CYAN = FOREGROUND_GREEN | FOREGROUND_BLUE;
	const WORD WHITE = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
	const WORD YELLOW = FOREGROUND_RED | FOREGROUND_GREEN;

	log_level_color_["DEBUG"] = CYAN;
	log_level_color_["INFO"] = GREEN;
	log_level_color_["ERROR"] = RED | BOLD;
	log_level_color_["FATAL"] = BACKGROUND_RED | WHITE | BOLD;
	log_level_color_["USER"] = YELLOW | BOLD;
}
void WinColorOutLog::OutToConsole(std::string& fmtlog)
{
	//仅仅染色 DEBUG这几个字母
	//这个是根据 日志的fmt格式而定 我们的日志fmt的时候是 [时间戳]\t[DEBUG]这样的
	do
	{
		fmtlog += "\r\n";
		//调试模式下在vs里面打印日志
		if (is_str_utf8(fmtlog.c_str()))
		{
			fmtlog = UTF8_To_string(fmtlog);
		}
		OutputDebugStringA(fmtlog.c_str());
		auto pos0 = fmtlog.find("]");
		if (std::string::npos == pos0)
			break;
		auto pos1 = fmtlog.find("]", pos0+1);
		if (std::string::npos == pos1)
			break;
		auto pos2 = fmtlog.find("[", pos1);
		if (std::string::npos == pos2)
			break;
		auto pos3 = fmtlog.find("]", pos2);
		if (std::string::npos == pos3)
			break;
		std::string logLevelstr = fmtlog.substr(pos2 + 1, pos3 - pos2 - 1);
		if (!log_level_color_.count(logLevelstr))
			break;
		printLog(fmtlog, 0, pos2 + 1);
		auto orig_attribs = set_color(log_level_color_[logLevelstr]);
		printLog(fmtlog, pos2 + 1, pos3);
		// reset to orig colors
		::SetConsoleTextAttribute(out_handle_, orig_attribs);
		printLog(fmtlog, pos3, fmtlog.size());
		return;
	} while (false);
	std::cout << fmtlog << "\r\n";
	return;
}
void WinColorOutLog::printLog(const std::string& fmtLog, size_t start, size_t end)
{
	auto size = static_cast<DWORD>(end - start);
	::WriteConsoleA(out_handle_, fmtLog.data() + start, size, nullptr, nullptr);
}
WORD  WinColorOutLog::set_color(WORD attribs)
{
	CONSOLE_SCREEN_BUFFER_INFO orig_buffer_info;
	::GetConsoleScreenBufferInfo(out_handle_, &orig_buffer_info);
	WORD back_color = orig_buffer_info.wAttributes;
	// retrieve the current background color
	back_color &= static_cast<WORD>(~(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY));
	// keep the background color unchanged
	::SetConsoleTextAttribute(out_handle_, attribs | back_color);
	return orig_buffer_info.wAttributes; // return orig attribs
}
#endif