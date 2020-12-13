#ifndef __LOG_HPP
#define __LOG_HPP
//����spdlog��json��־
#include "Singleton.hpp"
//#include "log.h"
#include<memory>
#include"spdlog/spdlog.h"
#include "spdlog/fmt/fmt.h"
#include<type_traits>
using spd_source_loc = spdlog::source_loc;
using spd_string_view = spdlog::string_view_t;
using spd_loglevel = spdlog::level::level_enum;

template<spd_loglevel lv>
class LogHelp;

class LogFormat;
#define F_L_F source_loc(__FILE__,__LINE__,__FUNCTION__)
//spdlog ��file�ƺ��ǻ����Ŀ¼̫���ˣ�����������ģ�����Ҳ������һ��
class Llog :public Singleton<Llog> {
	Llog();
	friend Singleton<Llog>;
	template<spd_loglevel lv>
	friend class LogHelp;
public:
	void InitAndStart(const std::string&logfilename, spd_loglevel level = spd_loglevel::debug,size_t asyncThreadCounts=1,size_t asyncItem=8*1024);
	void SetLogLevel(int level);
private:
	std::shared_ptr<spdlog::logger>    log_;
	spd_loglevel					     spd_loglevel_;
};

//�����Ҫ�����ֲ߳̾�����
class LogFormat {
	template<spd_loglevel lv>
	friend class LogHelp;
public:
	LogFormat&Msg(spd_string_view msg);

	//��ֵ����
	template<typename T>
	typename std::enable_if<std::is_arithmetic<typename std::remove_reference<T>::type>::value ||
						   std::is_enum<typename std::remove_reference<T>::type>::value, LogFormat& >::type
	operator()(spd_string_view key, T&& value)
	{
		fmt::format_to(bufer_, ",\"{}\":{}", key,value);
		return *this;
	}
	//�ַ���
	LogFormat& operator()(spd_string_view key, const char* value)
	{
		if(nullptr != value)
			fmt::format_to(bufer_, ",\"{}\":\"{}\"", key,value);
		return *this;
	}
	LogFormat& operator()(spd_string_view key,  char *const value)
	{
		if (nullptr != value)
			fmt::format_to(bufer_, ",\"{}\":\"{}\"", key, value);
		return *this;
	}
	LogFormat& operator()(spd_string_view key, const std::string& value)
	{
		fmt::format_to(bufer_, ",\"{}\":\"{}\"", key, value);
		return *this;
	}
	template<typename T1,typename T2,typename ...Args>
	LogFormat& operator()(T1&&key,T2&&value,Args&&...args)
	{
		//��������������2�ı��� ��KV��ʽ ��Key�������ַ��� const char*
		static_assert(0 == (sizeof...(Args) & 1), "number of args must be mutiple of 2");
		//static_assert(std::is_same<const char*, typename std::remove_reference<T1>::type>::value, \
		//	typeid(key).name());
		this->(std::forward<T1>(key),std::forward<T2>(value),std::forward<Args>(args)...);
		return *this;
	}


private:
	//LogFormat&Msg(const std::string& msg);
	bool                                 shouldlog_;
	fmt::basic_memory_buffer<char, 1024>   bufer_;
};
extern thread_local LogFormat g_logformat;
//trace 0 debug 1 .....crtical 5
// ����๹���ʱ�򴫵� _FILE__ ����Ϣ ������ʱ����� spdlog.log
template<spd_loglevel loglevel>
class LogHelp {
public:
	LogHelp(const char*file, size_t Line, const char*function)\
		: src_loc_(file, Line, function)
	{
		g_logformat.shouldlog_ = loglevel >= Llog::Instance().spd_loglevel_;
		g_logformat.bufer_.clear();
	}
	static LogFormat& LogFarmater()
	{
		return g_logformat;
	}
	~LogHelp()noexcept
	{
		try
		{
			if (g_logformat.shouldlog_)
				Llog::Instance().log_->log(src_loc_, loglevel,\
					spd_string_view(g_logformat.bufer_.data(), g_logformat.bufer_.size()));
		}
		catch (...)//�����쳣 Ҳ������
		{

		}
	}
	spd_source_loc    src_loc_;
};
#define TRACE_LOG     LogHelp<spd_loglevel::trace>(__FILE__,__LINE__,__FUNCTION__).LogFarmater()
#define DEBUG_LOG     LogHelp<spd_loglevel::debug>(__FILE__,__LINE__,__FUNCTION__).LogFarmater()
#define INFO_LOG      LogHelp<spd_loglevel::info>(__FILE__,__LINE__,__FUNCTION__).LogFarmater()
#define ERROR_LOG     LogHelp<spd_loglevel::err>(__FILE__,__LINE__,__FUNCTION__).LogFarmater()
#define CRITICAL_LOG  LogHelp<spd_loglevel::critical>(__FILE__,__LINE__,__FUNCTION__).LogFarmater()
#endif
