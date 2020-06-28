//类似socket的sds 动态字符串 
//模仿redis的sds 但不实现那么多按长度分类的结构，仅实现一种结构即可 柔性数组
#ifndef __SYMPLE_STRING__H
#define __SYMPLE_STRING__H
#ifdef __cplusplus
extern "C"{
#endif
#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdarg.h>
#include<errno.h>
#include<assert.h>
#include<unistd.h>

typedef char* sds;

typedef struct simple_str{
    uint32_t    len_;   //以用;
    uint32_t    alloc_; //总共;
    char        buf_[];
}simple_str;
#define SDS_HEAD_SIZE sizeof(simple_str)
#define SDS_ERROR(msg)  do{\
    int rt = errno;\
    fprintf( stderr, "sds error %s  %s\n",msg, strerror(rt));\
}while(0)

#define SDS_CAST(s) (simple_str*)(s-SDS_HEAD_SIZE);

//1K一下 2倍 扩容 1k以上只单纯的加1k
#define LIMIT_RELLOC 1024
//malloc申请内存其实会有对齐比如申请7个字节 实际给的是8个 给操作系统有关
//对齐是sizeof(long) 32位是4 64位是8
#define RealSize(reqsize, res ) do{\
    res = (reqsize);\
    res += res>LIMIT_RELLOC?LIMIT_RELLOC:res;\
    if(res &(sizeof(long)-1 ))\
        res = (res+sizeof(long))&(~(sizeof(long)-1));\
}while(0); 
//构造
sds sdsnewlen( const void*src, size_t initlen);
sds sdsnewstr( const char*src );
sds sdsnew(size_t size );
sds sdsempty();
//析构
void  sdsfree(sds str);

//追加
sds sdscatlen( sds s, const void*src, size_t len );
sds sdscat( sds, const char* src );
//拷贝
sds sdscopy( sds src );

//清空
void sdsclear(sds s);
//截断
void sdssetlen(sds s, size_t len );
//格式化
// 这个性能比较低 里面在不停的尝试去扩容直到满足要求为止
//  所以慎用
sds sdssprintf( sds s, const char*fmt, ... );

//一下不允许外部调用
//扩容
static sds sdsMakeRoom(sds s, size_t len );
//获取已用size
static size_t sdsgetsize(sds s );
//获取全部大小
static size_t sdsgetalloc(sds s ); 

#ifdef TEST_SDS
//单元测试
void sds_unit_test(int count);
#endif

#ifdef __cplusplus
}
#endif
#endif



