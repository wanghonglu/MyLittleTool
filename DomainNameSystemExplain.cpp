//linux 下域名解析，拿到一个ip 端口 或者直接一个域名，怎么解析成 addrinfo的形式
//而不是主动去构造这样的结构，用于connect 或者socket之类的
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h> //getaddrinfo
#include<string.h>
#include<map>
#include<stdio.h>
#include<arpa/inet.h> //inet_ntop
#include<string>
/*  
 int getaddrinfo(const char *node, const char *service,
                 const struct addrinfo *hints,
                 struct addrinfo **res);

   struct addrinfo {
    int              ai_flags;
    int              ai_family;
    int              ai_socktype;
    int              ai_protocol;
    socklen_t        ai_addrlen;
    struct sockaddr *ai_addr;
    char            *ai_canonname;
    struct addrinfo *ai_next;
    };
该接口中 跟socket( domain, type, protocol )类似
 family    就是上面的domian  就是域 就是指定通信特征 包括地址格式 常用的比如 AF_INET (ipv4网络 ) AF_INET6 (ipv6 网络 ） AF_UNIX( unix域 ） AF_UPSPEC(未指定)
 socktype  就是上面的type 套接字类型，比如常用的 SOCK_STREAM( 在域AF_INET下就是 TCP )  SOCK_DGRAM(AF_INET下就是UDP )
 protocol  socket这个函数中通常设置为0 传说协议由上面两个参数默认
 也可以手动指定 比如  IPPROTO_TCP (传输控制协议 大名鼎鼎的TCP ) IPPROTO_UDP( 用户数据包协议 UDP ) 
*/
//如果只有域名 则端口port传入NULL
//host:ip  service:端口
int ExplainDomainName( const char* host, const char* service   )
{
    //存放解析出来的 一组地址的链表
    struct addrinfo    *result_list=NULL;
    struct addrinfo    hint;//getaddrinfo 提供的第三个参数 用来选择符合特定条件的地址
   
    struct sockaddr_in *sigle_result;//单个地址结构 

    const char  *addr;//解析出来的地址
    int error;
    char    abuf[INET_ADDRSTRLEN];//inet_ntop 网络字节序的二进制地址转换成文本字符串的形式 与之相反 inet_pton 文本字符串转换成网络字节序

    memset( &hint, 0x00, sizeof( hint ) );//模板结构除了要赋值的内容其他必须都是NULL；
    hint.ai_family = AF_UNSPEC;//这样能取到 IPV4 和IPV6 
    hint.ai_socktype = SOCK_STREAM;// 仅取 TCP
    hint.ai_protocol = IPPROTO_TCP;// TCP

    error = getaddrinfo( host, service,&hint, &result_list );
    if( error != 0 )
    {
        //错误码比较特殊不能通过往常 perror或者strerror来获取 需要通过 gai_strerror
        fprintf( stderr, "%s\n", gai_strerror( error ) );
        return -1;
    }
    
    std::map<int,std::string> family={ {AF_INET,"AF_INET"},//IPV4
                                       {AF_INET6, "AF_INET6"},//IPV6
                                       {AF_UNIX,"AF_UNIX"},
                                       {AF_UNSPEC,"AF_UNSPEC"}};
    std::map<int,std::string> sock_type = { {SOCK_STREAM, "SOCK_STREAM"},//TCP
                                            {SOCK_DGRAM, "SOCK_DGRAM"},//UDP
                                            {SOCK_SEQPACKET, "SOCK_SEQPACKET"},//固定长度 有序可靠面向连接的报文传递
                                            {SOCK_RAW,"SOCK_RAW" }};//IP协议的数据报接口    
    std::map<int,std::string> protocol = { {IPPROTO_TCP, "TCP"},
                                           {IPPROTO_UDP,"UDP"},
                                           {IPPROTO_RAW,"RAW"}};//原始ip数据包协议
    struct addrinfo* temp;
    for( temp = result_list; temp !=NULL; temp = temp->ai_next )
    {
       if( family.count( temp->ai_family ) )
           fprintf( stdout, "family:%s\n", family[temp->ai_family].c_str() );
        if( sock_type.count( temp->ai_socktype ) )
           fprintf( stdout, "socktype:%s\n", sock_type[temp->ai_socktype].c_str() );
        if( protocol.count( temp->ai_protocol ) )
           fprintf( stdout, "protocol:%s\n", protocol[temp->ai_protocol].c_str() );
       if( temp->ai_family == AF_INET )
       {
           sigle_result = ( struct sockaddr_in *)temp->ai_addr;
           addr = inet_ntop( AF_INET, &sigle_result, abuf, INET_ADDRSTRLEN );
           fprintf( stdout, " address:%s port:%d \n ", addr?addr:"unknown", ntohs( sigle_result->sin_port ) );  
       }
    }

    freeaddrinfo( result_list );//这个必须要释放 不然会有内存泄漏
    return 0;
    
}
int main()
{
    ExplainDomainName( "www.cnblogs.com", NULL );
    puts("=============");
    ExplainDomainName( "127.0.0.1", "5672" );
    return 0;
}
