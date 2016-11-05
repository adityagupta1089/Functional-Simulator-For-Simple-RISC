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

static bool fetch_bubble = false;
static bool fetch_bubble_in = false;
static bool is_pipelined = false;
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
static int global_PC;

IF_OF if_of;
OF_EX of_ex;
EX_MA ex_ma;
MA_RW ma_rw;
//=================================================
// EX -> IF
//=================================================
word ex_if_branchPC;
bool ex_if_isBranchTaken;

void run_simplesim(char* pipeline) {
    is_pipelined = *pipeline == '1';
    int cycle = 1;
    bool pipeline_empty = true;
    while (1) {
        if ((cycle - 1) % 10 == 0) header();
        printf("|  %4d ", cycle++);
        // these are still running serially, but we can assume them to be
        // running in parallel for the cycle
        pipeline_empty = !(fetch() + decode() + execute() + mem() + write_back());
        info();
        if (is_pipelined) update_pipeline();
        else update_non_pipeline();
        if (pipeline_empty || cycle > 1000) break;
    }
    write_data_memory();
    exit_info(cycle);
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
    global_PC = 0;
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

void header() {
    //---
    printf("+=======+======+======+======+======+======");
    for (int i = 0; i < 14; i++)
        printf("+======");
    printf("+=======+======+\n");
    //---
    printf("|  CY   |  IF  |  OF  |  EX  |  MA  |  RW  ");
    //---
    for (int i = 0; i < 14; i++)
        printf("|  r%-2d ", i);
    printf("|  sp   |  ra  |\n");
    //---
    printf("+=======+======+======+======+======+======");
    for (int i = 0; i < 14; i++)
        printf("+======");
    printf("+=======+======+\n");
    //---
}
//=================================================
// FETCH
//
// reads from the instruction memory and updates
// the instruction register
//=================================================
bool fetch() {
    word instruction = read_word(MEM, global_PC);
    if (instruction == -1 || fetch_bubble_in) {
        fetch_bubble = true; //for non pipeline
        printf("| ---- ");
        if_of.push_bubble();
        return false;
        fetch_bubble_in = false;
    }
    fetch_bubble = false;
    if_of.update(global_PC, instruction);

    printf("| I%-3d ", global_PC / 4);
    return true;
}

//=================================================
// DECODE
//
// reads the instruction register, reads operand1,
// operand2 from register file, decides the
// operation to be performed in execute stage
//=================================================
bool decode() {
    //=================================================
    // CONTROL SIGNALS
    //=================================================
    if (if_of.hasBubble()) {
        printf("| ---- ");
        of_ex.push_bubble();
        return false;
    }
    word instruction = if_of.getInstruction();
    int PC = if_of.getPc();
    word opcode = (instruction & 0xF8000000) >> 27;
    Control control;
    bit I = ((instruction & 0x4000000) >> 26);
    control.update(opcode, I);

    //=================================================
    // IMMEDIATE AND BRANCH TARGET CALCULATION
    //=================================================
    word immx = instruction & 0xFFFF;
    bit u = (instruction & 0x10000) >> 16;
    bit h = (instruction & 0x20000) >> 17;
    if (h) immx <<= 16;
    else if (((instruction & 0x8000) >> 8) && !u) immx |= 0xFFFC0000; //if negative sign extension
    int branch_target = (instruction & 0x7FFFFFF) << 2;
    if ((branch_target & 0x10000000) >> 28) branch_target += 0xE0000000; // if negative extending sign
    branch_target += PC;
    //=================================================
    // OPERAND CALCULATION
    //=================================================
    word operand1 = control.isRet ? R[15] : R[(instruction & 0x3C0000) >> 18]; //?ra:rs1
    word operand2 =
            control.isSt ?
                    R[(instruction & 0x3C00000) >> 22] : R[(instruction & 0x3C000) >> 14]; //?rd:rs2

    word A = operand1;
    word B = control.isImmediate ? immx : operand2;
    of_ex.update(PC, branch_target, A, B, operand2, instruction, control);

    printf("| I%-3d ", PC / 4);
    return true;
}

//=================================================
// EXECUTE
//
//executes the ALU operation based on ALUop
//=================================================
bool execute() {
    if (of_ex.hasBubble()) {
        printf("| ---- ");
        ex_ma.push_bubble();
        if (is_pipelined) ex_if_isBranchTaken = false;
        return false;
    }
    word instruction = of_ex.getInstruction();
    int PC = of_ex.getPc();
    word A = of_ex.getA();
    word B = of_ex.getB();
    word operand2 = of_ex.getOperand2();
    int branch_target = of_ex.getBranchTarget();
    Control control = of_ex.getControl();

    //=================================================
    // ALU UNIT
    //=================================================

    word aluResult;

    if (control.isAdd) aluResult = A + B;
    else if (control.isSub) aluResult = A - B;
    else if (control.isMul) aluResult = A * B;
    else if (control.isDiv) aluResult = A / B;
    else if (control.isMod) aluResult = A % B;
    else if (control.isCmp) {
        if (A > B) gt = 1;
        else gt = 0;
        if (A == B) eq = 1;
        else eq = 0;
    } else if (control.isAnd) aluResult = A & B;
    else if (control.isOr) aluResult = A | B;
    else if (control.isNot) aluResult = !B;
    else if (control.isMov) aluResult = B;
    else if (control.isLsl) aluResult = A << B;
    else if (control.isLsr) aluResult = A >> B;
    else if (control.isAsr) aluResult = (word) (((signed int) A) >> B);
    //=================================================
    // BRANCH UNIT
    //=================================================
    word branchPC = control.isRet ? A : branch_target;
    control.isBranchTaken = control.isUBranch || (control.isBeq && eq)
            || (control.isBgt && gt); //isUBranch||(isBeq&&flags.E)||(isBgt&&flags.GT)

    ex_if_isBranchTaken = control.isBranchTaken;
    ex_if_branchPC = branchPC;

    ex_ma.update(PC, branchPC, aluResult, operand2, instruction, control);

    printf("| I%-3d ", PC / 4);
    return true;
}

//=================================================
//MEM
//
//perform the memory operation
//=================================================
bool mem() {
    if (ex_ma.hasBubble()) {
        printf("| ---- ");
        ma_rw.push_bubble();
        return false;
    }
    word instruction = ex_ma.getInstruction();
    int PC = ex_ma.getPc();
    word operand2 = ex_ma.getOperand2();
    word aluResult = ex_ma.getAluResult();
    int branch_PC = ex_ma.getbranchPC();
    Control control = ex_ma.getControl();

    word ldResult;
    if (control.isLd) ldResult = read_word(MEM, aluResult);
    else if (control.isSt) write_word(MEM, aluResult, operand2);

    ma_rw.update(PC, branch_PC, ldResult, aluResult, instruction, control);

    printf("| I%-3d ", PC / 4);
    return true;
}

//=================================================
// WRITE BACK
//
//writes the results back to register file
//=================================================
bool write_back() {
    if (ma_rw.hasBubble()) {
        printf("| ---- ");
        return false;
    }
    word instruction = ma_rw.getInstruction();
    int PC = ma_rw.getPc();
    word ldResult = ma_rw.getLdResult();
    word aluResult = ma_rw.getAluResult();
    int branch_PC = ma_rw.getbranchPC();
    Control control = ma_rw.getControl();

    word result = control.isLd ? ldResult : control.isCall ? PC + 4 : aluResult;
    if (control.isWb) {
        if (control.isCall) R[15] = result;
        else R[(instruction & 0x3C00000) >> 22] = result;
    }

    printf("| I%-3d ", PC / 4);
    return true;

}
//=================================================

void update_pipeline() {
    bool data_conflict = !if_of.hasBubble()
            && ((!of_ex.hasBubble()
                    && data_lock_conflict(if_of.getInstruction(), of_ex.getInstruction()))
                    || (!ex_ma.hasBubble()
                            && data_lock_conflict(if_of.getInstruction(),
                                    ex_ma.getInstruction()))
                    || (!ma_rw.hasBubble()
                            && data_lock_conflict(if_of.getInstruction(),
                                    ma_rw.getInstruction())));
    bool branch_conflict = !of_ex.hasBubble()
            && branch_lock_condition(of_ex.getInstruction());
    if (data_conflict || branch_conflict) of_ex.push_bubble();
    if (branch_conflict) if_of.push_bubble();
    if (!data_conflict || branch_conflict) {
        global_PC = ex_if_isBranchTaken ? ex_if_branchPC : global_PC + 4;
        if_of.tick();
    }
    //printf("BL(%d), DL(%d)", branch_conflict, data_conflict);
    of_ex.tick();
    ex_ma.tick();
    ma_rw.tick();
}

bool data_lock_conflict(word A, word B) {
    int opcodeA = (A & 0xF8000000) >> 27;
    int opcodeB = (B & 0xF8000000) >> 27;
    if (opcodeA == OPCODE_NOP || opcodeA == OPCODE_B || opcodeA == OPCODE_BEQ
            || opcodeA == OPCODE_BGT || opcodeA == OPCODE_CALL) return false;
    if (opcodeB == OPCODE_NOP || opcodeB == OPCODE_CMP || opcodeB == OPCODE_ST
            || opcodeB == OPCODE_B || opcodeB == OPCODE_BEQ || opcodeB == OPCODE_BGT
            || opcodeB == OPCODE_RET) return false;
    int src1 = (opcodeA == OPCODE_RET) ? 15 : (A & 0x3C0000) >> 18;    //?ra:rs1
    int src2 = (opcodeA == OPCODE_ST) ? (A & 0x3C00000) >> 22 : (A & 0x3C000) >> 14; //?rd:rs2
    int dest = (opcodeB == OPCODE_CALL) ? 15 : (B & 0x3C00000) >> 22;   //?ra:rd
    return (opcodeA != OPCODE_MOV && opcodeA != OPCODE_NOT && src1 == dest)
            || (!(opcodeA != OPCODE_ST && ((A & 0x4000000) >> 26)) && src2 == dest);
}

bool branch_lock_condition(word A) {
    int opcodeA = (A & 0xF8000000) >> 27;
    bool isUBranch = (opcodeA == OPCODE_B) || (opcodeA == OPCODE_CALL)
            || (opcodeA == OPCODE_RET);
    bool isBgt = opcodeA == OPCODE_BGT;
    bool isBeq = opcodeA == OPCODE_BEQ;
    return isUBranch || (isBeq && eq) || (isBgt && gt);
}
//=================================================
void update_non_pipeline() {
    if_of.tick();
    of_ex.tick();
    ex_ma.tick();
    ma_rw.tick();
    if (fetch_bubble && if_of.hasBubble() && of_ex.hasBubble() && ex_ma.hasBubble()
            && ma_rw.hasBubble()) {
        global_PC = ex_if_isBranchTaken ? ex_if_branchPC : global_PC + 4;
        fetch_bubble_in = false;
    } else fetch_bubble_in = true;
}

//=================================================
void info() {
    for (int i = 0; i < 14; i++)
        printf("|  %-3d ", R[i]);
    printf("|  %-4d |  %-3d |\n", R[14], R[15]);
}
void exit_info(int cycles) {
    printf("+=======+======+======+======+======+======");
    for (int i = 0; i < 14; i++)
        printf("+======");
    printf("+=======+======+\n");
    printf("Exited. Took %d cycles with %spipelining.\n", cycles - 2,
            is_pipelined ? "" : "no ");
}
//=================================================
word read_word(byte* mem, word address) {
    word* data;
    data = (word*) (mem + address);
    return *data; //basically return MEM[address]
}

//=================================================
void write_word(byte* mem, word address, word data) {
    word* data_p;
    data_p = (word*) (mem + address);
    *data_p = data; //basically MEM[address]=data
}
//=================================================
