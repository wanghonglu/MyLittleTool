#include "ApolloConfigClient.h"
using namespace apolloconfig;
ApolloConfigClient::ApolloConfigClient()
{
	m_ioservice.reset();
	m_work.reset();
	m_timer.clear();
	m_thread.reset();
}
void ApolloConfigClient::Init(const std::string& _ip, uint32_t delaytime)
{
	if (m_ioservice)
		return ;
	m_address = _ip + ":"+std::to_string(8080);
	m_delaytime = delaytime;
	m_ioservice = std::make_shared<boost::asio::io_service>();
	m_work = std::make_shared<boost::asio::io_service::work>(*m_ioservice);
	assert(m_ioservice != nullptr && m_work != nullptr);
	m_localIP = GetLocalIp();
}
void ApolloConfigClient::Join()
{
	if (m_thread && m_thread->joinable())
		m_thread->join();
}
void ApolloConfigClient::Stop()
{
	if (m_ioservice)
		m_ioservice->stop();
}
void ApolloConfigClient::Start()
{
	if (m_thread)
		return;
	if (!m_ioservice)
		return;
	m_thread.reset(new std::thread([this]() {m_ioservice->run(); }));
	assert(m_thread != nullptr);
	m_ioservice->post([this]() {BeginWatching(); });
}
//cluster 貌似我们目前用的都是default
void ApolloConfigClient::RegistConfig(const std::string& appId, const std::string& cluster, const std::string& ns,\
	ApolloConfigBase& conf)
{
	m_configParseHandler[appId+cluster+ns] = std::bind(&ApolloConfigBase::ParseConfig,&conf, std::placeholders::_1);

	AppIdClusterPairPtr  appIdCluster = std::make_shared<std::pair<std::string, std::string>>(std::make_pair(appId, cluster));
	if (!appIdCluster)
		return;
	if (m_appIdAndCluster.count(appIdCluster))
		m_appIdAndCluster[appIdCluster].push_back(ns);
	else
		m_appIdAndCluster[appIdCluster] = std::vector<std::string>{ ns };
	m_notificationId[appId + cluster + ns] =-1;//预先赋值为-1方便后面watch 
}
//apollo 的api github上看有三个 带缓存读取 不带缓存读取 和感知配置更新 三种接口 https://github.com/ctripcorp/apollo/wiki/%E5%85%B6%E5%AE%83%E8%AF%AD%E8%A8%80%E5%AE%A2%E6%88%B7%E7%AB%AF%E6%8E%A5%E5%85%A5%E6%8C%87%E5%8D%97
/*
	带缓存会有一定延迟 不用
	不带缓存的 url URL: {config_server_url}/configs/{appId}/{clusterName}/{namespaceName}?releaseKey={releaseKey}&ip={clientIp}
	后面那个Ip是用来灰度发布的  以后说不定有用 releaseKey是感知接口返回的 
	就是检测到发生变化的namesapce 然后调用不带缓存读取 releaseKey设置为-1 就会返回正常数据
	releasekey如果跟服务器版本一致就
	之所以不直接用不带缓存的接口是因为 感知配置更新接口一分钟如果无返回会返回304 如果发生变化返回200
	内存中会保存一份 notificationId的map, 只有获取的相应的配置之后才会更新否则不更新
*/
void ApolloConfigClient::BeginWatching()
{
	SLOG_ERROR(" Apool Regist Key "<< m_configParseHandler.size() );
	for (const auto& appIdCluset : m_appIdAndCluster)
	{
		if (!appIdCluset.second.empty())
		{
			WatchConfig(appIdCluset.first);
		}
	}
}
//感知配置更新接口 
//url : {config_server_url} / notifications / v2 ? appId = { appId }&cluster = { clusterName }&notifications = { notifications }
//notifications 的格式为 [{"namespaceName":"my_config","notificationId":-1},{"namespaceName":"other_test.json","notificationId":-1}]
//这个接口的每个参数都要URLcode
void ApolloConfigClient::WatchConfig(AppIdClusterPairPtr appIdCluster, HttpClientPtr httpclt )
{
	if (!m_appIdAndCluster.count(appIdCluster))
		return;
	std::vector<std::string>& namespaces = m_appIdAndCluster[appIdCluster];
	if (namespaces.empty())
		return;
	if (httpclt == nullptr)
		httpclt = MakeHttpClient();
	std::string& appID = appIdCluster->first;
	std::string& cluster = appIdCluster->second;
	std::stringstream ss;
	ss << "/notifications/v2?appId=" << httpurlencode(appID) << "&cluster=" << httpurlencode(cluster) << "&notifications=";
	try
	{
		auto json_array = nlohmann::json::array();
		for (const auto& eachname : namespaces)
		{
			nlohmann::json temp;
			temp["namespaceName"] = eachname;
			temp["notificationId"] = m_notificationId[appID + cluster + eachname];
			json_array.emplace_back(temp);
		}
		ss << httpurlencode(json_array.dump());
	}
	catch ( const std::exception& err )
	{
		SLOG_ERROR("JSON ERROR " << err.what());
		return;
	}
	catch (...)
	{
		SLOG_ERROR("JSON ERROR ");
		return;
	}

	try
	{
		httpclt->request("GET", ss.str(), "", SimpleWeb::CaseInsensitiveMultimap{},\
			[httpclt, appIdCluster, this](Response resp, const ErrorCode&err) {
			WatchCallBack(resp, err, httpclt, appIdCluster);
		    });
	}
	catch (...)
	{
		DelayExcute(m_delaytime, [appIdCluster, this]() {
			WatchConfig(appIdCluster);
		});
	}
}

HttpClientPtr ApolloConfigClient::MakeHttpClient()
{
	HttpClientPtr http = std::make_shared<SimpleWeb::Client<SimpleWeb::HTTP>>(m_address);
	http->config.timeout = 70;//如果配置无变化 watch接口1分钟后返回 httpcode 304, 所以这个超时要大于60 也不建议不设，Apollo出异常了
	http->io_service = m_ioservice;
	return http;
}
void ApolloConfigClient::WatchCallBack(Response resp, const ErrorCode& err, HttpClientPtr httpclt,\
	AppIdClusterPairPtr appIdCluster)
{
	try
	{
		if (!resp || err)
		{
			throw std::runtime_error("ApolloWatch Error " + err.message());
		}
		std::string content = resp->content.string();//这个第二次获取会是空的
		//有变化会返回200 无变化会返回304 这个返回的code有个空格，不能直接跟"200"比较，略坑
		int httpCode = std::atoi(resp->status_code.c_str());
		if (httpCode != HTTP_OK && httpCode != HTTP_NO_CHANGE)
			throw std::runtime_error("AppolloWatch Return httpCode Error  "+httpCode );

		if (httpCode == HTTP_NO_CHANGE)
		{
			//配置无变化 进行下一轮watch 
			WatchConfig(appIdCluster, httpclt);//正常情况下这个连接可以复用  没问题
			return;
		}
		//有变化 阿波罗返回的是个json数组 跟consul一样
		nlohmann::json  json_array = nlohmann::json::parse(content);
		if (!json_array.is_array())
			throw std::runtime_error("ApolloWatch return not a array " + content);
		for (const auto& json : json_array)
		{
			std::string ns = json["namespaceName"].get<std::string>();
			int32_t   notificationId = json["notificationId"].get<int>();
			m_notificationId[appIdCluster->first + appIdCluster->second + ns] = notificationId;
			GetOneConfig(appIdCluster,ns);
		}
		//解析成功 进行下一轮watch 
		WatchConfig(appIdCluster, httpclt);
	}
	catch ( const std::exception& err)
	{
		SLOG_ERROR("WatchCallBack error "<<err.what());
		DelayExcute(m_delaytime, [appIdCluster, this]() {
			WatchConfig(appIdCluster);
		});
	}
	catch (...)
	{
		SLOG_ERROR("WatchCallBack error ");
		DelayExcute(m_delaytime, [appIdCluster, this]() {
			WatchConfig(appIdCluster);
		});
	}
}
//不带缓存的http接口
//URL: {config_server_url}/configs/{appId}/{clusterName}/{namespaceName}?releaseKey={releaseKey}&ip={clientIp}
void ApolloConfigClient::GetOneConfig(AppIdClusterPairPtr appIdCluster, const std::string& ns)
{
	std::stringstream ss;
	ss << "/configs/" << appIdCluster->first << "/" << appIdCluster->second << "/" << ns << "?ip=" << m_localIP;
	HttpClientPtr http_client = MakeHttpClient();
	try
	{
		http_client->request("GET", ss.str(), "", SimpleWeb::CaseInsensitiveMultimap{}, \
			[http_client, this](Response resp, const ErrorCode&err) {
			GetConfigCallBack(resp, err, http_client);
		});
	}
	catch (const std::exception& err )
	{
		SLOG_ERROR("GetConfig error "<<err.what());
		return;
	}
	catch (...)
	{
		SLOG_ERROR("GetConfig error ");
		return;
	}
}
void ApolloConfigClient::GetConfigCallBack(Response resp, const ErrorCode&err, HttpClientPtr httpclt)
{
	try
	{
		if (err || !resp)
			throw std::runtime_error(err.message());
		std::string content = resp->content.string();
		nlohmann::json  json_config = nlohmann::json::parse(content);
		std::string appId = json_config["appId"].get<std::string>();
		std::string cluster = json_config["cluster"].get<std::string>();
		std::string ns = json_config["namespaceName"].get<std::string>();
		//这里面的内容是 如果是properties 就是 key-value的形式
		//如果是具体的json 或者xml key就是content
		nlohmann::json&  configurations= json_config["configurations"];
		std::shared_ptr<std::string> values;
		auto it = configurations.find("content");
		if (it != configurations.end())
		{
			values.reset(new std::string(it.value().get<std::string>()));
		}
		else
			values.reset(new std::string(configurations.dump()));
		auto handler = m_configParseHandler[appId + cluster + ns];
		if (handler != nullptr && m_ioservice)
			m_ioservice->post([handler, values,ns]() { 
			try
			{
				handler(values);//调用具体的config实现的解析
			}
			catch (const std::exception& err)
			{
				SLOG_ERROR("ParseConfigError "<<ns<<" "<<err.what() );
			}
			catch (...)
			{
				SLOG_ERROR("ParseConfigError " << ns );
			}	
		});
		else//新加了配置但是没有加入具体的handler 提示下
			throw std::runtime_error("new config added but no handler " + appId + cluster + ns);
	}
	catch ( const std::exception& err )
	{
		SLOG_ERROR("GetConfigCallBack error "<<err.what());
		return;
	}
	catch (...)
	{
		SLOG_ERROR("GetConfigCallBack error ");
		return;
	}
}
void ApolloConfigClient::DelTimer(uint32_t id)
{
	m_timer.erase(id);
}
std::string ApolloConfigClient::GetLocalIp()noexcept
{
	std::string ip;
	boost::asio::ip::tcp::resolver  resolver(*m_ioservice);
	boost::asio::ip::tcp::resolver::query query(boost::asio::ip::host_name(), "");
	auto  iter = resolver.resolve(query);
	decltype(iter) end;
	if (iter != end)
		ip= iter->endpoint().address().to_string();
	return ip;
}
std::string ApolloConfigClient::httpurlencode(const std::string &value)
{
	static auto hex_chars = "0123456789ABCDEF";

	std::string result;
	result.reserve(value.size()); // Minimum size of result

	for (auto &chr : value) {
		if (chr == ' ')
			result += '+';
		else if (chr == '!' || chr == '#' || chr == '$' ||
			(chr >= '&' && chr <= ',') || (chr >= '/' && chr <= ';') ||
			chr == '=' || chr == '?' || chr == '@' || chr == '[' ||
			chr == ']' || chr == '{' || chr == '}' || chr == '"')
			result +=
			std::string("%") + hex_chars[chr >> 4] + hex_chars[chr & 15];
		else
			result += chr;
	}

	return result;
}
