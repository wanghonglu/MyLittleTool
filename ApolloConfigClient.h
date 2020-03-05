#ifndef __APOLLOCONFIGCLIENT__H
#define __APOLLOCONFIGCLIENT__H
/*	阿波罗github地址 https://github.com/ctripcorp/apollo
	octans下面有一个Apollo客户端的实现，但实现过于复杂，而且有点问题
	1: 阿波罗的namespace 创建的时候可以根据配置文件的类型选择不同的类型 json xml等等
	好处是在修改的时候会做相应的校验，保证配置的正确，如果是默认的properties 即使配置文件写错了，也不会有报错，挺严重
	而且设置了类型后 不用再额外设置key namespace就是key  当namespace的key是非properties get到的结果在content里面
	2: 阿波罗的接口如果配置没变化 1分钟后会返回304 有配置200 这个需要判断下，解析失败和http报错目前都会不停的调用
	不太好
	3: 用起来麻烦，原则上，只需要暴露出一个接口，接收string参数，然后解析出需要的内容就可以了
	4: 不支持多个appid cluster的情况
	5: 如果配置太多，异步获取、解析耗时，可能进程起来了 但是配置还没有加载到内存里，容易出现bug,在程序启动第一次加载配置的时候
	适当等待一下比较好，配置如果配错了，最好启动的时候不要起来，中途修改配置如果配置配错解析失败，则不会交换内存中的数据
	6: 写的太复杂
*/
#include "LSingleton.h"
#include<string>
#include<memory>
#include<set>
#include<boost/asio.hpp>
#include<boost/asio/steady_timer.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>
#include<sstream>
#include<unordered_map>
#include<chrono>
#include<thread>
#include "simplewebserver/client_http.hpp"
#include "json.hpp"
#include "LLog.h"
#include<atomic>
/* 阿波罗的监控的最小单位是namespace 也就是 即使同一个namespace 的某一个key变化，也会把整个namespace下的内容推过来 */
/* 所以建议一个key一个namespace */
/* 阿波罗页面端口是8070 客户端api端口是8080  */
namespace apolloconfig {
	class ApolloConfigClient;
	class ApolloConfigBase;//配置的基类  具体的实现继承这个
	//用智能指针做std::unordered_map的key的时候需要实现一个hash函数
	//用来计算出具体的hash值，如果不实现会导致两个即使内容一样的值算出来的hash值也不一样
	//必须还得实现相等的比较，不然默认是按指针地址比较的不对的
	struct shared_pair_hash {
		template<typename T1, typename T2>
		std::size_t operator()(const std::shared_ptr<std::pair<T1, T2>>& p)const
		{
			return std::hash<T1>{}(p->first) ^ std::hash<T2>{}(p->second);
		}
	};
	struct shared_pair_equal {
		template<typename T1, typename T2>
		bool operator () (const std::shared_ptr<std::pair<T1, T2>>& p1, const std::shared_ptr<std::pair<T1, T2>>& p2)const
		{
			return p1->first == p2->first && p1->second == p2->second;
		}
	};
	enum HttpCode {
		HTTP_OK = 200, //watch接口如果有变化会返回这个
		HTTP_NO_CHANGE = 304,//阿波罗watch接口如果配置没变化会返回这个
	};
	using HttpClientPtr = std::shared_ptr<SimpleWeb::Client<SimpleWeb::HTTP>>;
	using Response = std::shared_ptr<SimpleWeb::Client<SimpleWeb::HTTP>::Response>;
	using ParseConfigHandler = std::function<bool(std::shared_ptr<std::string>)>;
	using AppIdClusterPairPtr = std::shared_ptr<std::pair<std::string, std::string>>;
	class ApolloConfigClient :public LSingleton<ApolloConfigClient>{
	public:
	    ApolloConfigClient();
		bool Init() { return true; }
		void Init(const std::string& _ip, uint32_t delaytime=30);
		void Join();
		void Stop();
		void RegistConfig(const std::string& appId, const std::string& cluster, const std::string& ns, ApolloConfigBase& );
		static std::string httpurlencode(const std::string& val);
		//获取阿波罗的配置是异步的时候，有些配置是比较重要的，希望能同步获取，或者同步等待异步全部获取完
		// 比如比赛场的配置 是xml的 文件大概有600多K，我看了下解析需要3s http请求需要大概1s 
		// 这才是一个配置，以后再加，这个请求、解析时间可能会很长 有必要加一个同步措施 等待配置加载到内存
		// 所以提供两个版本 一个是直接watch，加载在异步回调里，第二个同步阻塞等待异步加载完成，保证后续操作正常
		void BeginWatching();
		void WaitGetAllConfigAndBeginWatching();
		//
	private:

		void WatchConfig(AppIdClusterPairPtr appIdCluster, HttpClientPtr http = nullptr );

		void WatchCallBack(Response resp, const boost::system::error_code&err, HttpClientPtr httpclt, AppIdClusterPairPtr appIdCluster);

		void GetOneConfig(AppIdClusterPairPtr appIdCluster, const  std::string& ns);

		void GetConfigCallBack(Response resp, const boost::system::error_code&err, HttpClientPtr httpclt  );
		template<typename Function>
		void DelayExcute(unsigned int seconds, Function func);

		void DelTimer(uint32_t);

		std::string GetLocalIp()noexcept;


		HttpClientPtr MakeHttpClient();
		std::string								       m_address;
		std::shared_ptr<boost::asio::io_service>       m_ioservice;
		std::shared_ptr<boost::asio::io_service::work> m_work;
		std::shared_ptr<std::thread>				   m_thread;	     
		std::unordered_map<uint32_t ,std::shared_ptr<boost::asio::steady_timer>> m_timer;
		//每个key对应的解析handler key 是 appId cluster namespace 
		std::unordered_map<std::string, ParseConfigHandler > m_configParseHandler;
		//watch 的时候是以appID和cluster两个维度为准
		std::unordered_map<AppIdClusterPairPtr , std::vector<std::string>, shared_pair_hash, shared_pair_equal> m_appIdAndCluster;
		//notifications 是以appid cluster namespace 三个维度确定的 这个值在配置发生变化会变化
		std::unordered_map<std::string, int64_t>  m_notificationId;//appid.cluster.namespace 2 notification
		uint32_t     m_delaytime = 30;//网络出现异常的时候  延时调用的时间
		std::string  m_localIP;
		std::atomic<uint32_t>  m_totalConfigs;
	};
	/* namespace最好在创建的时候，指定具体的类型，比如json xml 这样这里传入的namespace要带上.json这样 
	   比如 commonconfig.json  commonconfig就是namespace 类型为json 
	*/
	class ApolloConfigBase {
	public:
		virtual bool ParseConfig(std::shared_ptr<std::string>) = 0;//具体的配置只需要实现这一个接口就行，解析具体配置
	};

	template<typename FunctionType>
	void ApolloConfigClient::DelayExcute(unsigned int seconds, FunctionType func)
	{
		static uint32_t ID = 0;
		auto timer = std::make_shared<boost::asio::steady_timer>(*m_ioservice);
		if (timer == nullptr)
			return;
		if (seconds < 0)
			seconds = m_delaytime;
		++ID;
		m_timer[ID] = timer;
		timer->expires_from_now(std::chrono::seconds(seconds));
		timer->async_wait([func,Id=ID ,this](const boost::system::error_code& err) {
			if (err != boost::asio::error::operation_aborted)//ioservice.stop()
			{
				func();
			}
			DelTimer(Id);
		});
	}
}
#define gApolloConfig apolloconfig::ApolloConfigClient::Instance()
/*
实际用的时候
gApolloConfig.Init("127.0.0.1");
gApolloConfig.RegistConfig("first_project", "default", "other_test.json", gapollotest);
gApolloConfig.BeginWatching();//如果启动的时候需要加载完成则需要WaitGetAllConfigAndBeginWatching
然后对应的配置类实现解析的函数 ParseConfig
解析成功后交换内存中的数据，注意加锁

*/
#endif