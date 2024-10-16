The `CMP` (compare) instruction in assembly language is used to compare two operands by performing a subtraction operation without storing the result. Instead, it sets the CPU's flags based on the outcome of this subtraction, allowing for conditional branching based on the comparison.

### Key Features of the `CMP` Instruction

1. **Operation**: The `CMP` instruction subtracts the second operand from the first operand:
   \[
   \text{Flags} = \text{First Operand} - \text{Second Operand}
   \]

2. **Flags Affected**: The `CMP` instruction affects several flags in the status register, including:
   - **Zero Flag (ZF)**: Set if the result is zero (operands are equal).
   - **Sign Flag (SF)**: Set if the result is negative (first operand is less than the second operand).
   - **Carry Flag (CF)**: Set if there is a borrow (first operand is less than the second operand, indicating an unsigned comparison).
   - **Overflow Flag (OF)**: Set if the subtraction operation resulted in an overflow.

3. **Operands**: The operands can be registers, memory locations, or immediate values. Both operands must be of the same size (e.g., both 8-bit, 16-bit, or 32-bit).

### Syntax

```assembly
CMP operand1, operand2
```

### Example of the `CMP` Instruction

Here’s an example demonstrating how to use the `CMP` instruction in assembly language:

```assembly
program CompareExample;

#include("stdlib.hhf")

begin CompareExample;

    // Initialize two values for comparison
    mov eax, 10    // First operand (in EAX)
    mov ebx, 20    // Second operand (in EBX)

    // Compare the values in EAX and EBX
    cmp eax, ebx   // Compare EAX (10) with EBX (20)

    // Check the result using conditional jumps
    je  equal      // Jump to equal if ZF is set (equal case)
    jl  less       // Jump to less if SF is set (less case)
    jg  greater    // Jump to greater if neither of the above conditions is met

equal:
    stdout.put("The values are equal.", nl);
    jmp end_program;

less:
    stdout.put("The first value is less than the second value.", nl);
    jmp end_program;

greater:
    stdout.put("The first value is greater than the second value.", nl);

end_program:
    // Exit the program
    stdout.put("End of program.", nl);

end CompareExample;
```

### Explanation of the Example

1. **Initialization**: Two values are loaded into the `EAX` and `EBX` registers. Here, `EAX` contains `10`, and `EBX` contains `20`.

2. **Comparison**: The `CMP` instruction is used to compare the values in `EAX` and `EBX`. This operation sets the flags based on the subtraction result.

3. **Conditional Jumps**:
   - **`je equal`**: Jumps to the `equal` label if the Zero Flag (ZF) is set (indicating the values are equal).
   - **`jl less`**: Jumps to the `less` label if the Sign Flag (SF) is set (indicating the first operand is less than the second).
   - **`jg greater`**: Jumps to the `greater` label if the first operand is greater than the second.

4. **Output**: Depending on the comparison, a different message is printed to the standard output.

### Conclusion

The `CMP` instruction is a powerful tool for comparing values in assembly language. By setting the CPU's flags based on the comparison result, it allows for conditional branching and decision-making in programs. Understanding how to use `CMP` effectively is essential for control flow in assembly programming.
