Shifting left by an immediate value is a common operation in ARM assembly that allows you to multiply a number by powers of two. When you shift a binary number left by `n` bits, it effectively multiplies that number by \(2^n\). 

In ARM assembly, the shift left operation can be performed using the `LSL` (Logical Shift Left) instruction. You can specify an immediate value to indicate how many positions to shift.

### Example: Shift Left by Immediate Value

Here's an example that demonstrates how to use the `LSL` instruction to shift a register value left by an immediate value.

#### ARM Assembly Code

```assembly
.section .data
    value: .word 0b00000000000000000000000000000010 ; Initial value (2 in decimal)
    result: .space 4                              ; Space to store the result of the shift operation

.section .text
.global _start

_start:
    // Load the value from memory
    LDR     r0, =value                          ; Load address of value into r0
    LDR     r1, [r0]                            ; Load the value into r1

    // Shift left by immediate value (shift left by 3 bits)
    LSL     r2, r1, #3                          ; r2 = r1 << 3 (multiply by 8)

    // Store the result
    STR     r2, =result                         ; Store the result of the shift

    // Exit the program
    MOV     r7, #1                              ; syscall number for exit
    SWI     0                                   ; software interrupt to exit

.section .bss
```

### Explanation of the Example:

1. **Data Section**:
   - **`value`**: A sample 32-bit value (2 in decimal, represented in binary as `0b00000000000000000000000000000010`).
   - **`result`**: Reserved space to store the result of the shift operation.

2. **Loading the Value**:
   - **`LDR r0, =value`**: Loads the address of `value` into register `r0`.
   - **`LDR r1, [r0]`**: Loads the value from memory into register `r1`.

3. **Shift Left Operation**:
   - **`LSL r2, r1, #3`**: Performs a logical shift left on `r1` by 3 bits. The result is stored in `r2`. This operation effectively multiplies the value in `r1` by \(2^3\) (or 8). For instance, if the initial value is 2, shifting left by 3 will give `2 * 8 = 16` (represented as `0b00000000000000000000000000010000`).

4. **Storing the Result**:
   - **`STR r2, =result`**: Stores the result of the shift operation into the `result` space.

5. **Exiting the Program**:
   - The program ends with a syscall to exit.

### Summary

- **Logical Shift Left**: The `LSL` instruction shifts bits to the left by a specified immediate value, effectively multiplying the original value by \(2^n\).
- **Immediate Value**: You can specify the number of bits to shift as an immediate value in the instruction, making it efficient for multiplication by powers of two.
- **Result Storage**: The result of the shift operation is typically stored for later use.
