The `DIV` and `IDIV` instructions in assembly language are used for division operations, but they differ in how they handle the signs of the operands. Here’s a detailed explanation of both instructions.

### `DIV` Instruction

- **Purpose**: The `DIV` instruction is used for unsigned division.
- **Operation**: It divides the contents of the implicit dividend register (which varies based on the operand size) by the specified divisor.
- **Dividend Register**:
  - For 8-bit division: The dividend is stored in `AX`.
  - For 16-bit division: The dividend is stored in `DX:AX` (the combination of `DX` and `AX`).
  - For 32-bit division: The dividend is stored in `EDX:EAX` (the combination of `EDX` and `EAX`).
- **Result**: The quotient is stored in the lower register (e.g., `AL` for 8-bit, `AX` for 16-bit, and `EAX` for 32-bit), and the remainder is stored in the upper register (e.g., `AH` for 8-bit, `DX` for 16-bit, and `EDX` for 32-bit).

#### Syntax
```assembly
DIV divisor
```

#### Example of `DIV`
Here’s an example of using `DIV` for unsigned division:

```assembly
program UnsignedDivisionExample;

#include("stdlib.hhf")

begin UnsignedDivisionExample;

    // Initialize dividend and divisor
    mov eax, 20   // Dividend (20)
    mov ebx, 3    // Divisor (3)

    // Perform unsigned division
    div ebx       // Divide EAX by EBX; quotient in EAX, remainder in EDX

    // Output the result
    stdout.put("Unsigned Division Result: Quotient = ", eax, ", Remainder = ", edx, nl);

end UnsignedDivisionExample;
```

### `IDIV` Instruction

- **Purpose**: The `IDIV` instruction is used for signed division.
- **Operation**: Similar to `DIV`, it divides the contents of the implicit dividend register by a specified divisor but treats the numbers as signed integers.
- **Dividend Register**: The dividend register is the same as for `DIV`:
  - For 8-bit division: `AX`.
  - For 16-bit division: `DX:AX`.
  - For 32-bit division: `EDX:EAX`.
- **Result**: The quotient is stored in the same lower register as in `DIV`, while the remainder is stored in the same upper register.

#### Syntax
```assembly
IDIV divisor
```

#### Example of `IDIV`
Here’s an example of using `IDIV` for signed division:

```assembly
program SignedDivisionExample;

#include("stdlib.hhf")

begin SignedDivisionExample;

    // Initialize dividend and divisor
    mov eax, -20  // Dividend (-20)
    mov ebx, 3    // Divisor (3)

    // Perform signed division
    idiv ebx      // Divide EAX by EBX; quotient in EAX, remainder in EDX

    // Output the result
    stdout.put("Signed Division Result: Quotient = ", eax, ", Remainder = ", edx, nl);

end SignedDivisionExample;
```

### Summary of Differences

| Instruction | Type        | Dividend Register    | Result Storage            |
|-------------|-------------|-----------------------|---------------------------|
| `DIV`       | Unsigned    | AX (8-bit), DX:AX (16-bit), EDX:EAX (32-bit) | Quotient in AL/AX/EAX, Remainder in AH/DX/EDX |
| `IDIV`      | Signed      | AX (8-bit), DX:AX (16-bit), EDX:EAX (32-bit) | Quotient in AL/AX/EAX, Remainder in AH/DX/EDX |

### Important Considerations

- **Division by Zero**: Both `DIV` and `IDIV` will cause a divide error (interrupt) if the divisor is zero.
- **Sign Handling**: When using `IDIV`, the sign of the dividend and divisor is taken into account, affecting the quotient and remainder results.

### Conclusion

The `DIV` and `IDIV` instructions are fundamental for performing division operations in assembly language, with `DIV` focusing on unsigned integers and `IDIV` handling signed integers. Understanding how these instructions work, including their operand requirements and result storage, is essential for effective assembly programming.
