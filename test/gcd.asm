.main:				
	mov r0, 5142	@	a = 5142
	mov r1, 9936	@	b = 9936
	cmp r0, r1		@	if(a>b)
	bgt .swap		@		swap
	b .continue		@	else continue
.swap:				
	mov r2, r0		@	temp=a
	mov r0, r1		@	a = b
	mov r1, r2		@	b = temp
.continue:			
	mov r2, 0		@	remainder = 0
	b .gcd			@	gcd
.gcd:				
	mod r2, r1, r0	@	remainder = b % a
	cmp r2, 0		@	if(remainder==0)
	beq .output		@		output
	bgt .swap_rem	@	else swap_rem
.swap_rem:			
	mov r1, r0		@	b = a
	mov r0, r2		@	a = remainder
	b .gcd			@	gcd
.output:			
