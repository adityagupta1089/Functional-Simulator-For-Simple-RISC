@ r0 N
@ r1 i
@ r2 i1
@ r3 j
@ r4 temp
@ r5 k
@ r6 i2
@ r7 temp2
@ r8 p1
@ r9 p2
@ r10 i3
@ r11 4*N
@ r12 indexcalculation
.main:
    mov r0, 5           @N=5
    mul r11, r0, 4      @r11<-4N
    mov r1, 0           @i=0
    mov r2, 0           @i1=0
    b .loopi    
.loopi:
    cmp r1, r0          @if(i<N)
    bgt .multiply
    beq .multiply
    mov r3, 0           @j=0
    b .loopj
.loopic:
    add r1, r1, 1       @i++
    b .loopi
.loopj:
    cmp r3, r0          @if(j<N)
    bgt .loopic
    beq .loopic
    add r4, r1, r3      @temp=i+j   
    st r4, 100[r2]    @A[i1]=temp
    mul r4, r4, 2       @temp=2*temp=2*i+2*j
    add r4, r4, r3      @temp=temp+j=2*i+3*j
    st r4, 200[r2]    @B[i1]=temp
    add r2, r2, 4       @i1=i1++
    add r3, r3, 1       @j++
    b .loopj
.multiply:
    mov r1, 0           @i=0
    mov r2, 0           @i1=0
    mov r10, 0          @i3=0
    b .loopi2
.loopi2:
    cmp r1, r0          @if(i<N)
    bgt .exit
    beq .exit   
    mov r3, 0           @j=0
    mov r6, 0           @i2=0
    b .loopj2
.loopi2c:
    add r1, r1, 1       @i++    
    b .loopi2
.loopj2:
    cmp r3, r0          @if(j<N)
    bgt .loopi2c
    beq .loopi2c
    mov r4, 0           @temp=0
    mov r5, 0           @k=0
    mul r2, r0, r1      @i1=N*i
    mul r2, r2, 4       @i1*=4
    mul r6, r3, 4       @i2=4*j
    b .loopk
.loopj2c:
    st r4, 300[r10]   @C[i3]=temp
    add r3, r3, 1       @j++    
    add r10, r10, 4     @i3+=4
    b .loopj2
.loopk:
    cmp r5, r0          @if(k<N)
    bgt .loopj2c    
    beq .loopj2c
    ld r8, 100[r2]    @p1=A[i1]
    ld r9, 200[r6]    @p2=B[i2]
    mul r7, r8, r9      @temp2=p1*p2
    add r4, r4, r7      @temp+=temp2
    add r5, r5, 1       @k++    
    add r2, r2, 4       @i1+=4  
    add r6, r6, r11     @i2+=4*N
    b .loopk
.exit:
