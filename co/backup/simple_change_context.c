#include<stdio.h>
void f1();
void f2();
void f3();
void f4();
unsigned long last_rip =0;
//g通用约束 可以使用寄存器/内存/立即数等等
/*
 *  因为函数调用会先压入上一个函数的rbp(栈基指针)所以我们先把这个栈基指针 pop掉 然后压入我们想跳转的函数指针
 *  
 *  ret指令其实是 popq %rip 即指令寄存器被改成我们想让跳转的函数
 *
 *  这里保证了 ret pop出来的是我们指定的函数指针,但是真正的rip 里面存的还是主函数的下一步
 *  这样执行完f4之后 函数还可以回到main函数里面继续往下执行
 * */
#define ChangeRip(fun)  \
    __asm__ __volatile__ (\
    "movq  %0, %%rax \n\t"\
    "popq %%rbx \n\t"\
    "pushq %%rax \n\t"\
    "ret \n\t"::"g"(fun)\
    );
void f1()
{
    printf("1\n");
    ChangeRip(f3);
}
void f2()
{
    printf("2\n");
    ChangeRip(f4);
}
void f3()
{
    printf("3\n");
    ChangeRip(f2);
}
void f4()
{
    printf("4\n");
}
int main()
{
    f1();
    printf("5\n");
    return 0;
}

