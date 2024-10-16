Shifting left or right by a variable in ARM assembly allows you to dynamically control the number of bits to shift based on the contents of a register, rather than using a fixed immediate value. This is useful in scenarios where the shift amount may change during execution, such as in loops or when processing variable-length data.

### Example: Shift Left or Right by Variable

In this example, we'll perform both left and right shifts using a variable shift amount. We will load a shift amount from memory into a register and then use that register to shift another value.

#### ARM Assembly Code

```assembly
.section .data
    value: .word 0b00000000000000000000000000001000 ; Initial value (8 in decimal)
    shift_amount: .word 2                            ; Amount to shift (2 bits)
    result_left: .space 4                             ; Space to store the result of left shift
    result_right: .space 4                            ; Space to store the result of right shift

.section .text
.global _start

_start:
    // Load the value and shift amount from memory
    LDR     r0, =value                              ; Load address of value into r0
    LDR     r1, [r0]                                ; Load the value into r1

    LDR     r2, =shift_amount                       ; Load address of shift_amount into r2
    LDR     r3, [r2]                                ; Load the shift amount into r3

    // Shift left by variable amount
    LSL     r4, r1, r3                              ; r4 = r1 << r3 (shift left by variable amount)
    STR     r4, =result_left                        ; Store the result of left shift

    // Shift right by variable amount
    LSR     r5, r1, r3                              ; r5 = r1 >> r3 (shift right by variable amount)
    STR     r5, =result_right                       ; Store the result of right shift

    // Exit the program
    MOV     r7, #1                                  ; syscall number for exit
    SWI     0                                       ; software interrupt to exit

.section .bss
```

### Explanation of the Example:

1. **Data Section**:
   - **`value`**: A sample 32-bit value (8 in decimal, represented in binary as `0b00000000000000000000000000001000`).
   - **`shift_amount`**: The variable shift amount (2 bits in this example).
   - **`result_left` and `result_right`**: Reserved spaces to store the results of the shift operations.

2. **Loading Values**:
   - **`LDR r0, =value`**: Loads the address of `value` into register `r0`.
   - **`LDR r1, [r0]`**: Loads the value from memory into register `r1`.
   - **`LDR r2, =shift_amount`**: Loads the address of `shift_amount` into register `r2`.
   - **`LDR r3, [r2]`**: Loads the shift amount from memory into register `r3`.

3. **Shift Left Operation**:
   - **`LSL r4, r1, r3`**: Performs a logical shift left on `r1` by the amount specified in `r3`. The result is stored in `r4`.
   - **`STR r4, =result_left`**: Stores the result of the left shift operation into the `result_left` space.

4. **Shift Right Operation**:
   - **`LSR r5, r1, r3`**: Performs a logical shift right on `r1` by the amount specified in `r3`. The result is stored in `r5`.
   - **`STR r5, =result_right`**: Stores the result of the right shift operation into the `result_right` space.

5. **Exiting the Program**:
   - The program ends with a syscall to exit.

### Summary

- **Shifting by Variable Amount**: This example demonstrates how to use the contents of a register as the shift amount for both left and right shifts.
- **Dynamic Shifting**: This approach allows for more flexible data manipulation, especially useful when the shift amount can change during execution.
- **Result Storage**: The results of the shifts are stored for later use.
