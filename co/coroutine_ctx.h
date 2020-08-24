#ifndef __CO_CTX__H
#define __CO_CTX__H
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<stdint.h>
typedef struct common_shared_stack common_shared_stack_t;
#include "process_scheduler.h"
#define log_print(fmt,...) fprintf( stderr, fmt"\n", ##__VA_ARGS__ );
 typedef void(*fn_type)(void*);
 enum RegistIdx{
     RSP,
     RBP,
     RIP,//指令寄存器
     RDI,//第一个参数
 };
 //还有R12-R15 rbx 这五个也是被调用放保存
 //还有参数寄存器 如果调用jump的时候上一个函数有参数,二这个jump就俩
 //那么剩下的4个寄存器是都有值的
 //即 rsi rdx rcx r8 r9
 // 总共需要保存的 
 // rbp(栈基指针)
 // rsp(栈顶指针)
 // rip(指令寄存器) 也就是函数开始的指针
 // rdi  第一个参数
 // rsi rdx rcx r8 r9 总共6个参数
 // rbx r12-r15 被调用放保存的寄存器
 //
 //栈帧格式定义为
 //|RSP|RBP|RIP|RDI|RBX|R12|R13|R14|R15|RDX|RCX|R8|R9|RSI
 //协程栈定义
typedef struct corotine_stack{
    void    *reg[14];
    char*   buf;//真正的栈空间
    char*   rbp;//栈基指针
}corotine_stack;

//共享站定义  线程内部的共享站
typedef struct shared_stack{
    char*    rsp;//栈顶 栈基可以算作原始栈的buf  
    size_t   len;
    char*    buf;//存储协程实际使用的栈
    common_shared_stack_t* common_stack;//通用共享栈
}shared_stack;
typedef struct context{
     fn_type fun;
     void* args;

     uint8_t is_shared_stack:1;
     uint8_t is_beg:1;
     uint8_t is_end:1;

     corotine_stack  co_stack;//协程上下文
     //共享栈的情况下 co_stack的buf不自己申请  非共享栈 buf自己申请
     shared_stack*  co_shared_stack;//共享栈

}context_t;
//汇编切换协程
extern void* jump_2_context(void * cur,void* pending );
 //函数栈帧
 void clear_stack(context_t*);
//协程切换
void coroutine_swap( context_t*cur,context_t* next );
//协程切除
void co_yield();
 /*
  * =================================================
  * */
 //非共享栈有点坑 一个协程一个栈 内存不够用 并发上不去
 //共享站也有缺点 每次切换拷贝耗时
 //统一的协程调度器  一个线程1个
 //
 //协程启动调用函数
void RoutineStartFun(void*);

//非共享栈
context_t* co_create(fn_type fun, void*args);
//共享栈
context_t* co_create_shared_stack(fn_type fun, void*args);
//协程启动
void co_start(context_t*);
//保存共享栈上内容
void save_shared_stack(context_t*);
#endif

