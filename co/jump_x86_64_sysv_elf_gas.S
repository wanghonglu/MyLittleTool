/*
            Copyright Oliver Kowalke 2009.
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENSE_1_0.txt or copy at
            http://www.boost.org/LICENSE_1_0.txt)
*/

/****************************************************************************************
 *                                                                                      *
 *  ----------------------------------------------------------------------------------  *
 *  |    0    |    1    |    2    |    3    |    4     |    5    |    6    |    7    |  *
 *  ----------------------------------------------------------------------------------  *
 *  |   0x0   |   0x4   |   0x8   |   0xc   |   0x10   |   0x14  |   0x18  |   0x1c  |  *
 *  ----------------------------------------------------------------------------------  *
 *  | fc_mxcsr|fc_x87_cw|        R12        |         R13        |        R14        |  *
 *  ----------------------------------------------------------------------------------  *
 *  ----------------------------------------------------------------------------------  *
 *  |    8    |    9    |   10    |   11    |    12    |    13   |    14   |    15   |  *
 *  ----------------------------------------------------------------------------------  *
 *  |   0x20  |   0x24  |   0x28  |  0x2c   |   0x30   |   0x34  |   0x38  |   0x3c  |  *
 *  ----------------------------------------------------------------------------------  *
 *  |        R15        |        RBX        |         RBP        |        RIP        |  *
 *  ----------------------------------------------------------------------------------  *
 *  ----------------------------------------------------------------------------------  *
 *  |    16   |   17    |                                                            |  *
 *  ----------------------------------------------------------------------------------  *
 *  |   0x40  |   0x44  |                                                            |  *
 *  ----------------------------------------------------------------------------------  *
 *  |        EXIT       |                                                            |  *
 *  ----------------------------------------------------------------------------------  *
 *                                                                                      *
 ****************************************************************************************/

.text
.globl jump_fcontext
.type jump_fcontext,@function
.align 16
jump_fcontext:
	//jump_fcontext 操作的参数1 是原来的栈指针 2是要跳过去的栈指针 参数3 是拥有该函数的对象的this指针  参数4是否保存fpu 好像是跟浮点数相关 具体有啥用不清楚~~
	//这6个寄存器是被调用方需要保存的寄存器
	//rpb是原来函数的栈基指针
    pushq  %rbp  /* save RBP */
    pushq  %rbx  /* save RBX */
    pushq  %r15  /* save R15 */
    pushq  %r14  /* save R14 */
    pushq  %r13  /* save R13 */
    pushq  %r12  /* save R12 */

    /* prepare stack for FPU */
	//准备8个字节保存  跟浮点数相关的寄存器
	//FPU  float point unit 浮点运算单元  默认是0
    leaq  -0x8(%rsp), %rsp

    /* test for flag preserve_fpu */
    cmp  $0, %rcx
    je  1f

    /* save MMX control- and status-word */
    stmxcsr  (%rsp)
    /* save x87 control-word */
    fnstcw   0x4(%rsp)

1:
    /* store RSP (pointing to context-data) in RDI */
	//rdi是第一个参数 即原来栈对象  把当前的rsp存在这里 后面再调回来 可以继续执行
    movq  %rsp, (%rdi)

    /* restore RSP (pointing to context-data) from RSI */
	//把 rsi是第二个参数,这里就是把第二个参数赋值给 rsp 方便执行第二个函数栈帧
    movq  %rsi, %rsp

    /* test for flag preserve_fpu */
    cmp  $0, %rcx
    je  2f

    /* restore MMX control- and status-word */
    ldmxcsr  (%rsp)
    /* restore x87 control-word */
    fldcw  0x4(%rsp)

2:
    /* prepare stack for FPU */
	//因为存放下面的rbp栈基指针的时候 这里是偏移了8个字节的
    leaq  0x8(%rsp), %rsp

	//把之前存储的 rbp栈基指针 和几个需要被调函数保存的寄存器弹出来
    popq  %r12  /* restrore R12 */
    popq  %r13  /* restrore R13 */
    popq  %r14  /* restrore R14 */
    popq  %r15  /* restrore R15 */
    popq  %rbx  /* restrore RBX */
    popq  %rbp  /* restrore RBP */

    /* restore return-address */
	//这里 相当于在之前存这些数据之前 这里的8个字节其实是rip寄存器
	//因为 当执行 call context_jump的时候  要做的第一步便是 push %rip rip代表函数要执行的下一步,本来调转到context_jump执行完后 rip再次指向下一步
	//但我们jump其实没做什么,所以这里可以再次pop出来 继续往下执行 等于r8现在存的是调用jump之前的下一步rip
    popq  %r8

    /* use third arg as return-value after jump */
	// 这里是把第三个参数作为返回值 返回出去 这样就能实现两个协程之前传递数据
    movq  %rdx, %rax
    /* use third arg as first arg in context function */
	//把第三个参数 作为新协程入口的 第一个参数 其实是this指针 这个this指针是新协程对象的this指针  
    movq  %rdx, %rdi

    /* indirect jump to context */
	//跳到之前执行jump之前的下一步的地方!!!
    jmp  *%r8
.size jump_fcontext,.-jump_fcontext

/* Mark that we don't need executable stack.  */
.section .note.GNU-stack,"",%progbits
