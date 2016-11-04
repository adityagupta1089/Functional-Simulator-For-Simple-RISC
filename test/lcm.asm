I0:	mov r1, 1729
I1:	mov r2, 1963
I2:	mov r3, 1
I3:	mov r4, r1
I4:	.loop:
I5:		mod r5, r4, r2
I6:		cmp r5, 0
I7:		beq .lcm
I8:		add r3, r3,1
I9:		mul r4, r1, r3
I10:	b .loop
I11:.lcm:
I12:	mov r0, r4
