#ifndef __CONSULCLT_HPP
#define __CONSULCLT_HPP
/*
 * 基于simple_web_server的 consul的 类
 *  第一步： kv存储 也就是动态刷新配置
 *  第二不： 服务发现服务注册 todo
 * */
#include "client_http.hpp"
#include "json.hpp"
#include "def.h"
#include <string>
#include<unordered_map>
#include<memory>
#include<boost/asio.hpp>
#include<boost/steady_timer.hpp>
class ConfigBase{
    //key 命名 webui上从kv后面到真实的key为止 例如:simpletest/config/someip 
   explicit ConfigBase( const std::string& key ):m_key(key){}
   virtual void  ParseJsonConfig( const nlohmann::json& json )=0;
   std::string       m_key;
   WRMUTEX           m_mutex;//读写锁
};
class ConsuleClient{

    private:
        void LoopRun();//io_sevice->run();
        std::unordered_map< std::string, std::shared_ptr<ConfigBase>> m_configs;
        std::string     m_ip;
        unsigned int    m_port=8500;
        std::shared_ptr<boost::asio::io_service>  m_ioservice;
        std::shared_ptr<boost::asio::io_service::work> m_work;

};
#endif
