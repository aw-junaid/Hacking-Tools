section .data
    num1 dd 5
    num2 dd 8
    message1 db 'Numbers are equal.', 0
    message2 db 'Number 1 is greater.', 0
    message3 db 'Number 2 is greater.', 0

section .text
    global _start

_start:
    ; Compare num1 and num2
    mov eax, [num1]
    cmp eax, [num2]

    ; Jump to appropriate label based on the comparison result
    je equal_numbers
    jg num1_greater
    jl num2_greater

    ; Exit the program if none of the conditions are met
    jmp exit_program

equal_numbers:
    ; Print message for equal numbers
    mov eax, 4         ; syscall: write
    mov ebx, 1         ; file descriptor: stdout
    mov ecx, message1  ; message address
    mov edx, 18        ; message length
    int 0x80           ; Call kernel
    jmp exit_program   ; Jump to exit after printing the message

num1_greater:
    ; Print message for num1 being greater
    mov eax, 4         ; syscall: write
    mov ebx, 1         ; file descriptor: stdout
    mov ecx, message2  ; message address
    mov edx, 21        ; message length
    int 0x80           ; Call kernel
    jmp exit_program   ; Jump to exit after printing the message

num2_greater:
    ; Print message for num2 being greater
    mov eax, 4         ; syscall: write
    mov ebx, 1         ; file descriptor: stdout
    mov ecx, message3  ; message address
    mov edx, 22        ; message length
    int 0x80           ; Call kernel

exit_program:
    ; Exit the program
    mov eax, 1         ; syscall: exit
    xor ebx, ebx       ; status: 0
    int 0x80           ; Call kernel
