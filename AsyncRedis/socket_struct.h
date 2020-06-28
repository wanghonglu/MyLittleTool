//socket结构
#ifndef __SOCK_STRUCT__H
#define __SOCK_STRUCT__H
#ifdef __cplusplus
extern "C"{
#endif
#include<sys/types.h>
#include<sys/socket.h>
#include "simstr.h"
typedef TcpSocket{
    int    fd_;         //socketfd
    sds    sendBuf_;   //sendbuf;

}



}





#ifdef __cplusplus
extern "C"{
#endif

#endif
