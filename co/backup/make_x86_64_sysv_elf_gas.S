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
.globl make_fcontext
.type make_fcontext,@function
.align 16
make_fcontext:
    /* first arg of make_fcontext() == top of context-stack */
	/* make_fcontext函数 第一个参数是  make_fcontext(stack_ + stackSize_, stackSize_, fn_); 
	   第一个参数是申请的内存地址的高位 将第一个参数放入rax中 
	   然后取16的整数倍
	*/
    movq  %rdi, %rax

    /* shift address in RAX to lower 16 byte boundary */
    andq  $-16, %rax

    /* reserve space for context-data on context-stack */
    /* size for fc_mxcsr .. RIP + return-address for context-function */
    /* on context-function entry: (RSP -0x8) % 16 == 0 */
	//mov是取内存上的值 lea是仅仅取内存地址 这里也就是内存向下偏移48个字节,rax是传入的第一个参数,即调用时传进来的内存
	//内存+size  
	//三个参数的寄存器分别位 rdi rsi rdx
	//这里将申请的内存地址-72个字节 (0x48=72)
    leaq  -0x48(%rax), %rax

    /* third arg of make_fcontext() == address of context-function */
	//这里把第三个参数 也就是那个函数指针(8个字节的长度)
	//也就是最前面 其实是空了16个字节                         (48)函数指针结束      (56)函数指针开始的地方      内存块(72)
    movq  %rdx, 0x38(%rax)

	//这两个好像是跟浮点数相关的处理的寄存器 反正就是也保存起来
    /* save MMX control- and status-word */
    stmxcsr  (%rax)
    /* save x87 control-word */
    fnstcw   0x4(%rax)

	//这里是存了一个finish函数的地址, 意思是 如果上面的函数执行完没有跳到其他的有效的context 就走到这里的exit了,相当于就出现了异常了,这里
	//finish直接调用了 _exit会结束进程,正常情况这里是不应该走到的
	//这里取的是跟 rip 和finish函数的相对值
    /* compute abs address of label finish */
    leaq  finish(%rip), %rcx
    /* save address of finish as return-address for context-function */
    /* will be entered after context-function returns */
    movq  %rcx, 0x40(%rax)

	//返回的时候 rax 返回值是存在rax的 所以外面的指针最终会收到rax的地址
	//函数指针在 rax+0x38的位置(占用8个字节) 然后8个字节后也就是0x40 就是finish函数
    ret /* return pointer to context-data */

finish:
    /* exit code is zero */
    xorq  %rdi, %rdi
    /* exit application */
    call  _exit@PLT
    hlt
.size make_fcontext,.-make_fcontext

/* Mark that we don't need executable stack. */
.section .note.GNU-stack,"",%progbits
