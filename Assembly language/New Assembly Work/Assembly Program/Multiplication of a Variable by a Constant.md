Multiplying a variable by a constant in assembly is straightforward and efficient. It can be done either using the `MUL`/`IMUL` instruction or by leveraging bit shifts when the constant is a power of two.

### General Case (Multiplication by Any Constant)
If the constant is not a power of two, you simply load the variable into a register and multiply it by the constant using `MUL` or `IMUL` (for signed integers). 

### Example: Multiplication of a Variable by a Constant

Let’s say you want to multiply a variable by 7 (which is not a power of two):

```assembly
section .data
    var     dd  5              ; variable to be multiplied

section .text
    global _start

_start:
    mov     eax, [var]          ; load var into eax
    imul    eax, eax, 7         ; multiply eax by 7
    ; Result is now in eax

    ; exit system call
    mov     eax, 60             ; exit system call number
    xor     edi, edi            ; exit code 0
    syscall
```

### Breakdown:
- **`imul eax, eax, 7`**: This multiplies the value in `eax` by 7 and stores the result back in `eax`.
  - The first `eax` is the destination register.
  - The second `eax` is the source operand (the variable).
  - `7` is the constant multiplier.
  
### Multiplication by a Power of Two (Efficient Shifting)

When multiplying by powers of two (e.g., \(2^n\)), you can use bit shifting, which is faster than general multiplication. For example, multiplying by 8 is the same as shifting left by 3 bits:

```assembly
section .data
    var     dd  5              ; variable to be multiplied

section .text
    global _start

_start:
    mov     eax, [var]          ; load var into eax
    shl     eax, 3              ; multiply eax by 8 (shift left by 3 bits)
    ; Result is now in eax

    ; exit system call
    mov     eax, 60             ; exit system call number
    xor     edi, edi            ; exit code 0
    syscall
```

### Key Points:
- **For general constants**: Use `IMUL` or `MUL`. In this case, `imul eax, eax, constant` multiplies the variable in `eax` by the constant.
- **For powers of two**: Use `SHL` to shift the bits left. Shifting left by `n` bits multiplies by \(2^n\).

### Efficiency
- **`IMUL`** is versatile but slower than shifts.
- **`SHL`** is much faster when multiplying by powers of two.
