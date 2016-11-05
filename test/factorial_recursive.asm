mov r0, 12
call .factorial_n
b .exit
.factorial_n:
	cmp r0, 1
	beq .factorial_1
	bgt .factorial_n_minus_1
	b .factorial_1
.factorial_n_minus_1
	sub r14, r14, 8
	st r0, 0[r14]
	st r15, 4[r14]
	sub r0, r0, 1
	call .factorial_n
	ld r2, 0[r14]
	ld r15, 4[r14]
	mul r1, r2, r1
	add r14, r14, 8
	ret
.factorial_1:
	mov r1, 1
	ret
.exit:

