#ifndef __CONSULCLT_H__
#define __CONSULCLT_H__
/*
 * 基于simple_web_server的 consul的 类
 *  第一步： kv存储 也就是动态刷新配置
 *  第二不： 服务发现服务注册 todo
 *  simple_web_server 以前的老版本跟boost最新的版本不兼容
 *  会报一个 socket->get_io_service() 不存在
 *  用最新的client_ws.hpp 写法上稍微有点不同
 * */
//#include "include/client_http.hpp"
#include "client_http.hpp"
#include "json.hpp"
#include "def.h"
#include <string>
#include<unordered_map>
#include<memory>
#include<boost/asio.hpp>
#include<boost/asio/steady_timer.hpp>
#include "Singleton.hpp"
#include<boost/asio/basic_io_object.hpp>
/*base64*/
#include <boost/archive/iterators/base64_from_binary.hpp>  
#include <boost/archive/iterators/binary_from_base64.hpp>  
#include <boost/archive/iterators/transform_width.hpp>  
#include "log.h"
class ConfigBase{
    //key 命名 webui上从kv后面到真实的key为止 例如:simpletest/config/someip 
   public:
   explicit ConfigBase( const std::string& key ):m_key(key){}
   virtual void  ParseJsonConfig( const std::string& )=0;
   std::string       m_key;
   mutable WRMUTEX   m_mutex;//读写锁
   unsigned int      m_retryTime=3;//失败重试时间 单位s
   std::string       m_modifyIndex;
};
/* consul 的http连接如果加上index参数，链接会保持5分钟，所以不需要用http连接池，每次请求都重新new出一个client就行 */
// 第一次请求就不需要index参数，就能保持瞬间返回 */
/* 如果请求失败，需要定时重新尝试，不宜频率过高 */
    static const std::string g_consul_version="/v1";
    static const std::string g_consul_kvprefix="/kv/";
    static const std::string g_consul_index="X-Consul-Index";//在http头里会返回 同上次比较不一致就说明改变了
    /* simple_web_server 的老版本跟boost 1.7不兼容 得改下 */
    using HttpClientPtr  =std::shared_ptr<SimpleWeb::Client<SimpleWeb::HTTP> >;
   // using HttpClientPtr  =std::shared_ptr<SimpleWeb::SocketClient<SimpleWeb::WS> >;
class ConsulClient:public Singleton<ConsulClient>{
    public:
    ConsulClient();
    void  Init( const std::string&  _ip, unsigned int port );
    void  Stop();
    void  RegistKeyValue(const std::string& key,  std::shared_ptr<ConfigBase> );
    std::shared_ptr<boost::asio::io_service>&GetIoService();
    std::shared_ptr<ConfigBase> GetConfigByKey( const std::string& key );
    //TODO  服务注册接口 服务发现接口

    private:
    void  DoRealQuestConfig( HttpClientPtr httpclient, const std::string& key, int modify=-1 );
    void  GetKeyValueJson_All();//第一次获取配置
	template<typename Function>
	void  DelayExecut(uint32_t seconds, Function f);//当出现异常后延时执行
    HttpClientPtr NewHttpClient();
	static bool Base64Decode(const std::string&, std::string&);
	static bool Base64Encode(const std::string&, std::string&);
    std::unordered_map< std::string, std::shared_ptr<ConfigBase>> m_configs;
    std::string     m_ip;
    unsigned int    m_port=8500;
    std::string     m_address;
    std::shared_ptr<boost::asio::io_service>  m_ioservice;
    std::shared_ptr<boost::asio::io_service::work> m_work;
    std::shared_ptr<std::thread>              m_thread;
	std::shared_ptr<boost::asio::steady_timer> m_timer;

};
template<typename Function>
void ConsulClient::DelayExecut(uint32_t seconds, Function f)
{
	if( !m_timer)
		m_timer = std::make_shared< boost::asio::steady_timer >(*(GetIoService()));

	m_timer->expires_from_now(
		std::chrono::seconds(seconds)
	);
	//这个地方的f 一定要是值拷贝，不能是引用，因为传进来的lamda表达式
	//的捕获列表中有值拷贝的情况下，这个地方如果是引用
	//在回调执行的时候，那些值拷贝的内容已经失效
	//比如string 这样会把内存写坏 导致比如类内的成员变量失效。。等等一系列bug
	//总之牵扯到异步回调的时候，要么上智能指针，要么上值拷贝，千万慎用引用
	m_timer->async_wait([f](const boost::system::error_code err) {
		if (err != boost::asio::error::operation_aborted)//io_service.stop()
		{
			f();
		}
	});
}
#define gConsul ConsulClient::Instance()
#define RegistConsul( key, Config )  gConsul.RegistKeyValue(key, std::make_shared<Config>(key))
#include<vector>
class ConfigTest:public ConfigBase{
public:
    ConfigTest( const std::string& key  ):ConfigBase(key){}
    struct data_{
        std::string  ip_;
        unsigned int port_;
        std::string  addr_;
        unsigned int count_;
        std::vector<int> flags_;
        void Print()
        {
            SLOG_ERROR("config "<<LVAL(ip_)<<LVAL(port_)<<LVAL(addr_)<<LVAL(count_)<<LVAL(flags_));
        }
    };
    data_  m_data;
    void ParseJsonConfig(  const std::string& result  )override{
        try{
			nlohmann::json content = nlohmann::json::parse(result);
            if( content["address"].is_string() &&
                content["retry_counts"].is_number() && 
                content["flags"].is_array() &&
                content["ipconfig"].is_object() )
            {
                data_ temp;
                temp.addr_ = content["address"].get<std::string>();
                temp.count_ = content["retry_counts"].get<unsigned int>();
                temp.flags_ = content["flags"].get<std::vector<int> >();
                temp.ip_ = content["ipconfig"]["someIp"].get<std::string>();
                temp.port_ = content["ipconfig"]["someport"].get<unsigned int>();

                {
                    W_LOCK l(m_mutex);
                    m_data = temp;
                }
                m_data.Print();
            }

        }
        catch(...)
        {
            SLOG_ERROR( "parse error " );
        }
    }
};

#endif
