section .data
    newline db 10     ; ASCII code for newline character

section .text
    global _start

_start:
    mov ecx, 1         ; Initialize counter to 1

print_loop:
    ; Print the current number
    mov eax, 4         ; syscall: write
    mov ebx, 1         ; file descriptor: stdout
    mov edx, 1         ; message length
    lea ecx, [ecx + '0'] ; Convert counter to ASCII character
    mov [esp - 4], ecx ; Store the ASCII character on the stack
    mov ecx, esp - 4   ; Set the pointer to the ASCII character
    int 0x80           ; Call kernel

    ; Print a newline character
    mov eax, 4         ; syscall: write
    mov ebx, 1         ; file descriptor: stdout
    mov edx, 1         ; message length
    mov ecx, newline   ; newline character
    int 0x80           ; Call kernel

    ; Increment counter
    inc dword [esp - 4]

    ; Check if the counter is greater than 10
    cmp dword [esp - 4], 11
    jne print_loop     ; Jump back to the loop if not greater

    ; Exit the program
    mov eax, 1         ; syscall: exit
    xor ebx, ebx       ; status: 0
    int 0x80           ; Call kernel
