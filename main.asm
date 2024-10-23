; -- header --
bits 64
default rel
; -- variables --
section .bss
; -- constants --
section .data
string_literal_0 db "Hello, Universe!", 0x0A, 0
string_literal_1 db "Hello World!", 0x0A, 0
string_literal_2 db "1+1=2", 0x0A, 0
; -- Entry Point --
section .text
global main
extern ExitProcess
extern printf
extern scanf

main:
    PUSH rbp
    MOV rbp, rsp
    SUB rsp, 32
; -- PRINT ---
    LEA rcx, string_literal_0
    XOR eax, eax
    CALL printf
; -- PRINT ---
    LEA rcx, string_literal_1
    XOR eax, eax
    CALL printf
; -- PRINT ---
    LEA rcx, string_literal_2
    XOR eax, eax
    CALL printf
; -- HALT ---
    jmp EXIT_LABEL
EXIT_LABEL:
    XOR rax, rax
    CALL ExitProcess
