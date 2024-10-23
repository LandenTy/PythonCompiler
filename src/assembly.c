#include "assembly.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

// Function to output NASM-style assembly to a file
void output_assembly(FILE* input_file, FILE* output_file) {
    char line[256];
    int print_count = 0;

    // Header
    fprintf(output_file, "; -- header --\n");
    fprintf(output_file, "bits 64\n");
    fprintf(output_file, "default rel\n");

    // Variables (bss section)
    fprintf(output_file, "; -- variables --\n");
    fprintf(output_file, "section .bss\n");

    // Constants (data section)
    fprintf(output_file, "; -- constants --\n");
    fprintf(output_file, "section .data\n");

    // Read each line and process print statements
    while (fgets(line, sizeof(line), input_file)) {
        // Trim leading whitespace
        char* trimmed_line = line;
        while (isspace(*trimmed_line)) trimmed_line++;

        // Check if it's a print statement
        if (is_print_statement(trimmed_line)) {
            char print_content[256];
            extract_print_content(trimmed_line, print_content);

            // Output the string literal in the .data section
            fprintf(output_file, "string_literal_%d db %s, 0x0A, 0\n", print_count, print_content);

            // Increment count for unique string labels
            print_count++;
        }
    }

    // Text section
    fprintf(output_file, "; -- Entry Point --\n");
    fprintf(output_file, "section .text\n");
    fprintf(output_file, "global main\n");
    fprintf(output_file, "extern ExitProcess\n");
    fprintf(output_file, "extern printf\n");
    fprintf(output_file, "extern scanf\n\n");

    fprintf(output_file, "main:\n");
    fprintf(output_file, "    PUSH rbp\n");
    fprintf(output_file, "    MOV rbp, rsp\n");
    fprintf(output_file, "    SUB rsp, 32\n");

    // Output print statement handling in the main function
    for (int i = 0; i < print_count; i++) {
        fprintf(output_file, "; -- PRINT ---\n");
        fprintf(output_file, "    LEA rcx, string_literal_%d\n", i);
        fprintf(output_file, "    XOR eax, eax\n");
        fprintf(output_file, "    CALL printf\n");
    }

    // Exit handling
    fprintf(output_file, "; -- HALT ---\n");
    fprintf(output_file, "    jmp EXIT_LABEL\n");
    fprintf(output_file, "EXIT_LABEL:\n");
    fprintf(output_file, "    XOR rax, rax\n");
    fprintf(output_file, "    CALL ExitProcess\n");
}
