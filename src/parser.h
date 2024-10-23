#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>

bool is_print_statement(const char* line);
void extract_print_content(const char* line, char* buffer);

#endif // PARSER_H
