Multiplying by a power of two in assembly language is quite efficient because you can simply use bit shifts instead of the traditional multiplication instruction. Shifting bits to the left effectively multiplies a number by powers of two. This is because each left shift operation moves all bits one position to the left, doubling the value.

Here’s an overview of how it works:

### Left Shift for Multiplication
- Shifting a number to the left by `n` bits (using `SHL` in x86 assembly, for example) multiplies it by \(2^n\).
- For example:
  - $\(5 \times 2 = 5 \ll 1\)$ (shift left by 1)
  - $\(5 \times 4 = 5 \ll 2\)$ (shift left by 2)
  - $\(5 \times 8 = 5 \ll 3\)$ (shift left by 3)

### Example in Assembly (x86)
If you want to multiply a number by 4 (which is \(2^2\)):

```assembly
section .data
    num     dd      5               ; our number to multiply

section .text
    global _start

_start:
    mov     eax, [num]              ; load num into eax register
    shl     eax, 2                  ; shift left by 2 bits (multiply by 4)
    ; Now eax holds 5 * 4 = 20

    ; exit system call
    mov     eax, 60                 ; exit system call number
    xor     edi, edi                ; exit code 0
    syscall
```

### Key Points:
- **`SHL` (Shift Left)**: This is the instruction for shifting bits to the left, multiplying by a power of two.
- **Efficiency**: Bit shifts are faster than using a multiplication instruction for powers of two.
