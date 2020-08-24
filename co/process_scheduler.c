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
 //线程局部变量 线程安全
 common_shared_stack_t* GetGlobSharedStackPerThread(ProcessScheduler* proc)
 {
     if( proc->co_shared_stack == NULL )
     {
         //默认用一个共享栈 栈空间8M
         SetGlobleSharedStack(1,8*1024*1024);
     }
     size_t idx = (proc->co_shared_stack_idx++)%proc->co_total_shared_stack;
     return proc->co_shared_stack[idx];
 }
 ProcessScheduler* GetCurrentProcessScheduler()
 {
     if(!gPerThreadProcessScheduler)
         gPerThreadProcessScheduler = ProcessSchedulerInit();
 
     return gPerThreadProcessScheduler;
 }
/*
 * count:创建几个共享栈
 * stacksize:每个栈多大
 * */
void SetGlobleSharedStack(size_t count,size_t stacksize )
{
    ProcessScheduler* proc = GetCurrentProcessScheduler();
    proc->co_total_shared_stack = count;
    proc->co_shared_stack_idx=0;
    proc->co_shared_stack = (common_shared_stack_t**)malloc(count*sizeof(common_shared_stack_t*));

    for( int i=0;i<count;i++ )
    {
        proc->co_shared_stack[i]=(common_shared_stack_t*)malloc(sizeof(common_shared_stack_t));
        proc->co_shared_stack[i]->cur_co = NULL;
        proc->co_shared_stack[i]->stack_buf = (char*)malloc(stacksize); 
        proc->co_shared_stack[i]->stack_size = stacksize;
        proc->co_shared_stack[i]->co_shared_rbp = proc->co_shared_stack[i]->stack_buf+stacksize;
    }
}


