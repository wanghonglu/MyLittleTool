.text
.globl jump_2_context
.align 16
/*
	rdi 参数1 当前运行的栈帧  rdi
	rsi 参数2 准备跳往的栈帧  rsi
*/
jump_2_context:
	//第一个参数保存 原来那个栈的esp
	//保存和下面的弹出是恰好相反的
	movq  %rsp, 0(%rdi)
	movq  %rbp,8(%rdi)
	
	//指令寄存器的值恰好再上一次调用call时压入栈中即此时的 *rsp
	leaq (%rsp),%rax
	//这个地方是把 rsp的值赋值给16(%rdi)
	movq  0(%rax),%rax
	movq %rax,16(%rdi)
	//rid 先暂时存在rax中
	movq %rdi,24(%rdi)
	movq %rbx,32(%rdi)
	movq %r12,40(%rdi)
	movq %r13,48(%rdi)
	movq %r14,56(%rdi)
	movq %r15,64(%rdi)
	movq %rdx,72(%rdi)
	movq %rcx,80(%rdi)
	movq %r8,88(%rdi)
	movq %r9,96(%rdi)
	movq %rdi,104(%rdi)



	//以上操作完成将当前栈帧 rsp rbp rip存入参数1中 下面操作将参数二的这三个值放入cpu的寄存器中
	// 恢复 rbp
	movq 0(%rsi), %rsp
	movq 8(%rsi), %rbp
	movq 24(%rsi),%rdi

	movq 32(%rsi),%rbx
	movq 40(%rsi),%r12
	movq 48(%rsi),%r13
	movq 56(%rsi),%r14
	movq 64(%rsi),%r15

	movq 72(%rsi),%rdx
	movq 80(%rsi),%rcx
	movq 88(%rsi),%r8
	movq 96(%rsi),%r9

	//函数指针压入栈顶
	pushq 16(%rsi)
	movq 104(%rsi),%rsi

	ret
