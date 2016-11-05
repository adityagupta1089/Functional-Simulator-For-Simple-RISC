mov r0, 1
b .store
.store
	cmp r0, 100
    bgt I8
    mul r4, r0, 4
    ST r0, 256[r4]
    add r0, r0, 1
    B .store
mov r0, 1
b .sum
    cmp r0, 100
    bgt .exit
    mul r4, r0, 4
    ld r1, 256[r4]
    add r2, r2, r1
    add r0, r0, 1
    B .sum
.exit
