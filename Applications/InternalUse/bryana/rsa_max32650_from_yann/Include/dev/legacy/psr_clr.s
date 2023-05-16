
	.text
	.global psr_clr

psr_clr:
	mrs	r3, cpsr
	mov	r1, #0xFFFFFFFF
	eor	r0, r0, r1
	and	r3, r3, r0
	msr	cpsr, r3
	mov	pc, lr	
