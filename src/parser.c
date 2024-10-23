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

        // Replace escape sequences
        char* pos;
        // Handle newline
        while ((pos = strstr(buffer, "\\n")) != NULL) {
            *pos = '\0'; // Cut the string at the newline
            strcat(buffer, "0x0A"); // Append the newline character in assembly
            strcat(buffer, pos + 2); // Append the rest of the string
        }

        // Handle tab
        while ((pos = strstr(buffer, "\\t")) != NULL) {
            *pos = '\0'; // Cut the string at the tab
            strcat(buffer, "0x09"); // Append the tab character in assembly
            strcat(buffer, pos + 2); // Append the rest of the string
        }
    } else {
        strcpy(buffer, "Malformed print statement");
    }
}
