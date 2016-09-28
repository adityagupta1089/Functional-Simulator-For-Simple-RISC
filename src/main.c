/*

The project is developed as part of Computer Architecture class
Project Name: Functional/Pipeline Simulator for SimpleRISC Processor

Developer's Name:
Developer's Email id:
Date:

*/

/* main.cpp
   Purpose of this file: The file handles the input and output, and
   invokes the simulator
*/

#include "mySimpleSim.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  char* prog_mem_file;
  if(argc < 2) {
    printf("Incorrect number of arguments. Please invoke the simulator \n\t./mySimpleSim <input mem file> \n");
    exit(1);
  }

  //reset the processor
  reset_proc();
  //load the program memory
  load_program_memory(argv[1]);
  //run the simulator
  run_simplesim();

  return 1;
}
