#pragma warning (disable:4800)
<<<<<<< HEAD
#pragma warning (disable:4305)
=======
#pragma warning (disable:4566)
>>>>>>> 2eb550816d6203079d57724aa8643fd9ecae2e11
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "Log.hpp"
#include "LLog.hpp"
#define TestCounts 10000
//∑«spdlogtest
struct TimeCost {
	TimeCost(const std::string& content) :m_TestContent(content) {
		m_time = now_ms();
	}
	~TimeCost()
	{
		fprintf(stderr, "%s Cost %lld ms\n", m_TestContent.c_str(), now_ms() - m_time);
	}
	std::string m_TestContent;
	uint64_t  m_time;
};
void LogTest()
{
	std::string str("testString0000000");
	int number = 190;
	double f = 20.98;
	float c = 123.99;
	TimeCost  timeCost("OrdinaryLogTest");
	std::vector<std::thread> ths;
	for (int i = 0; i < 10; i++)
	{
		int beg = i*TestCounts;
		ths.emplace_back([str, number, f, c,beg]() {
			for (int j = beg,end = beg+ TestCounts; j <end; j++)
			{
				LOG_INFO << "\"msg\":\"TestMsg\",\"int\":" << 100\
					<< ",\"string\", \"stringtest\"" << ",\"float\":" << c\
					<< ",\"double\":" << f\
					<< ",\"int2\":" << number\
					<< ",\"std::string\":" << str\
					<< ",\"IDX\":" << j;
				LOG_DEBUG << "\"msg\":\"TestMsg\",\"int\":" << 100\
					<< ",\"string\", \"stringtest\"" << ",\"float\":" << c\
					<< ",\"double\":" << f\
					<< ",\"int2\":" << number\
					<< ",\"std::string\":" << str\
					<< ",\"IDX\":" << j;
			}
		});
	}
	for (auto& t : ths)
		t.join();


}
void SpdLogTest()
{
	std::string str("testString0000000");
	int number = 190;
	double f = 20.98;
	float c = 123.99;
	TimeCost  timeCost("SpdLogTest");
	std::vector<std::thread> ths;
	for (int i = 0; i < 10; i++)
	{
		int beg = i*TestCounts;
		ths.emplace_back([str, number, f, c, beg]() {
			for (int j = beg, end = beg + TestCounts; j < end; j++)
			{
				DEBUG_LOG.Msg("TestMsg")
					("int", 100)("string", "stringtest")
					("float", c)("double", f)("int2", number)
					("std::string", str)
					("IDX", j);
				INFO_LOG.Msg("TestMsg")
					("int", 100)("string", "stringtest")
					("float", c)("double", f)("int2", number)
					("std::string", str)
					("IDX", j);
			}
		});
	}
	for (auto& t : ths)
		t.join();
}

int main()
{
	//auto console = spdlog::stdout_color_mt("console");
	//console->info("a{}   bbbb->{}", 100, "fffff");
	//spdlog
	JsonSpdlog::Instance().InitAndStart("TestSpd.log");
	JsonSpdlog::Instance().SetLogLevel(spd_loglevel::info);
	//∆’Õ®log
	Log_SetFileNameAndStart("TestOrdinary");
	Log_SetLogLevel(LLOG_LEVEL_INFO);


	SpdLogTest();
	LogTest();

	system("pause");
}