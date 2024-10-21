#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Function to check if a string starts with "print"
int is_print_statement(const char* line) {
    return (strncmp(line, "print", 5) == 0) && (isspace(line[5]) || line[5] == '(');
}

// Function to extract the content of the print statement
void extract_print_content(const char* line, char* buffer) {
    const char* start = strchr(line, '(');
    const char* end = strrchr(line, ')');

    if (start && end && start < end) {
        strncpy(buffer, start + 1, end - start - 1);
        buffer[end - start - 1] = '\0';  // Null-terminate the string
    } else {
        strcpy(buffer, "Malformed print statement");
    }
}

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
    fprintf(output_file, "; -- contants --\n");
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
            fprintf(output_file, "string_literal_%d db %s, 0\n", print_count, print_content);

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

    // Assemble the .asm file into an object file
    char nasm_command[512];
    snprintf(nasm_command, sizeof(nasm_command), "nasm -f elf64 %s -o %s.o", asm_file, output_name);
    int result = system(nasm_command);
    if (result != 0) {
        fprintf(stderr, "Error: NASM assembly failed.\n");
        return 1;
    }

    // Compile the object file into an executable
    char gcc_command[512];
    snprintf(gcc_command, sizeof(gcc_command), "gcc -o %s.exe %s.o", output_name, output_name);
    result = system(gcc_command);
    if (result != 0) {
        fprintf(stderr, "Error: GCC compilation failed.\n");
        return 1;
    }

    // Execute the compiled program
    printf("Running %s.exe...\n", output_name);
    char run_command[512];
    snprintf(run_command, sizeof(run_command), "cls && %s.exe", output_name);
    result = system(run_command);

    return 0;
}
