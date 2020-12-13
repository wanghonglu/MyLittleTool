#include "Log.hpp"
#include "spdlog/async.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/async_logger.h"
thread_local LogFormat g_logformat;
Llog::Llog()
{
}
//struct caldaily
void Llog::InitAndStart(const std::string&logfilename, spd_loglevel level,\
	size_t asyncThreadCounts, size_t asyncItem)
{
	
	spdlog::init_thread_pool(asyncItem, asyncThreadCounts, []() {});
	//日切滚动sink
	auto dailyfilesink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(logfilename, 0, 0, 0);
#ifdef WIN32
	//标准数据sink
	auto out2consolesink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	log_.reset(new spdlog::async_logger("spdlog", { out2consolesink ,dailyfilesink }, \
		spdlog::thread_pool()));
	log_->set_pattern(R"({"datetime":"%Y-%m-%d %T %e","level":"%^%l%$","thread":%t,"file":"%s-%#-%!"%v })");
#else
	log_.reset(new spdlog::async_logger("spdlog", {dailyfilesink }, \
		spdlog::thread_pool()));
	log_->set_pattern(R"({"datetime":"%Y-%m-%d %T %e","level":"%l","thread":%t,"file":"%s-%#-%!"%v })");
#endif
	log_->flush_on(spd_loglevel::info);
	spdlog::register_logger(log_);
	spdlog::set_error_handler([](const std::string& errMsg) {
		fprintf(stderr, "spdlog someting is error %s\n", errMsg.c_str());
	});
	SetLogLevel(level);
}
void Llog::SetLogLevel(int level)
{
	if(level>=spd_loglevel::trace && level<=spd_loglevel::critical )
		spdlog::set_level(static_cast<spd_loglevel>(level));
}
LogFormat& LogFormat::Msg(spd_string_view msg)
{
	fmt::format_to(bufer_, ",\"msg\":{}", msg);
	return *this;
}