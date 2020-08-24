#include "co_ctx.h"
void RoutineFun( context_t *ctx  )
{
    ctx->fun(ctx->args1, ctx->args2,ctx->args3 );
}
//函数栈帧
void make_stack(context_t* ctx, fn_type fun,void* args1, void*args2,void*args3 )
{
    memset(ctx, 0, sizeof(context_t));
  ctx->fun = fun;
  ctx->args1 = args1;
  ctx->args2 = args2;
  ctx->args3 = args3;


  ctx->st = malloc(1*1024*128);//128k
  void* tmp = ctx->st+1024*128;
  //对齐
  tmp = (void*)((size_t)tmp&(-16L)); //-16 掩码 1111 0000

  //之所以需要中间一层调用是为了更方便的构造函数栈帧
  //每个不同的函数,里面有不同的局部变量式 函数栈帧的结构是不一样的

  ctx->reg[RSP]=tmp;
  ctx->reg[RBP]=0;
  ctx->reg[RIP]=RoutineFun;
  ctx->reg[RDI]=ctx;
}
void clear_stack(context_t* ctx )
{
if( ctx->st )
    free(ctx->st);
}
#if 0
ProcessScheduler* ProcessSchedulerInit()
{
    ProcessScheduler* scheduler=(ProcessScheduler*)malloc(sizeof(ProcessScheduler));
    memset(scheduler,0,sizeof(ProcessScheduler));
    scheduler->co_index_=0;
    //创建一个0号协程,0号协程 即主协程,这个协程跟别的不一样 这个协程在系统的栈上
    //其他的在堆上,这个协程不开辟空间
    context_t* main_co = (context_t*)malloc(sizeof(context_t)); 
    memset(main_co, 0x00, sizeof(context_t));

    return scheduler;
}
ProcessScheduler* GetCurrentProcessScheduler()
{
if(!gPerThreadProcessScheduler)
    gPerThreadProcessScheduler = ProcessSchedulerInit();

return gPerThreadProcessScheduler;
}
#endif

