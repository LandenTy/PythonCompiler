#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assembly.h"
#include "utils.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <python_file> [-o output_name]\n", argv[0]);
        return 1;
    }

    char* python_file = argv[1];
    char* output_name = "main";  // Default output name

    // Parse command-line arguments for the -o flag
    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            output_name = argv[i + 1];  // Set the output name to the user-specified name
            i++;  // Skip the next argument since it's the output name
        }
    }

    FILE* input_file = fopen(python_file, "r");
    if (!input_file) {
        perror("Error opening Python file");
        return 1;
    }

    // Create the assembly file name with the custom output name
    char asm_file[256];
    snprintf(asm_file, sizeof(asm_file), "%s.asm", output_name);

    // Open the output assembly file
    FILE* output_file = fopen(asm_file, "w");
    if (!output_file) {
        perror("Error creating assembly file");
        fclose(input_file);
        return 1;
    }

    // Generate the assembly code and write it to the file
    output_assembly(input_file, output_file);

    // Close the files
    fclose(input_file);
    fclose(output_file);

    printf("Assembly code has been written to %s\n", asm_file);

    // Execute the commands for assembling and running the program
    execute_commands(asm_file, output_name);

    return 0;
}
