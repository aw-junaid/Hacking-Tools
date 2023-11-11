section .data
    var1 dd 10      ; First variable
    var2 dd 20      ; Second variable
    temp dd 0       ; Temporary variable

section .text
    global _start

_start:
    ; Display the original values
    mov eax, [var1]
    call print_number

    mov eax, [var2]
    call print_number

    ; Swap the values
    mov eax, [var1]
    mov ebx, [var2]
    mov [temp], eax   ; Store var1 in temp
    mov eax, ebx      ; Move var2 to var1
    mov [var1], eax
    mov eax, [temp]   ; Move temp (original var1) to var2
    mov [var2], eax

    ; Display the values after swapping
    mov eax, [var1]
    call print_number

    mov eax, [var2]
    call print_number

    ; Exit the program
    mov eax, 1         ; syscall: exit
    xor ebx, ebx       ; status: 0
    int 0x80           ; Call kernel

print_number:
    ; Print a number to the standard output
    mov eax, 4         ; syscall: write
    mov ebx, 1         ; file descriptor: stdout
    mov ecx, eax       ; number to print
    mov edx, 1         ; message length
    add ecx, '0'       ; Convert the number to ASCII
    int 0x80           ; Call kernel
    ret
