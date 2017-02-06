	.file	"net2hostlgcc.c"
	.text
	.globl	net2hostlgcc
	.type	net2hostlgcc, @function
net2hostlgcc:
.LFB0:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$16, %esp
	movl	8(%ebp), %eax
	movl	%eax, -4(%ebp)
	movl	-4(%ebp), %eax
	sarl	$24, %eax
	movl	%eax, %edx
	movl	-4(%ebp), %eax
	sall	$8, %eax
	andl	$16711680, %eax
	orl	%eax, %edx
	movl	-4(%ebp), %eax
	sarl	$8, %eax
	andl	$65280, %eax
	orl	%eax, %edx
	movl	-4(%ebp), %eax
	sall	$24, %eax
	orl	%edx, %eax
	movl	%eax, -4(%ebp)
	movl	-4(%ebp), %eax
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	net2hostlgcc, .-net2hostlgcc
	.ident	"GCC: (Gentoo 4.9.3 p1.5, pie-0.6.4) 4.9.3"
	.section	.note.GNU-stack,"",@progbits
