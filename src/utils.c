#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

void execute_commands(const char* asm_file, const char* output_name) {
    // Assemble the .asm file into an object file
    char nasm_command[512];
    snprintf(nasm_command, sizeof(nasm_command), "nasm -f elf64 %s -o %s.o", asm_file, output_name);
    int result = system(nasm_command);
    if (result != 0) {
        fprintf(stderr, "Error: NASM assembly failed.\n");
        exit(1);
    }

    // Compile the object file into an executable
    char gcc_command[512];
    snprintf(gcc_command, sizeof(gcc_command), "gcc -o %s.exe %s.o", output_name, output_name);
    result = system(gcc_command);
    if (result != 0) {
        fprintf(stderr, "Error: GCC compilation failed.\n");
        exit(1);
    }

    // Execute the compiled program
    printf("Running %s.exe...\n", output_name);
    char run_command[512];
    snprintf(run_command, sizeof(run_command), "cls && %s.exe", output_name);
    system(run_command);
}
