#include"coroutine_ctx.h"
#include<stdio.h>
#include<unistd.h>
#include<string.h>
int g_a;
void fun1(void* args )
{
    printf("1\n");
}
void fun2(void* args )
{
    printf("2\n");
}
void fun3(void* args )
{
    printf("3\n");
}
//通过递归调用来消耗程序栈帧
void* beg;
void* end;
void recuresive(int a)
{
    static int b = 0;
    if( !b  )
    {
        __asm__("movq %rbp,beg");
    }
    char  aa[1024]={0};//这样是消耗1k的栈帧
    //fprintf(stderr, "heap aa == %p\n", aa );
    if( a == 0 )
    {
        return;
    }
    g_a = aa[100];//不加这个可能会被编译器优化掉
    if( !b  )
    {
        __asm__ ("movq %rsp,end");
        //栈是从高字节到低字节  所以这个值是 rbp-rsp
        fprintf( stderr, "function cost stack :%ld \n", (size_t)(beg-end ));
        b = 1;
    }
    recuresive(--a);
}
void fun(void*args)
{
    //函数调用占 1040个字节 1000次的话 再加上esp指针 还要压入rdi rip 总共占用应该是1064 算下来 985次左右 及第一个参数 就会超过1M ~~~ 900足够看了
    recuresive(970);
}
int main(int argc,char**argv)
{
    context_t* co1 = co_create(fun1, NULL);
    context_t* co2 = co_create(fun2, NULL);
    context_t* co3 = co_create(fun3, NULL);

    co_start(co2);
    co_start(co1);
    co_start(co3);

    fprintf(stderr, "stack co1 == %p\n", co1 );


    //启动1000个协程 看看非共享栈
    //
    ////堆内存的使用 似乎是copy-on-write的 也就是说如果malloc出来不使用的话
    //top命令看的话 RES那一列 没多大变化 看不到实际内存的情况

    int co_nums = 100;
    if( argc<2 )
        co_nums = 1000;
    else
        co_nums = atoi(argv[1]);
    context_t ** co_array = (context_t**)malloc(sizeof(context_t*)*co_nums);

    for( size_t i=0;i<co_nums;i++  )
        co_array[i] = co_create(fun, (void*)i);
    printf("create coroutine ok\n");

    for( size_t i=0;i<co_nums-1;i++  )
    {
        co_start(co_array[i]);
    }

    printf("end\n");


    for( int i=0;i<co_nums;i++ )
    {
        clear_stack(co_array[i]);
    }


    clear_stack(co1);
    clear_stack(co2);
    clear_stack(co3);

    fprintf(stderr, "all end\n");
}
