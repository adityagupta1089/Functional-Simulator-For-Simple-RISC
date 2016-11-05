//=================================================
//
// The project is developed as part of Computer Architecture class
// Project Name: Functional/Pipeline Simulator for SimpleRISC Processor
//
// Developer's Name:     Aditya Gupta
// Developer's Email id: 2015csb1003[at]iitrpr[dot]ac[dot]in
//
// Developer's Name:     Vishal Singh
// Developer's Email id: 2015csb1040[at]iitrpr[dot]ac[dot]in
//=================================================

//=================================================
// main.cpp
//
// Purpose of this file: The file handles the input and output, and
// invokes the simulator
//=================================================
#include "../include/mySimpleSim.h"
#include <stdio.h>
#include <stdlib.h>
//=================================================
int main(int argc, char** argv) {
    //=================================================
    byte* prog_mem_file;
    if (argc < 3) {
        printf("Incorrect number of arguments. Please invoke the simulator \n\t../mySimpleSim <input mem file> <pipeline: 0/1>\n");
        exit(1);
    }
    //=================================================
    //reset the processor
    reset_proc();
    //load the program memory
    load_program_memory(argv[1]);
    //run the simulator
    run_simplesim(argv[2]);
    //=================================================
    return 1;
}
//=================================================
