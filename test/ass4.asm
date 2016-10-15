.main:
	@ declaring values of a-c
	mov r0, 10
	mov r1, 12
	mov r2, -1
	@ we will now call f which will store the returned value
	@ to r6 and we are taking a convention that the function 
	@ arguments are r0, r1, ... and so on till required, in
	@ case of f we will only need r0, r1, r2. which
	@ incidentally collide with original definition of a,b,c
	@ also we don't a,b,c after function call so we don't
	@ store them, optimizing the code 
	call .f
	@ print the value returned
	.print r6
	b .exit
.f:
	@ function arguments are r0, r1, r2
	@ for creating a for loop we use i=0 as r3 and n=5 as r4
	mov r3, 0
	mov r4, 5
	@ now loop
	b .f_loop
.f_loop:
	@ comparing i<n
	cmp r3, r4
	@ if not exit loop (second part of f is in f2)
	beq .f2
	bgt .f2
	@ a=b+c
	add r0, r1, r2
	@ b=a-c
	sub r1, r0, r2
	@ now we are calling g so we need to store a,b,c,i,n and
	@ ra which require 6x4 bytes = 24 bytes
	sub sp,sp,20
	st r0, [sp]
	st r1, 4[sp]
	st r2, 8[sp]
	st r3, 12[sp]
	st r4, 16[sp]
	st ra, 20[sp]
	@ g will use r0, r1 and r2 as parameters which again
	@ conincide with original definition of a,b,c
	call .g
	@ g has returned the value in r6, but first load all 
	@ values back
	ld r0, [sp]
	ld r1, 4[sp]
	ld r2, 8[sp]
	ld r3, 12[sp]
	ld r4, 16[sp]
	ld ra, 20[sp]
	add sp, sp, 20
	@ c=c+g(a,b,c)
	add r2, r2, r6
	@ i=i+1
	add r3, r3, 1
	@ loop back
	b .f_loop
.f2:
	@ return value should be a+b+c (in r6)
	add r6, r0, r1
	add r6, r6, r2
	ret
.g:
	@ function arguments are in r0, r1, r2
	@ now h wont modify our arguments in r0, r1, r2
	@ so we can optimize our code by not loading/storing
	@ functions arguments (but only ra) to h would be r0-r3 
	@ where r3 would be d. again g would return values in r6
	@ d=r3=0 => h(a,b,c,0)
	mov r3, 0
	sub sp,sp,4
	st ra, [sp]
	call .h
	ld ra, [sp]
	add sp,sp,4
	@ a=r6
	mov r0, r6
	@ d=r3=1=> h(a,b,c,1)
	mov r3, 1
	sub sp,sp,4
	st ra, [sp]
	call .h
	ld ra, [sp]
	add sp,sp,4
	@ b=r6
	mov r1, r6
	@ d=r3=2 => h(a,b,c,2)
	mov r3, 2
	sub sp,sp,4
	st ra, [sp]
	call .h
	ld ra, [sp]
	add sp,sp,4
	@ c=r6
	mov r2, r6
	@ now r6 should be a+b+c
	add r6, r0, r1
	add r6, r6, r2
	ret
.h:
	@ now our arguments are in r0-r3
	@ we compare to 0,1,2 for switch case
	@ if d==0
	cmp r3, 0
	beq .h0
	@ if d==1
	cmp r3, 1
	beq .h1
	@ if d==2
	cmp r3, 2
	beq .h2
.h0:
	@ return a+b-c
	add r6, r0, r1
	sub r6, r6, r2
	@ return 
	ret
.h1:
	@ return a-b+c
	sub r6, r0, r1
	add r6, r6, r2
	@ return 
	ret
.h2:
	@ return a+b+c
	add r6, r0, r1
	add r6, r6, r2
	@ return 
	ret
.h3:	
	@ return 0
	mov r6, 0
	ret	
.exit:	
