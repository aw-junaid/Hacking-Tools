section .data
    num1 dd 10        ; Define the first number as a double-word (4 bytes)
    num2 dd 20        ; Define the second number as a double-word (4 bytes)
    result dd 0       ; Define a variable to store the result

section .text
    global _start

_start:
    ; Load the first number into a register
    mov eax, [num1]

    ; Add the second number to the first number
    add eax, [num2]

    ; Store the result in the variable
    mov [result], eax

    ; Your program can continue here...

    ; Exit the program
    mov eax, 1         ; syscall: exit
    xor ebx, ebx       ; status: 0
    int 0x80           ; Call kernel

section .bss
    ; You can add a section for uninitialized data (if needed)
