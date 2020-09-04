#include "process_scheduler.h"
//线程局部变量 每个线程1个
__thread ProcessScheduler* gPerThreadProcessScheduler = NULL;
 #define Co_StackSize 1024*1024
 #define Co_SharedStack 1024*1024*8
 struct ProcessScheduler* ProcessSchedulerInit()
 {
     ProcessScheduler* scheduler=(ProcessScheduler*)malloc(sizeof(ProcessScheduler));
     memset(scheduler,0,sizeof(ProcessScheduler));
     scheduler->co_index_=0;
     scheduler->co_stack_size = Co_StackSize;
     //创建一个0号协程,0号协程 即主协程,这个协程跟别的不一样 这个协程在系统的栈上
     //其他的在堆上,这个协程不开辟空间
     context_t* main_co = (context_t*)malloc(sizeof(context_t));
     memset(main_co, 0x00, sizeof(context_t));
 
     scheduler->allcos_[0]=main_co;
 
     return scheduler;
 }
 ProcessScheduler* GetCurrentProcessScheduler()
 {
     if(!gPerThreadProcessScheduler)
         gPerThreadProcessScheduler = ProcessSchedulerInit();
 
     return gPerThreadProcessScheduler;
 }

