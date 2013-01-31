	.file	"pthread_attr.c"
.globl count
	.bss
	.align 4
	.type	count, @object
	.size	count, 4
count:
	.zero	4
	.section	.rodata
.LC0:
	.string	"count = %d\n"
	.text
.globl start
	.type	start, @function
start:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	movl	count, %eax
	addl	$1, %eax
	movl	%eax, count
	movl	count, %eax
	movl	%eax, 4(%esp)
	movl	$.LC0, (%esp)
	call	printf
	movl	$0, (%esp)
	call	start
	leave
	ret
	.size	start, .-start
	.section	.rodata
.LC1:
	.string	"\351\273\230\350\256\244\346\240\210\345\244\247\345\260\217: %d\n"
	.text
.globl main
	.type	main, @function
main:
	leal	4(%esp), %ecx
	andl	$-16, %esp
	pushl	-4(%ecx)
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ecx
	subl	$68, %esp
	leal	-40(%ebp), %eax
	movl	%eax, (%esp)
	call	pthread_attr_init
	leal	-44(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	-40(%ebp), %eax
	movl	%eax, (%esp)
	call	pthread_attr_getstacksize
	movl	-44(%ebp), %edx
	movl	%edx, %eax
	sarl	$31, %eax
	shrl	$22, %eax
	addl	%edx, %eax
	sarl	$10, %eax
	movl	%eax, 4(%esp)
	movl	$.LC1, (%esp)
	call	printf
	movl	$1048576, 4(%esp)
	leal	-40(%ebp), %eax
	movl	%eax, (%esp)
	call	pthread_attr_setstacksize
	movl	$0, 12(%esp)
	movl	$start, 8(%esp)
	leal	-40(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	-48(%ebp), %eax
	movl	%eax, (%esp)
	call	pthread_create
	movl	-48(%ebp), %eax
	movl	$0, 4(%esp)
	movl	%eax, (%esp)
	call	pthread_join
	addl	$68, %esp
	popl	%ecx
	popl	%ebp
	leal	-4(%ecx), %esp
	ret
	.size	main, .-main
	.ident	"GCC: (GNU) 4.1.2 20070626 (Red Hat 4.1.2-14)"
	.section	.note.GNU-stack,"",@progbits
