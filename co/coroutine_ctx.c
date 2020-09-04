#include<stdio.h>
#include"coroutine_ctx.h"
#include "process_scheduler.h"
void clear_stack(context_t* ctx )
{
   if( ctx->is_shared_stack )
   {
       if(ctx->co_shared_stack->buf )
       {
           free(ctx->co_shared_stack->buf);
       }
       free(ctx->co_shared_stack);
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

    if( next->is_shared_stack )
    {
        //将共享栈上之前使用的协程的
        if( next->co_shared_stack->common_stack->cur_co  )
            save_shared_stack(next->co_shared_stack->common_stack->cur_co);//保存原共享栈内容
    }
    //上面是切出的时候
    jump_2_context(&(cur->co_stack),&(next->co_stack));
    //下面是切回的时候 
    //这个需要重新获取
    ProcessScheduler* process = GetCurrentProcessScheduler();
    context_t *cur_co = process->allcos_[process->co_index_];
    if( cur_co->is_shared_stack && cur_co->co_shared_stack->buf && cur_co->co_shared_stack->len>0 )
    {
        resume_shared_stack(cur_co);
    }
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
//创建共享协程的时候必须把共享栈传进来,不能把共享栈放在调度器里面
//因为如果在协程里面调用协程,两个协程可能会用同一个共享栈,汇编切换里面会发生
//一个协程往里面写 同时一个协程读
context_t* co_create_shared_stack(fn_type fun,void*args,common_shared_stack_t* shared_st  )
{
    context_t* co = (context_t*)malloc(sizeof(context_t));
    memset(co,0,sizeof(context_t));
    co->fun = fun;
    co->args = args;
    co->is_shared_stack = 1;

    co->co_shared_stack =(shared_stack*)malloc(sizeof(shared_stack)); 
    //栈顶
    //共享栈
    co->co_shared_stack->common_stack = shared_st;

    co->co_stack.reg[RSP]=shared_st->co_shared_rbp;//共享站 栈顶指针
    co->co_stack.reg[RBP]=0;
    co->co_stack.reg[RIP]=RoutineStartFun;
    co->co_stack.reg[RDI]=co;
    return co;

}
//保存上下文 从共享栈上保存实际占用内存到自身上
void save_shared_stack(context_t* co)
{
    //共享栈实际使用的内存是从rbp->rsp之间的内存
    //栈帧是从大到小的 栈顶指针存在在rsp  栈基在寄存器里面
    size_t  len =  co->co_shared_stack->common_stack->co_shared_rbp - co->co_shared_stack->rsp;
    if( co->co_shared_stack->buf )
        free(co->co_shared_stack->buf);
    co->co_shared_stack->buf = (char*)malloc(len);
    co->co_shared_stack->len = len;
    memcpy(co->co_shared_stack->buf, co->co_shared_stack->rsp, len);
}
//恢复上下文 把自身保存的内容 恢复到共享栈上
void resume_shared_stack( context_t* co  )
{
    memcpy( co->co_shared_stack->common_stack->co_shared_rbp, co->co_shared_stack->buf, co->co_shared_stack->len );
}
//传入0 默认8M 共享栈
common_shared_stack_t*
create_shared_stack(size_t size )
{
    common_shared_stack_t* shared_st = (common_shared_stack_t*)malloc(sizeof(common_shared_stack_t));
    memset(shared_st,0,sizeof(common_shared_stack_t));

    if(size<1024*1024)
        size = DefaultStackSize;
    shared_st->stack_buf = (char*)malloc(size);
    shared_st->stack_size = size;

    //栈基指针  buf+size;

    shared_st->co_shared_rbp = shared_st->stack_buf+size-sizeof(void*);

    //16字节对齐
    shared_st->co_shared_rbp =(char*)((size_t)shared_st->co_shared_rbp&(-16L));

    return shared_st;
}
void
shared_stack_free(common_shared_stack_t* st )
{
    free(st->stack_buf);
    free(st);
}


