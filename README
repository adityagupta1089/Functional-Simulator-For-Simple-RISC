================================================
Functional Simulator for simpleRISC processor
================================================

README

Table of contents
1. Directory Structure
2. Software description      
3. How to build
4. How to execute
5. Output
6. Test Results

Directory Structure:
--------------------
CS112-Project
  |
  |- bin
      |
      |- mySimpleSim
  |- include
      |
      |- mySimpleSim.h
  |- src
      |- main.c
      |- Makefile
      |- mySimpleSim.c
  |- test
      |- simple_add.mem
      |- prime.mem
      |- matrix.mem
      |- lcm.mem
      |- gcd.mem
      |- fibonacci_recursive.mem
      |- factorial_recursive.mem
      |- factorial_iterative.mem
      |- assignment4.mem
      |- array_add.mem 
      
      
Software description      
--------------------

            +------------------------------------------------+
            |                                                |
            | FUNCTIONAL_SIMULATOR_FOR_SIMPLE-RISC_PROCESSOR |
            |                                                |
            +------------------------------------------------+


  SimpleRISC ISA
 +--------------+

-> SimpleRisc is a simple, generic, complete and consice RISC ISA.
-> The assembly language of Simplerisc has only 21 instructions.
    - Register Transfer Instruction (mov)
    - Arithmetic Instructions (add,sub,mul,mod,div,cmp)
    - Logical instructions (and,or,not)
    - Shift Instructions (lsr,lsl,asr)
    - Data Transfer Instructions (ld,st)
    - Unconditional Branch Instruction (b)
    - Conditional Branch Instructions (beq,bgt)
    - Function Call/Return Instructions (call,ret)
    - Do Nothing Instruction (nop)

-> SimpleRisc assumes that we have only 16 registers(R0,R1.....R15)
    - Registers R0,R1...R13 are the general purpose registers.
    - Register R14 is the stack pointer.
    - Register R15 is the return address
    - Each register is a 32 bit wide.

-> Each instruction is encoded into a 32 bit value.
-> The memory is a large array of bytes. A part of it saves the 
   instructions and the rest of it is devoted to store the data.     
-> Each Simple Risc instruction can be divided in 3 instruction formats:
    - Branch:    Contains a 5-bit opcode and 27 bit offset.
    - Register:  Encodes a 3-address instruction with two register source 
                 operands and one register destination operand.
    - Immediate: Encodes a 3-address instruction that has an immediate as 
                 ne of the operands.






  Simulator for SimpleRISC processor
 +----------------------------------+

 -> Our Simulator is computer program written in a C language the simulates the 
    working of the SimpleRISC processor that run over the SimpleRISC ISA.
 -> The program takes the instructions from the .mem file.
 -> Each time the program takes the new instruction file, it resets the Register 
    file, PC and the instruction memory array MEM array.
 -> After reseting, it reads the instruction file and populates the instruction
    memory array ‘MEM’.
 -> Finally the run_simplesim() method is run which calls the following
    functions:
     - fetch(): Reads from the instruction memory array MEM according to the PC
                value and updates the instruction register.
     - decode(): Reads the instruction register, reads operand1,operand2 from
                 register file.It also calculates the values of the immediate 
                 operand and the branch target by unpacking the offset embedded
                 in the instruction.
     - execute(): Calculates the control signals based on the opcode present in
                  the instruction. Executes the ALU operation based on the
                  control signal calculated and updates the other signals.It 
                   also calculates the branchPC.
     - mem(): Performs two operations, either reads the MEM array or writes the
              MEM array based on the control signal.

     - write_back(): Writes the results back to register file.



How to build
------------
For building:
    $cd src
    $make

For cleaning the project:
    $cd src
    $make clean

How to execute
--------------
	$cd src
	$../mySimpleSim ../test/input_file.mem


Output
------
 After the execution of the program, the MEM array is written in the 
 'data_out.mem' file and the state of the variables is printed on the 
 console as shown:
 
 
 +--------------------------------------------------------------------------------------+
 |        ======================================================================        |
 |        CYCLE ****                                                                    |
 |        ======================================================================        |
 |        Instruction Fetch:                                                            |  
 |             Read 0x******** (instruction) from * (PC)                                |
 |          ---------------------------------------------------------------             |
 |        Decode:                                                                       |
 |             Immediate and Branch Target Calculation:                                 |
 |                Calculated immediate as * using u=*,h=*                               | 
 |                and branch target as **********                                       |
 |             Operand Calculation:                                                     | 
 |                Calculated operand(1) = *, operand(2) = *                             |
 |             ---------------------------------------------------------------          | 
 |        Execute:                                                                      |
 |             Control Signals:                                                         |
 |                Calculated opcode as * and respective signals:                        |
 |                    isAdd(*),isSub(*),isCmp(*),isMul(*),isDiv(*)                      | 
 |                    isMod(*),isLsl(*),isLsr(*),isAsr(*), isOr(*)                      | 
 |                    isAnd(*),isNot(*),isMov(*),isCall(*),isLd(*)                      | 
 |                    isUBranch(*),isBgt(*),isBeq(*),isWb(*),                           |
 |                    isImmediate(*)                                                    |
 |             ALU Unit:                                                                |
 |                Performed required operation on A=*, B=* to get                       |
 |                aluResult=*, gt=*,eq=*                                                |
 |             Branch Unit:                                                             |
 |                Calculated Branch PC as ********** and isBranchTaken(*)               | 
 |             ---------------------------------------------------------------          |
 |        Memory:                                                                       |
 |           No load/store to do                                                        | 
 |           ---------------------------------------------------------------            |
 |        Write Back:                                                                   |
 |             Stored * intro R[*]                                                      |
 |             ---------------------------------------------------------------          | 
 |       Summary:                                                                       |
 |           PC=***:                                                                    |
 |             r00=****, r01=****, r02=****, r03=****, r04=****,                        |
 |             r05=****, r06=****, r07=****, r08=****, r09=****,                        |
 |             r10=****, r11=****, r12=****, r13=****, sp=****, ra=****.                |
 |                                                                                      |
 +--------------------------------------------------------------------------------------+


Test Results
--------------
1. simple_add.mem
    Calculates 4(r1)+12(r2)=16(r3).
    
2. prime.mem
    Calculats whether 1729 (r1) is prime or not  
    (r0=isPrime) which comes out to be 0 (divisible
    by 7)
    
3. matrix.mem
    See corresponding '.asm' file for this testcase 
    involving 
    matrix operations with ld/st.
    
4. lcm.mem
    Finds lcm of r1(1963) and r2(1729) into r0(261079).
    
5. fibonacci_recursive.mem
    Calculates n(th) [=15] fibonaaci number, i.e. 377 into 
    r2
    f0=1, f1=1
    
6. factorial_recursive.mem
    Calculates factorial of 12(r0), i.e. 479001600 into r1
    
7. assignment4.mem
    See 'ass4.asm' and 'ass4.c' for this testcase invloving 
    chained (3 total) functions and switch case.
    Stores result 29416(d) into r6.
    
8. array_add.mem
    Creates an array of size 100 and initializes A[i]=i and
    sums them to get 5050 into r2.
    
9. gcd.mem
	Finds gcd of 5142(r0) and 9936(r1) into r0(6)
	
10. factorial_iterative.mem
	Finds factorial of 10(r0) into r1(3628800)
