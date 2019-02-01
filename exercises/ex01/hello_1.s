	.file	"hello.c"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"%d x %d = %d\n"
	.text
	.globl	verify
	.type	verify, @function
verify:
.LFB23:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	movl	%edi, %ebp
	movl	%esi, %r12d
	movl	%edx, %ebx
	movl	%edi, %r8d
	movl	%edx, %ecx
	movl	%esi, %edx
	movl	$.LC0, %esi
	movl	$1, %edi
	movl	$0, %eax
	call	__printf_chk
	imull	%r12d, %ebx
	cmpl	%ebp, %ebx
	sete	%al
	movzbl	%al, %eax
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE23:
	.size	verify, .-verify
	.globl	mult
	.type	mult, @function
mult:
.LFB24:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movl	%esi, %edx
	movl	$0, %ecx
	movl	$0, %ebx
	testl	%edi, %edi
	jg	.L11
	jmp	.L5
.L6:
	addl	$1, %ebx
	cmpl	%eax, %ebx
	jne	.L6
	movl	%r8d, %ebx
.L7:
	addl	$1, %ecx
	cmpl	%ecx, %edi
	je	.L5
.L11:
	testl	%edx, %edx
	jle	.L7
	leal	(%rbx,%rdx), %r8d
	movl	%r8d, %eax
	jmp	.L6
.L5:
	movl	%edi, %esi
	movl	%ebx, %edi
	call	verify
	testl	%eax, %eax
	je	.L8
	movl	%ebx, %eax
	jmp	.L3
.L8:
.L3:
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE24:
	.size	mult, .-mult
	.section	.rodata.str1.1
.LC1:
	.string	"Hello, World!"
.LC2:
	.string	"%d\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB25:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movl	$.LC1, %edi
	call	puts
	movl	$3, %esi
	movl	$2, %edi
	call	mult
	movl	%eax, %edx
	movl	$.LC2, %esi
	movl	$1, %edi
	movl	$0, %eax
	call	__printf_chk
	movl	$0, %eax
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE25:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.10) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
