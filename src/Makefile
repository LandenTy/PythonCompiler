# Compiler and flags
CC = gcc
CFLAGS = -Wall -g
OBJ = quantum.o assembly.o parser.o utils.o

# Default output name and directory
OUTPUT_NAME = quantum.exe
OUTPUT_DIR = ..

# Check for output flag
ifneq ($(OUTPUT),)
    OUTPUT_NAME = $(OUTPUT)
endif

# Default target
all: $(OUTPUT_DIR)/$(OUTPUT_NAME)

# Build the executable
$(OUTPUT_DIR)/$(OUTPUT_NAME): $(OBJ)
	$(CC) -o $@ $^

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up generated .o and .asm files
clean:
	del /Q $(OBJ) *.asm

.PHONY: all clean