.text
.globl jump_2_context
.align 16
/*
	rdi 参数1 当前运行的栈帧  rdi
	rsi 参数2 准备跳往的栈帧  rsi
*/
jump_2_context:
	//第一个参数保存 原来那个栈的esp
	movq  %rsp, 0(%rdi)
	movq  %rbp,8(%rdi)
	//指令寄存器 rip 这个没法直接访问,当执行call的时候 会默认执行一次 push %rip 即把rip 入栈
	//即取出栈内第一个值即可


	//以上操作完成将当前栈帧 rsp rbp rip存入参数1中 下面操作将参数二的这三个值放入cpu的寄存器中
	// 恢复 rbp
	movq 8(%rsi), %rbp
	movq (%rsi), %rsp
	//同样的 rip寄存器也没办法直接访问,但是函数调用结束的时候 ret指令,会默认执行 pop %rip 即将栈顶的值恢复到rip寄存器里
	//所以这里将栈顶改成相应的值即可
	ret
	
