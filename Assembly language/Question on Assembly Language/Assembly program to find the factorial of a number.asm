section .data
    prompt db 'Enter a number: ', 0
    result_msg db 'Factorial: ', 0

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

    ; Calculate the factorial
    mov ebx, eax       ; Copy the number to ebx (initial value for factorial)
    mov eax, 1         ; Initialize result to 1

calculate_factorial:
    imul eax, ebx       ; Multiply result by ebx
    dec ebx             ; Decrement ebx (decrementing the number for factorial)
    cmp ebx, 1          ; Compare with 1
    jg calculate_factorial  ; Jump if greater than 1

    ; Display the result message
    mov eax, 4         ; syscall: write
    mov ebx, 1         ; file descriptor: stdout
    mov ecx, result_msg ; message address
    mov edx, 11        ; message length
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
    ; Convert a string to an integer
    xor eax, eax       ; Clear eax to store the result
    xor ecx, ecx       ; Clear ecx to use it as a counter/multiplier

convert_loop:
    movzx edx, byte [eax]  ; Load the next byte (character) from the string into edx
    cmp edx, 10             ; Check if it's the null terminator
    je  convert_done        ; If it is, we are done

    sub edx, '0'            ; Convert ASCII character to integer ('0' -> 0, '1' -> 1, ..., '9' -> 9)
    imul eax, ecx, 10       ; Multiply the current result by 10
    add eax, edx            ; Add the new digit to the result
    inc ecx                 ; Increment the counter/multiplier
    inc eax                 ; Move to the next character in the string
    jmp convert_loop        ; Repeat the process for the next character

convert_done:
    ret
