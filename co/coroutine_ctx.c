#include<stdio.h>
#include"coroutine_ctx.h"
#include "process_scheduler.h"
void clear_stack(context_t* ctx )
{
   if( ctx->is_shared_stack )
   {
       if( ctx->co_shared_stack && ctx->co_shared_stack->buf )
       {
           free(ctx->co_shared_stack->buf);
           free(ctx->co_shared_stack);
       }
   }
   else
   {
       if( ctx->co_stack.buf )
           free(ctx->co_stack.buf);
   }

   free(ctx);
}
//协程切换
void coroutine_swap(context_t*cur, context_t*next )
{
    if( cur->is_shared_stack )
    {
        char rsp;//这个rsp就是当前栈的栈顶
        cur->co_shared_stack->rsp = &rsp;
    }

    ProcessScheduler* process = GetCurrentProcessScheduler();
    if( next->is_shared_stack )
    {
        //将共享栈上之前使用的协程的
        common_shared_stack_t* shared_st = GetGlobSharedStackPerThread(process);
        if( shared_st->cur_co  )
            save_shared_stack(shared_st->cur_co);//保存原共享栈内容
        
    }
    jump_2_context(&(cur->co_stack),&(next->co_stack));
}
void co_yield()
{
    ProcessScheduler* process = GetCurrentProcessScheduler();
    context_t *curl = process->allcos_[process->co_index_];
    context_t *last = process->allcos_[process->co_index_-1];
    //curl 协程结束 相应的调度器里也得把这个排在最前面的改掉
    process->co_index_--;
    coroutine_swap(curl, last);
}
//协程开始函数
void RoutineStartFun(void* args )
{
    context_t* co = (context_t*)args;
    co->is_beg = 1;
    if( co  )
        co->fun( co->args );
    //当前执行完这个函数 栈还需要再切回去
    co->is_end = 1;//中间如果启动了新的协程 这里是不会跑得到的
    co_yield();
}
//非共享栈
context_t* co_create(fn_type fun,void*args )
{
    ProcessScheduler* gProcess = GetCurrentProcessScheduler();
    context_t* co = (context_t*)malloc(sizeof(context_t));
    memset(co,0,sizeof(context_t));
    co->fun = fun;
    co->args = args;

    co->is_shared_stack=0;
    co->co_stack.buf = (char*)malloc(gProcess->co_stack_size); 
    co->co_stack.rbp = co->co_stack.buf + gProcess->co_stack_size;
    //栈顶
    void* rsp = co->co_stack.rbp-sizeof(void*);
    //对齐
    rsp = (void*)((size_t)rsp&(-16L)); //-16 掩码 1111 0000

    co->co_stack.reg[RSP]=rsp;
    co->co_stack.reg[RBP]=0;
    co->co_stack.reg[RIP]=RoutineStartFun;
    co->co_stack.reg[RDI]=co;
    return co;
}
//协程开始启动
void co_start(context_t* co )
{
    ProcessScheduler* gProcess = GetCurrentProcessScheduler();

    context_t* last_co = gProcess->allcos_[gProcess->co_index_];

    gProcess->allcos_[++gProcess->co_index_] = co;


    coroutine_swap(last_co,co);
}
//创建共享栈协程
context_t* co_create_shared_stack(fn_type fun,void*args )
{
    context_t* co = (context_t*)malloc(sizeof(context_t));
    memset(co,0,sizeof(context_t));
    co->fun = fun;
    co->args = args;
    co->is_shared_stack = 1;

    
    co->co_shared_stack =(shared_stack*)malloc(sizeof(shared_stack)); 
    //栈顶
    //共享栈
    common_shared_stack_t* common_stack = GetGlobSharedStackPerThread(GetCurrentProcessScheduler());  

    co->co_shared_stack->common_stack = common_stack;

    void* rsp = common_stack->co_shared_rbp-sizeof(void*);
    rsp = (void*)((size_t)rsp & (-16L));

    co->co_stack.reg[RSP]=rsp;//共享站 栈顶指针
    co->co_stack.reg[RBP]=0;
    co->co_stack.reg[RIP]=RoutineStartFun;
    co->co_stack.reg[RDI]=co;
    return co;

}
void save_shared_stack(context_t* co)
{
    //共享栈实际使用的内存是从rbp->rsp之间的内存
    //栈帧是从大到小的 栈顶指针存在在rsp  栈基在寄存器里面
    size_t  len =  co->co_shared_stack->common_stack->co_shared_rbp - co->co_shared_stack->rsp;
    if( co->co_shared_stack->buf )
        free(co->co_shared_stack->buf);
    co->co_shared_stack->buf = (char*)malloc(len);
    memcpy(co->co_shared_stack->buf, co->co_shared_stack->rsp, len);
}


