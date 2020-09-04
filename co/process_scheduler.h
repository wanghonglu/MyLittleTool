#ifndef __PROCESS_SCHEDULER__H
#define __PROCESS_SCHEDULER__H
/*
	协程调度器  线程局部变量 即 每个线程一个
*/
#include "coroutine_ctx.h"
typedef struct context context_t;
/*
  * 协程A ->协程B 
  *     那么第一次运行的时候 A->B  B协程执行前 要把共享栈上的内存先拷贝给A
  *                                然后运行B  B运行完了 要从B->A
  *                                同样的 要把栈上的东西拷贝给B
  *                                然后执行A A开始执行的地方是 第一次A->B下面的内容
  *                                这时候要把开始保存的内容 再次拷贝进栈里
  * */
//通用共享栈
 typedef struct common_shared_stack{
     context_t*   cur_co;//当前使用共享栈的协程
     char*        stack_buf; 
     size_t       stack_size;
     char*        co_shared_rbp;//共享栈的rbp指针 栈基指针 stack_buf+stack_size
 }common_shared_stack_t;
 typedef struct ProcessScheduler{
     context_t*    allcos_[128];//协程内部启动协程,应该不会超过128 
     size_t        co_index_;//co_index_ 号协程 当前运行协程
     size_t        co_stack_size;
 
 }ProcessScheduler;


 ProcessScheduler* ProcessSchedulerInit();
 ProcessScheduler* GetCurrentProcessScheduler();

#endif 
