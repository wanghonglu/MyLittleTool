#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<string.h>
typedef void(*fn_type)(void*,void*);
typedef struct context_t{
    void*  rsp;//栈顶指针
    void*  rbp;//栈基指针
    fn_type fun;
    void* args1;
    void* args2;
    void*  st;
}context_t;
extern void jump_2_context(context_t * cur,context_t* pending );
#define Co_number 2
context_t  stack[Co_number];
context_t  main_stack;
void fun1(int* a,int* b)
{
    int c=100;
    printf("1 a=%d b=%d\n", *a, *b);
    jump_2_context(&stack[0],&stack[1]);
    printf("%d\n", *a+*b+c);
    jump_2_context(&stack[0],&stack[1]);
}
void fun2(int* a,int* b)
{
    int c=200;
    printf("2\n");
    jump_2_context(&stack[1],&stack[0]);
    printf("%d\n", *a+*b+c);
    jump_2_context(&stack[1],&main_stack);
}
typedef void(*routine_fun)(void*);
typedef struct regs{
    void* args;
    routine_fun  fun;
}regs;
void RoutineFun( context_t *ctx  )
{
    ctx->fun(ctx->args1, ctx->args2 );
}
void make_stack(context_t* ctx, fn_type fun,void* args1, void*args2 )
{
    memset(ctx, 0, sizeof(context_t));

    ctx->fun = fun;
    ctx->args1 = args1;
    ctx->args2 = args2;

    ctx->st = malloc(1*1024*128);//128k
    void*tmp = ctx->st+1024*128-sizeof(regs);
    //对齐
    tmp = (void*)((size_t)tmp&(-16L)); //-16 掩码 1111 0000
#if 0
    regs* reg = (regs*)tmp;
    reg->fun = (routine_fun)RoutineFun;
    reg->args = ctx;
#endif
    void** reg = &tmp;
    *reg = (routine_fun)RoutineFun;

    ctx->rsp = tmp;



}
void clear_stack(context_t* ctx )
{
    free(ctx->st);
}
int main(int argc,char**argv)
{
    int a=1,b=2;

    make_stack(&stack[0],(fn_type)fun1,&a,&b);
    make_stack(&stack[1],(fn_type)fun2,&a,&b);

    jump_2_context(&main_stack, &stack[0]);

    printf("end\n");

    clear_stack(&stack[0]);
    clear_stack(&stack[0]);

    return 0;
}
