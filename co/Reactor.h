/*
 *  封装epoll 做网络驱动
 * */
#ifndef __REACTOR__H__
#define __REACTOR__H__
#include<sys/epoll.h>
#include"timewheel.h"
typedef struct socket_info{
    int socket_fd_;
    struct timeval read_timeout_;
    struct timeval write_timeout_;
    int flag_;
}socket_info;
#define Max_socket_size 1024*10
typedef struct epoll_controler{
   int                  epoll_fd_;
   struct epoll_event*  event_ret_; 
   TimeWheel*           timeWheel_;//超时管理
   socket_info**        sockets_;//管理的所有socket
}epoll_controler;

epoll_controler* EpollControlerInit();

socket_info* AddSocket(epoll_controler* ep, int fd );
void DeleteSocket(epoll_controler* ep, int fd);

socket_info* GetSocket(epoll_controler* ep, int fd);

int SetSocketBlock( epoll_controler* ep, int fd);

int Epoll_ctl(epoll_controler*,int fd, int opt, struct epoll_event * );

//timeout 单位 毫秒
int Epoll_loop(epoll_controler*,int time_out );





#endif
