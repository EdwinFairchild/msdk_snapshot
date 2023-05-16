
	.text
	.global psr_set

psr_set:
	mrs	r3, cpsr
	orr	r3, r3, r0
	msr	cpsr, r3
	mov	pc, lr	
