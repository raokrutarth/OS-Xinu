
.text
.globl net2hostl


net2hostl:
	push %ebp
	mov %esp, %ebp
	
	mov 8(%ebp), %eax //load parameter in EAX
	bswap %eax // Swap bytes
	
	mov %ebp, %esp
	pop %ebp
	ret
	
