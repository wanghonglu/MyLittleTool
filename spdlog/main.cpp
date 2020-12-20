#pragma warning (disable:4800)
#pragma warning (disable:4305)
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "Log.hpp"
#include<algorithm>
template<typename T>
void func(T t)
{

}
template<typename T>
struct Test {
	T   a;
	void operator()() {
		INFO_LOG.Msg("TestMsg");
	}
};
int main()
{
	//auto console = spdlog::stdout_color_mt("console");
	//console->info("a{}   bbbb->{}", 100, "fffff");
	Llog::Instance().InitAndStart("Test.log");

	std::string str("testString0000000");
	int number = 190;
	double f = 20.98;
	float c = 123.99;

	INFO_LOG.Msg("TestMsg")\
		("int", 100)("string", "stringtest")\
		("float",c)("double",f)("int2",number)\
		("std::string",str);

	system("pause");
}