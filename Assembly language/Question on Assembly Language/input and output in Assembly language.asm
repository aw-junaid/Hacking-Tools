section .data
    prompt db 'Enter a number: ', 0
    result_msg db 'Result: ', 0

section .bss
    num resb 10       ; Reserve space for the input number (up to 10 digits)
    result resd 1     ; Reserve space for the result

section .text
    global _start

_start:
    ; Display prompt and read user input
    mov eax, 4         ; syscall: write
    mov ebx, 1         ; file descriptor: stdout
    mov ecx, prompt    ; message address
    mov edx, 16        ; message length
    int 0x80           ; Call kernel

    mov eax, 3         ; syscall: read
    mov ebx, 0         ; file descriptor: stdin
    mov ecx, num       ; buffer address
    mov edx, 10        ; buffer size
    int 0x80           ; Call kernel

    ; Convert the input string to an integer
    mov eax, num
    call str_to_int

    ; Multiply the input by 2
    shl eax, 1         ; Shift left by 1 (multiply by 2)

    ; Display the result message
    mov eax, 4         ; syscall: write
    mov ebx, 1         ; file descriptor: stdout
    mov ecx, result_msg ; message address
    mov edx, 8         ; message length
    int 0x80           ; Call kernel

    ; Display the result
    mov eax, 4         ; syscall: write
    mov ebx, 1         ; file descriptor: stdout
    mov ecx, result    ; result address
    mov edx, 4         ; result length
    int 0x80           ; Call kernel

    ; Exit the program
    mov eax, 1         ; syscall: exit
    xor ebx, ebx       ; status: 0
    int 0x80           ; Call kernel

str_to_int:
    ; Convert a string to an integer (same subroutine as in the previous example)
    ; ...
    ret


