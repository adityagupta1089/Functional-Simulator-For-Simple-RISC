//=================================================
//
// The project is developed as part of Computer Architecture class
// Project Name: Functional/Pipeline Simulator for simpleRISC Processor
//
// Developer's Name:     Aditya Gupta
// Developer's Email id: 2015csb1003[at]iitrpr[dot]ac[dot]in
//
//=================================================

//=================================================
// mySimpleSim.cpp
//
// Purpose of this file: implementation file for mySimpleSim
//=================================================
#include "../include/mySimpleSim.h"
#include <stdlib.h>
#include <stdio.h>

//=================================================
// REGISTER FILE
//=================================================
static unsigned int R[16];
static int PC;
//=================================================
// FLAGS
//=================================================
static int gt, eq;
//=================================================
// MEMORY
//=================================================
static byte MEM[4000];
//=================================================
// INTERMEDIATE DATA PATH SIGNALS
//=================================================
static word instruction_word;
static word operand1;
static word operand2;

word aluResult;
word ldResult;

//=================================================
// CONTROL SIGNALS
//=================================================
static boolean isSt = 0;
static boolean isBeq = 0;
static boolean isBgt = 0;
static boolean isRet = 0;
static boolean isImmediate = 0;
static boolean isWb = 0;
static boolean isUBranch = 0;
static boolean isBranchTaken = 0;

static boolean isAdd = 0;
static boolean isSub = 0;
static boolean isCmp = 0;
static boolean isMul = 0;
static boolean isDiv = 0;
static boolean isMod = 0;
static boolean isAnd = 0;
static boolean isOr = 0;
static boolean isNot = 0;
static boolean isMov = 0;
static boolean isLd = 0;
static boolean isLsl = 0;
static boolean isLsr = 0;
static boolean isAsr = 0;
static boolean isCall = 0;
//=================================================
// OTHER WORD VALUES
//=================================================
int branch_target;
word immx;

word branchPC;

void run_simplesim() {
    while (1) {
        fetch();
        if (instruction_word == -1) break;
        decode();
        execute();
        mem();
        write_back();
        printf("PC=%d: ", PC);
        for (int i = 0; i < 14; i++) {
            if (i < 5 || R[i] != 0) printf("r%d=%d, ", i, R[i]);
        }
        printf("sp=%d, ra=%d.", R[14], R[15]);
        printf("\n");
        PC = isBranchTaken ? branchPC : PC + 4;
    }
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
    R[14] = 4000;
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
    instruction_word = read_word(MEM, PC);
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
    // IMMEDIATE AND BRANCH TARGET CALCULATION
    //=================================================
    immx = instruction_word & 0xFFFF;
    bit u = (instruction_word & 0x10000) >> 16;
    bit h = (instruction_word & 0x20000) >> 17;
    if (h) immx <<= 16;
    else if (((instruction_word & 0x1000) >> 8) && !u) immx |= 0xFFFC0000; //if negative sign extension
    branch_target = (instruction_word & 0x7FFFFFF) << 2;
    if ((branch_target & 0x10000000) >> 28) branch_target += 0x1E0000000; // if negative extending sign
    branch_target += PC;
    //=================================================
    // OPERAND CALCULATION
    //=================================================
    //TODO check ops?
    word opcode = (instruction_word & 0xF8000000) >> 27;
    isRet = opcode == 20;
    isSt = opcode == 15;
    operand1 = isRet ? R[15] : R[(instruction_word & 0x3C0000) >> 18]; //?ra:rs1
    operand2 = isSt ? R[(instruction_word & 0x3C00000) >> 22] : R[(instruction_word & 0x3C000) >> 14]; //?rd:rs2
}

//=================================================
// EXECUTE
//
//executes the ALU operation based on ALUop
//=================================================
void execute() {
    //=================================================
    // CONTROL SIGNALS
    //=================================================
    word opcode = (instruction_word & 0xF8000000) >> 27;
    //=================================================
    isAdd = opcode == 0 || opcode == 14 || opcode == 15;
    isSub = opcode == 1;
    isCmp = opcode == 5;
    isMul = opcode == 2;
    isDiv = opcode == 3;
    isMod = opcode == 4;
    isLsl = opcode == 10;
    isLsr = opcode == 11;
    isAsr = opcode == 12;
    isOr = opcode == 7;
    isAnd = opcode == 6;
    isNot = opcode == 8;
    isMov = opcode == 9;
    //=================================================
    isCall = opcode == 19;
    isLd = opcode == 14;
    isUBranch = (opcode == 18) || (opcode == 19) || (opcode == 20);
    isBgt = opcode == 17;
    isBeq = opcode == 16;
    isWb = (opcode == 0) || isSub || isMul || isDiv || isMod || isAnd || isOr || isNot || isMov || isLd || isLsl || isLsr || isAsr || isCall;
    isImmediate = ((instruction_word & 0x4000000) >> 26);
    //=================================================
    // ALU UNIT
    //=================================================
    word A = operand1;
    word B = isImmediate ? immx : operand2;
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
    //TODO see lsr vs asr
    else if (isLsr) aluResult = A >> B;
    else if (isAsr) aluResult = A >> B;

    //=================================================
    // BRANCH UNIT
    //=================================================
    branchPC = isRet ? operand1 : branch_target;
    isBranchTaken = isUBranch || (isBeq && eq) || (isBgt && gt);    //isUBranch||(isBeq&&flags.E)||(isBgt&&flags.GT)
    //??
    isCall = isCall || (opcode == 18);
    isWb = isWb || isCall;
}

//=================================================
//MEM
//
//perform the memory operation
//=================================================
void mem() {
    if (isLd) ldResult = read_word(MEM, aluResult);
    else if (isSt) write_word(MEM, aluResult, operand2);

}

//=================================================
// WRITE BACK
//
//writes the results back to register file
//=================================================
void write_back() {
    word result = isLd ? ldResult : isCall ? PC + 4 : aluResult;
    if (isWb) {
        if (isCall) R[15] = result;
        else R[(instruction_word & 0x3C00000) >> 22] = result;
    }
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
