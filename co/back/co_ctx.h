#ifndef __CO_CTX__H
#define __CO_CTX__H
#include<stdlib.h>
#include<string.h>

 typedef void(*fn_type)(void*,void*,void*);
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
 typedef struct context_t{
     void *reg[14];
     fn_type fun;
     void* args1;
     void* args2;
     void* args3;
     void*  st;
 }context_t;
 extern void* jump_2_context(void * cur,void* pending );
 void RoutineFun( context_t *ctx  );
 //函数栈帧
 //|----函数地址---|-----参数------|
 void make_stack(context_t* ctx, fn_type fun,void* args1, void*args2,void*args3 );
 void clear_stack(context_t*);
 //统一的协程调度器  一个线程1个
typedef struct ProcessScheduler{
    context_t*    allcos_[128];//协程内部启动协程,应该不会超过128 
    size_t        co_index_;
}ProcessScheduler;
#endif

