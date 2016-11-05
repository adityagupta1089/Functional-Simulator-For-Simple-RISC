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
// mySimpleSim.h
//
// Purpose of this file: header file for mySimpleSim
//=================================================
#include <stdint.h>

//=================================================
// TYPEDEFS
//=================================================
typedef uint32_t word;
typedef int bit; //bit not supported in c++
typedef char byte;
//=================================================
// TYPEDEFS
//=================================================
void run_simplesim(char* pipeline);
void reset_proc();
void load_program_memory(char* file_name);
void write_data_memory();
//=================================================
// INSTRUCTION STAGES
//=================================================

void header();

//reads from the instruction memory and updates the instruction register
bool fetch();
//reads the instruction register, reads operand1, operand2 from register file, decides the operation to be performed in execute stage
bool decode();
//executes the ALU operation based on ALUop
bool execute();
//perform the memory operation
bool mem();
//writes the results back to register file
bool write_back();

void update_pipeline();
void update_non_pipeline();
void info();
void exit_info(int cycles);

bool data_lock_conflict(word A, word B);
bool branch_lock_condition(word A);
//=================================================
// MEMORY READ WRITE
//=================================================
word read_word(byte* mem, word address);
void write_word(byte* mem, word address, word data);
//=================================================
