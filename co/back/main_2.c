#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<string.h>
#include "co_ctx.h"
#define Co_number 2
context_t  stack[Co_number];
context_t  main_stack;
void fun1(int* a,int* b,int*c)
{
    int d=100;
    printf("1 a=%d b=%d c=%d\n", *a, *b,*c);
    jump_2_context(&stack[0],&stack[1]);
    printf("%d\n", *a+*b+*c+d);
    jump_2_context(&stack[0],&stack[1]);
}
void fun2(int* a,int* b,int*c)
{
    int d=200;
    printf("2 a=%d b=%d c=%d \n", *a, *b,*c);
    jump_2_context(&stack[1],&stack[0]);
    printf("%d\n", *a+*b+*c+d);
    jump_2_context(&stack[1],&main_stack);
}
int main(int argc,char**argv)
{
    int a=1,b=2,c=3;


    printf("context_t::rsp%ld \n",(size_t)(&(((context_t*)0)->reg)));

    make_stack(&stack[0],(fn_type)fun1,&a,&b,&c);
    make_stack(&stack[1],(fn_type)fun2,&a,&b,&c);

    printf("stack1:%p stack2:%p \n", &stack[0],&stack[1]);

    void* rax1 = jump_2_context(&main_stack, &stack[0]);

    printf("end\n");

    clear_stack(&stack[0]);
    clear_stack(&stack[1]);

    return 0;
}
