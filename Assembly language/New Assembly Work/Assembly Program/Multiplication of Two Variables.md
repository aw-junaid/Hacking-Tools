In assembly language, multiplying two variables is done using the `MUL` or `IMUL` instructions, depending on whether you are working with unsigned or signed integers. Let’s break this down for x86 assembly, where multiplication works with registers.

### Steps for Multiplying Two Variables:

1. Load one variable into a register (commonly `eax`).
2. Use the `MUL` or `IMUL` instruction to multiply by the second variable.
3. Store the result.

### Unsigned Multiplication (`MUL`)
- **`MUL`** is used for multiplying unsigned integers.
- It multiplies the value in `eax` by a source operand.
- The result is stored in `eax` and `edx` (since the result may need more than 32 bits).

### Signed Multiplication (`IMUL`)
- **`IMUL`** is used for signed integer multiplication.
- Similar to `MUL`, but it preserves the sign for negative numbers.

### Example: Multiplication of Two Variables (Unsigned)

```assembly
section .data
    var1    dd  5               ; first variable
    var2    dd  3               ; second variable

section .text
    global _start

_start:
    mov     eax, [var1]          ; load var1 into eax
    mul     dword [var2]         ; multiply eax by var2 (unsigned multiplication)
    ; Result is now in eax (low 32 bits) and edx (high 32 bits)
    
    ; exit system call
    mov     eax, 60              ; exit system call number
    xor     edi, edi             ; exit code 0
    syscall
```

### Example: Multiplication of Two Variables (Signed)

```assembly
section .data
    var1    dd  -5              ; first variable (negative)
    var2    dd  3               ; second variable

section .text
    global _start

_start:
    mov     eax, [var1]          ; load var1 into eax
    imul    dword [var2]         ; multiply eax by var2 (signed multiplication)
    ; Result is now in eax (low 32 bits) and edx (high 32 bits)

    ; exit system call
    mov     eax, 60              ; exit system call number
    xor     edi, edi             ; exit code 0
    syscall
```

### Key Points:
- **Unsigned multiplication**: Use `MUL` when dealing with non-negative numbers.
- **Signed multiplication**: Use `IMUL` when working with signed numbers, to handle negative values correctly.
- **Overflow**: The result of the multiplication is split between `eax` (low part) and `edx` (high part). For 32-bit numbers, if the result fits into `eax`, the content of `edx` will be zero.
