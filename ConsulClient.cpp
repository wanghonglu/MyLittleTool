#include "ConsulClient.h"
ConsulClient::ConsulClient()
{
    m_ioservice.reset();
    m_work.reset();
    m_thread.reset();
    m_configs.clear();
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
void ConsulClient::DoRealQuestConfig(HttpClientPtr httpclient, const std::string& key , int modify_index )
{
    if( !m_configs.count(key) )
        return ;
    std::stringstream ss;
    ss<<g_consul_version<<g_consul_kvprefix<<key
    if( modify_index !=-1  )
        ss<<"?index="<<m_configs[key]->m_modifyIndex
    auto callBack = [key ,this]( std::shared_ptr<SimpleWeb::Response> response, const SimpleWeb::error_code err, HttpClientPtr clt ){
       
        if( err || response->Content.string().empty() || !response->header.count(g_consul_index) )
        {
            //如果请求出现异常报错了，不要立即请求，防止出现异常，不停的请求，毕竟只是配置，出现异常延时调用就可以了
            SLOG_ERROR("Consul callback error "<<err.string())
            auto new_http_client = NewHttpClient();
            std::shared_ptr<boost::asio::stready_timer> timer = std::make_shared< boost::asio::stready_timer >( *(GetIoService()) );
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
            const std::string& content = response->Content; 
            std::string  modify_index = response[g_consul_index]
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
