#include "Reactor.h"
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<errno.h>
#include<unistd.h>
#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netdb.h>
#define ErrMsg(fmt,...)do{\
    int errCode=errno;\
    if( errCode!=0 )\
        fprintf( stderr,fmt" %s\n", ##__VA_ARGS__,strerror( errno ) );\
    else\
        fprintf( stderr,fmt"\n", ##__VA_ARGS__);\
}while(0)
epoll_controler*
EpollControlerInit()
{
    epoll_controler* ec = (epoll_controler*)malloc(sizeof(epoll_controler));
    memset(ec,0x00, sizeof(epoll_controler));
    ec->epoll_fd_ = epoll_create(Max_socket_size);
    ec->event_ret_ = (struct epoll_event*)malloc(Max_socket_size*sizeof(struct epoll_event));
    ec->timeWheel_ = InitTimeWheel();
    ec->sockets_ = (socket_info**)malloc(Max_socket_size*sizeof(socket_info*));
    memset(ec->sockets_, 0x00, Max_socket_size*sizeof(socket_info*) );
    return ec;
}
socket_info* GetSocket(epoll_controler* ep, int fd)
{
    if(fd<0 ||fd>Max_socket_size )
        return NULL;
    return ep->sockets_[fd];
}

int SetSocketBlock(epoll_controler* ep, int fd )
{
    if( fd<0 ||fd>Max_socket_size  )
    {
        return -1;
    }
    
    socket_info*  sf=GetSocket(ep,fd);
    if(!sf)
        return -1;
    sf->flag_ = fcntl(fd,F_GETFL);
    if(sf->flag_==-1)
    {
        ErrMsg("fcntl error ");
        return -1;
    }
    sf->flag_|=O_NONBLOCK;
    if( fcntl(fd,F_SETFL, sf->flag_) == 1 )
    {
        ErrMsg("fcntl error ");
        return -1;
    }
    return 0;
}

int
Epoll_ctl( epoll_controler* ep,int fd, int opt, struct epoll_event * et )
{
    if( epoll_ctl(ep->epoll_fd_,opt,fd,et) == -1 )
    {
        ErrMsg("epoll_ctl error %d", fd);
        return -1;
    }
    return 0;
}
int Epoll_loop(epoll_controler*ep,int time_out )
{
    return epoll_wait(ep->epoll_fd_,ep->event_ret_,Max_socket_size,time_out);
}

#ifdef __REACTOR_TEST__
int Connect(const char*ip, unsigned short port )
{
    int s =-1;
    struct addrinfo hints, *server_info,*node;
    char port[6]={0};
    snprintf( port,"%d", port);
    hints.ai_family = AF_UNSPEC;//AF_INET ipv4 AF_INET6 ipv6 这个则是限定返回两者都可
    hints.ai_socktype = SOCK_STREAM;//tcp
    if( getaddrinfo(ip, port, hints, &server_info) !=0  )
    {
        ErrMsg("getaddrinfo error ip:%s prot:%d ", ip, port);
        return -1;
    }
    //返回的是个链表
    for( node=server_info;node!=NULL;node = node->ai_next )
    {
        if((s = socket(node->ai_family,node->ai_socktype, node->ai_protocol)) == -1 )
        {
            ErrMsg("Socket error ");
            return -1;
        }
    }
}
#endif

