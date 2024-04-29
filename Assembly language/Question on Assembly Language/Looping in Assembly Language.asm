section .data
    count db 10  ; Initialize a variable 'count' with value 10

section .text
    global _start   ; Entry point for the program

_start:
    mov ecx, count  ; Load the value of 'count' into the ECX register
    jmp loop_start  ; Jump to the loop_start label

loop_start:
    ; Your loop instructions go here
    ; For example, print a message
    ; For simplicity, let's assume it prints a message and decrements count

    ; Print a message
    mov eax, 4      ; System call for 'write' (syscall number)
    mov ebx, 1      ; File descriptor (stdout)
    mov edx, 13     ; Message length
    mov ecx, message  ; Address of the message
    int 0x80        ; Call kernel to perform write syscall

    ; Decrement count
    dec byte [count]  ; Decrement the value stored in 'count'

    ; Check if count is zero
    cmp byte [count], 0  ; Compare 'count' with zero
    jne loop_start        ; If not zero, jump back to loop_start

    ; Exit the program
    mov eax, 1      ; System call for 'exit' (syscall number)
    xor ebx, ebx    ; Exit code 0
    int 0x80        ; Call kernel to perform exit syscall

section .data
    message db 'Hello, World!', 0xA  ; Message to be printed, followed by newline (0xA)
