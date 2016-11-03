//=================================================
//
// The project is developed as part of Computer Architecture class
// Project Name: Functional/Pipeline Simulator for simpleRISC Processor
//
// Developer's Name:     Aditya Gupta
// Developer's Email id: 2015csb1003[at]iitrpr[dot]ac[dot]in
//
// Developer's Name:     Vishal Singh
// Developer's Email id: 2015csb1040[at]iitrpr[dot]ac[dot]in
//=================================================

//=================================================
// mySimpleSim.cpp
//
// Purpose of this file: implementation file for mySimpleSim
//=================================================
#include "../include/mySimpleSim.h"
#include "pipelineRegisters.cpp"
#include <stdlib.h>
#include <stdio.h>

//=================================================
// REGISTER FILE
//=================================================
static unsigned int R[16];
//=================================================
// FLAGS
//=================================================
static int gt, eq;
//=================================================
// MEMORY
//=================================================
static byte MEM[4000];
//=================================================
// PIPELINE REGISTERS
//=================================================
static int PC;
IF_OF if_of;
OF_EX of_ex;
EX_MA ex_ma;
MA_RW ma_rw;

void run_simplesim() {
    while (1) {
        //assume all these on same line
        fetch();
        decode();
        execute();
        mem();
        write_data_memory();
        update(); //negative edge of clock
    }
    /*
     int cycle = 0;
     maj_sep
     while (1) {
     //=================================================
     // SUMMARY
     //=================================================
     printf("CYCLE %04d \n", cycle++);
     maj_sep
     //=================================================
     fetch();
     if (instruction == -1) break;
     decode();
     execute();
     mem();
     write_back();
     //=================================================
     // SUMMARY
     //=================================================
     printf("Summary:\n");
     printf("\tPC=%03d: ", PC);
     for (int i = 0; i < 14; i++) {
     if (i % 5 == 0) printf("\n\t");
     printf("r%02d=%04d, ", i, R[i]);
     }
     printf("sp=%04d, ra=%03d.", R[14], R[15]);
     printf("\n");
     maj_sep
     //=================================================
     PC = isBranchTaken ? branchPC : PC + 4;
     }
     printf("Exiting and Writing output MEM file.\n");
     maj_sep
     write_data_memory();*/
}

//=================================================
// RESET
//
// it is used to set the reset values
// reset all registers and memory content to 0
//=================================================
void reset_proc() {
    for (int i = 0; i < 14; i++)
        R[i] = 0;
    R[14] = 4000 - 8;
    R[15] = 0;
    PC = 0;
    for (int i = 0; i < 4000; i += 4)
        write_word(MEM, i, -1);
}

//=================================================
// LOAD PROGRAM MEMORY
//
//load_program_memory reads the input memory,
//and pupulates the instruction memory
//=================================================
void load_program_memory(char *file_name) {
    FILE *fp;
    unsigned int address, instruction;
    fp = fopen(file_name, "r");
    if (fp == NULL) {
        printf("Error opening input mem file\n");
        exit(1);
    }
    while (fscanf(fp, "%x %x", &address, &instruction) != EOF) {
        write_word(MEM, address, instruction);
    }
    fclose(fp);
}

//=================================================
// WRITE DATA MEMORY
//
//writes the data memory in "data_out.mem" file
//=================================================
void write_data_memory() {
    FILE *fp;
    unsigned int i;
    fp = fopen("data_out.mem", "w");
    if (fp == NULL) {
        printf("Error opening dataout.mem file for writing\n");
        return;
    }

    for (i = 0; i < 4000; i = i + 4) {
        fprintf(fp, "%x %x\n", i, read_word(MEM, i));
    }
    fclose(fp);
}

//=================================================
// FETCH
//
// reads from the instruction memory and updates
// the instruction register
//=================================================
void fetch() {
    word instruction = read_word(MEM, PC);
    if_of.update(PC, instruction);
}

//=================================================
// DECODE
//
// reads the instruction register, reads operand1,
// operand2 from register file, decides the
// operation to be performed in execute stage
//=================================================
void decode() {
    //=================================================
    // CONTROL SIGNALS
    //=================================================
    if (if_of.hasBubble()) return;
    word instruction = if_of.getInstruction();
    int PC = if_of.getPc();
    word opcode = (instruction & 0xF8000000) >> 27;
    Control control;
    bit I = ((instruction & 0x4000000) >> 26);
    control.update(opcode, I);
    
     //=================================================
     // IMMEDIATE AND BRANCH TARGET CALCULATION
     //=================================================
     immx = instruction & 0xFFFF;
     bit u = (instruction & 0x10000) >> 16;
     bit h = (instruction & 0x20000) >> 17;
     if (h) immx <<= 16;
     else if (((instruction & 0x8000) >> 8) && !u) immx |= 0xFFFC0000; //if negative sign extension
     branch_target = (instruction & 0x7FFFFFF) << 2;
     if ((branch_target & 0x10000000) >> 28) branch_target += 0xE0000000; // if negative extending sign
     branch_target += PC;
     //=================================================
     // OPERAND CALCULATION
     //=================================================
     operand1 = control.isRet ? R[15] : R[(instruction & 0x3C0000) >> 18]; //?ra:rs1
     operand2 = control.isSt ? R[(instruction & 0x3C00000) >> 22] : R[(instruction & 0x3C000) >> 14]; //?rd:rs2
     
     
     word A = operand1;
     word B = control.isImmediate ? immx : operand2;
     of_ex.update(PC,branch_target,B,A,operand2,instruction,control);
}

//=================================================
// EXECUTE
//
//executes the ALU operation based on ALUop
//=================================================
void execute() {

    /*
     //=================================================
     // ALU UNIT
     //=================================================
     
     if (isAdd) aluResult = A + B;
     else if (isSub) aluResult = A - B;
     else if (isMul) aluResult = A * B;
     else if (isDiv) aluResult = A / B;
     else if (isMod) aluResult = A % B;
     else if (isCmp) {
     if (A > B) gt = 1;
     else gt = 0;
     if (A == B) eq = 1;
     else eq = 0;
     } else if (isAnd) aluResult = A & B;
     else if (isOr) aluResult = A | B;
     else if (isNot) aluResult = !B;
     else if (isMov) aluResult = B;
     else if (isLsl) aluResult = A << B;
     else if (isLsr) aluResult = A >> B;
     else if (isAsr) aluResult = (word) (((signed int) A) >> B);
     //=================================================
     // BRANCH UNIT
     //=================================================
     branchPC = isRet ? operand1 : branch_target;
     isBranchTaken = isUBranch || (isBeq && eq) || (isBgt && gt);    //isUBranch||(isBeq&&flags.E)||(isBgt&&flags.GT)
     */
}

//=================================================
//MEM
//
//perform the memory operation
//=================================================
void mem() {
    /*
     if (isLd) ldResult = read_word(MEM, aluResult);
     else if (isSt) write_word(MEM, aluResult, operand2);
     */
}

//=================================================
// WRITE BACK
//
//writes the results back to register file
//=================================================
void write_back() {
    /*
     word result = isLd ? ldResult : isCall ? PC + 4 : aluResult;
     if (isWb) {
     if (isCall) R[15] = result;
     else R[(instruction & 0x3C00000) >> 22] = result;
     }
     */
}
//=================================================
void update() {

}
//=================================================
word read_word(byte* mem, word address) {
    word* data;
    data = (word*) (mem + address);
    return *data;    // basically return MEM[address]
}

//=================================================
void write_word(byte* mem, word address, word data) {
    word* data_p;
    data_p = (word*) (mem + address);
    *data_p = data;    //basically MEM[address]=data
}
//=================================================
