#include "ConsulClient.h"
ConsulClient::ConsulClient()
{
    m_ioservice.reset();
    m_work.reset();
    m_thread.reset();
    m_configs.clear();
}
bool ConsulClient::Base64Encode(const std::string& input, std::string& output)
{
	output.clear();
	typedef boost::archive::iterators::base64_from_binary<boost::archive::iterators::transform_width<std::string::const_iterator, 6, 8> > Base64EncodeIterator;
	std::stringstream result;
	copy(Base64EncodeIterator(input.begin()), Base64EncodeIterator(input.end()), std::ostream_iterator<char>(result));
	size_t equal_count = (3 - input.length() % 3) % 3;
	for (size_t i = 0; i < equal_count; i++) {
		result.put('=');
	}
	output = std::move(result.str());
	return !output.empty();
}

bool ConsulClient::Base64Decode(const std::string& input, std::string& output)
{
	typedef boost::archive::iterators::transform_width<boost::archive::iterators::binary_from_base64<std::string::const_iterator>, 8, 6> Base64DecodeIterator;
	std::stringstream result;
	try {
		std::copy(Base64DecodeIterator(input.begin()), Base64DecodeIterator(input.end()), std::ostream_iterator<char>(result));
	}
	catch (...) {
		return false;
	}
	output = std::move(result.str());
	return !output.empty();
}
std::shared_ptr<boost::asio::io_service>& ConsulClient::GetIoService()
{
    return m_ioservice;
}
void ConsulClient::Init( const std::string& _ip, unsigned int port  )
{
    m_ip = _ip;
    m_port = port;
    m_address = _ip+":"+std::to_string(port);
    m_ioservice = std::make_shared<boost::asio::io_service>();
    assert( m_ioservice );
    m_work = std::make_shared<boost::asio::io_service::work>( *m_ioservice );
    assert( m_work );
    m_thread.reset( new std::thread ([this](){ GetIoService()->run(); } ));
    assert( m_thread  );
    m_ioservice->post( [this](){ GetKeyValueJson_All(); } );
}
void ConsulClient::Stop()
{
    m_ioservice->stop();
}
void ConsulClient::RegistKeyValue(const std::string& key,  std::shared_ptr<ConfigBase> config)
{
    m_configs[key]=config;
}
void ConsulClient::GetKeyValueJson_All()
{
    if( m_configs.empty() )
        return ;
    auto client = NewHttpClient();
    for( auto& each_config:m_configs )
    {
        DoRealQuestConfig( client, each_config.first );
    }
}
/* consul的阻塞查询默认是保持5分钟 所以这里的超时就不配置了 */
HttpClientPtr ConsulClient::NewHttpClient()
{
    HttpClientPtr ptr = std::make_shared<SimpleWeb::Client<SimpleWeb::HTTP> >( m_address );
    ptr->io_service = m_ioservice;
    return ptr;
}
std::shared_ptr<ConfigBase> ConsulClient::GetConfigByKey( const std::string& key  )
{
    if( !m_configs.count(key) )
        return nullptr;
    return m_configs[key];
}
//传入-1 就是第一次调用
// 这是旧的simple_web_server
// 新的完全不是这么用的
#if 0
void ConsulClient::DoRealQuestConfig(HttpClientPtr httpclient, const std::string& key , int modify_index )
{
    if( !m_configs.count(key) )
        return ;
    std::stringstream ss;
    ss<<g_consul_version<<g_consul_kvprefix<<key;
    if( modify_index !=-1  )
        ss<<"?index="<<m_configs[key]->m_modifyIndex
    auto callBack = [key ,this]( std::shared_ptr<SimpleWeb::Response> response, const SimpleWeb::error_code err, HttpClientPtr clt ){
       
        if( err || response->Content.string().empty() || !response->header.count(g_consul_index) )
        {
            //如果请求出现异常报错了，不要立即请求，防止出现异常，不停的请求，毕竟只是配置，出现异常延时调用就可以了
            SLOG_ERROR("Consul callback error "<<err.string())
            auto new_http_client = NewHttpClient();
            std::shared_ptr<boost::asio::steady_timer> timer = std::make_shared< boost::asio::steady_timer >( *(GetIoService()) );
            auto config = GetConfigByKey(key);
            if( !config || !timer )
                return ;
            timer->timer.expires_from_now(
                      boost::posix_time::millisec(config->m_retryTime*1000);
                    );
            timer->async_wait([new_http_client, key,this  ](){
                     DoRealQuestConfig( new_http_client, key  );
                    });
            return ;
        }
        else 
        {
            const std::string& content = response->content.string(); 
            std::string  modify_index = response->header[g_consul_index];
            auto config = GetConfigByKey(key); 
            if( !config  )
                return;
            do
            {
                if( config->m_modifyIndex == modify_index  )//配置无变化 进入下一轮watch
                    break;
                //有变化，具体的配置去修改自己内存里的数据
                try{
                    nlohmann::json json_content = nlohmann::json::parse( content );
                    if( json_content.find("Key") != json_content.end() && json_content["Key"].is_string()  )
                    {
                        if( key != json_content["Key"].get<std::string>() )
                            SLOG_ERROR("Consule get key not equal to origine key "<<key<<"  "<<json_content["Key"].get<std::string>() );
                    }
                    if( json_content.find("Value") != json_content.end() && json_content["Value"].is_object() )
                    {
                        config->ParseJsonConfig( json_content["Value"] );//具体的改动具体的配置去处理
                    }        
                }
                catch( const std::exception& err  )
                {
                    SLOG_ERROR(" parse consul error key "<<key<<" content "<<content<<" err "<<err.what() );
                }
                catch(...)
                {
                    SLOG_ERROR(" parse consul error key "<<key<<" content "<<content );
                }
                
            }while(false)
            DoRealQuestConfig( clt, key, modify_index  );
        }
    }
    auto cb = std::bind( callBack, std::placeholders::_1, std::placeholders::_2,httpclient );
    try
    {
         httpclient->request( "GET", ss.str(), "", CaseInsensitiveMultimap{}, cb ); 

    }
    catch( const std::exception& err  )
    {
        SLOG_ERROR( " http request error "<< m_address<<" content "<<ss.str()<<" error "<< err.what() );
        return;
    }
    catch ( ... )
    {
        SLOG_ERROR( " http request error "<< m_address<<" content "<<ss.str()<<" error " );
        return;
    }
}
#endif
/*
	老版本的boost 和simplewebserver 在win10上运行似乎有点问题
	需要添加这个宏
	#define _WIN32_WINNT=0x0A00
*/
void ConsulClient::DoRealQuestConfig(HttpClientPtr httpclient, const std::string& key , int modify_index )
{
    if( !m_configs.count(key) )
        return ;
    std::stringstream ss;
	ss << g_consul_version << g_consul_kvprefix << key;
	if (modify_index != -1)
		ss << "?index=" << m_configs[key]->m_modifyIndex;
	std::string url = m_address + ss.str();
    auto callBack = [key ,this]( std::shared_ptr<SimpleWeb::Client<SimpleWeb::HTTP>::Response> response, const SimpleWeb::error_code err, HttpClientPtr clt )
	{
		auto err_callback = [key,this ]() {
			auto new_http_client = NewHttpClient();
			auto config = GetConfigByKey(key);
			if (!config)
				return;
			DelayExecut(config->m_retryTime, [this, key]() {
				auto new_http_client = NewHttpClient();
				DoRealQuestConfig(new_http_client, key);
			});
		};
		std::string content = response->content.string();
		/*  content.string() 这个如果第二次调用 就会是空的!!!!
			貌似跟标准库的rdbuf的实现有关
		*/
        if( err || content.empty() || !response->header.count(g_consul_index) )
        {
            //如果请求出现异常报错了，不要立即请求，防止出现异常，不停的请求，毕竟只是配置，出现异常延时调用就可以了
			SLOG_ERROR("Consul callback error " << err.message());
			err_callback();
            return ;
        }
		std::string  modify_index;
		auto it = response->header.find(g_consul_index);
		if (it != response->header.end())
			modify_index = it->second;
		auto config = GetConfigByKey(key);
		if (!config)
			return;
		if (config->m_modifyIndex == modify_index)//配置无变化 进入下一轮watch
		{
			DoRealQuestConfig(clt, key, std::atoi(modify_index.c_str()));
			return;
		}
		//有变化，具体的配置去修改自己内存里的数据
		try
		{
			nlohmann::json json_content_array = nlohmann::json::parse(content);
			//consul返回的是一个json数组
			if (!json_content_array.is_array())
			{
				throw std::runtime_error(" consul result is not a json");
			}
			auto json_content = json_content_array[0];
			if (json_content.find("Key") != json_content.end() && json_content["Key"].is_string())
			{
				if (key != json_content["Key"].get<std::string>())
					throw std::runtime_error("consul key is not equal" + key);
			}
			if (json_content.find("Value") == json_content.end() ||
				!json_content["Value"].is_string())
			{
				throw std::runtime_error(" consul value error ");
			}
			std::string val_result;
			Base64Decode(json_content["Value"], val_result);
			config->ParseJsonConfig(val_result);
			/* 配置正确解析 进入下一次watch */
			/* 保存新的modify_index用于下次调用 */
			config->m_modifyIndex = modify_index;
			DoRealQuestConfig(clt, key, std::atoi(modify_index.c_str()));
			return;
		}
		catch (const std::exception& err)
		{
			SLOG_ERROR(" parse consul error key " << key << " content " << content << " err " << err.what());
			err_callback();
		}
		catch (...)
		{
			SLOG_ERROR(" parse consul error key " << key << " content " << content);
			err_callback();
		}
  
	};
    auto cb = std::bind( callBack, std::placeholders::_1, std::placeholders::_2,httpclient );
    try
    {
         httpclient->request( "GET", ss.str(), "", SimpleWeb::CaseInsensitiveMultimap{}, cb );

    }
    catch( const std::exception& err  )
    {
        SLOG_ERROR( " http request error "<< m_address<<" content "<<ss.str()<<" error "<< err.what() );
        return;
    }
    catch ( ... )
    {
        SLOG_ERROR( " http request error "<< m_address<<" content "<<ss.str()<<" error " );
        return;
    }
}
