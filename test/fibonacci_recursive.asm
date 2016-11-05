@NOT-FIXED!
mov r0, 15
call i3
b i33
cmp r0, 0
beq i8
cmp r0, 1
beq i10
bgt i12
mov r1, 0
ret
mov r1, 1
ret
sub r14, r14, 8
st r0, 0[r14]
st r15, 4[r14]
sub r0, r0, 1
call i3
mov r2, r1
ld r0, 0[r14]
ld r15, 4[r14]
add r14, r14, 8
sub r14, r14, 12
st r0, 0[r14]
st r15, 4[r14]
st r2, 8[r14]
sub r0, r0, 2
call i3
ld r0, 0[r14]
ld r15, 4[r14]
ld r2, 8[r14]
add r14, r14, 12
add r1, r1, r2
ret

