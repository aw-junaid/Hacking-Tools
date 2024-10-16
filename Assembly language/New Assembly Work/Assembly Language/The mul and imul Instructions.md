The `MUL` and `IMUL` instructions in assembly language are used for multiplication, but they serve slightly different purposes based on the type of multiplication required.

### `MUL` Instruction

- **Purpose**: The `MUL` instruction is used for unsigned multiplication.
- **Operation**: It multiplies the contents of a specified operand (source) with the implicit operand in the `AL`, `AX`, or `EAX` register, depending on the operand size (8-bit, 16-bit, or 32-bit).
- **Result**: The result is stored in a specific register pair:
  - For 8-bit multiplication, the result is stored in `AX` (lower 8 bits).
  - For 16-bit multiplication, the result is stored in `DX:AX` (with the high 16 bits in `DX`).
  - For 32-bit multiplication, the result is stored in `EDX:EAX` (with the high 32 bits in `EDX`).

#### Syntax
```assembly
MUL source
```

#### Example of `MUL`
Here’s an example of using `MUL` for unsigned multiplication:

```assembly
program UnsignedMultiplicationExample;

#include("stdlib.hhf")

begin UnsignedMultiplicationExample;

    // Initialize registers for multiplication
    mov eax, 10  // First operand (in EAX)
    mov ebx, 3   // Second operand (in EBX)

    // Perform unsigned multiplication
    mul ebx      // Multiply EAX by EBX; result in EDX:EAX

    // Output the result
    stdout.put("Unsigned Multiplication Result: ", eax, nl);
    stdout.put("High part: ", edx, nl); // EDX may contain a high part of the result

end UnsignedMultiplicationExample;
```

### `IMUL` Instruction

- **Purpose**: The `IMUL` instruction is used for signed multiplication.
- **Operation**: It can multiply either two operands directly (where both can be signed) or implicitly use the `AX`, `EAX`, or `RAX` registers as with `MUL`.
- **Result**: Similar to `MUL`, the result is stored in:
  - For 8-bit multiplication, the result is stored in `AX`.
  - For 16-bit multiplication, the result is stored in `DX:AX`.
  - For 32-bit multiplication, the result is stored in `EDX:EAX`.

#### Syntax
```assembly
IMUL source
```
or
```assembly
IMUL destination, source
```

#### Example of `IMUL`
Here’s an example of using `IMUL` for signed multiplication:

```assembly
program SignedMultiplicationExample;

#include("stdlib.hhf")

begin SignedMultiplicationExample;

    // Initialize registers for multiplication
    mov eax, -10  // First operand (in EAX)
    mov ebx, 3    // Second operand (in EBX)

    // Perform signed multiplication
    imul ebx      // Multiply EAX by EBX; result in EDX:EAX

    // Output the result
    stdout.put("Signed Multiplication Result: ", eax, nl);
    stdout.put("High part: ", edx, nl); // EDX may contain a high part of the result

end SignedMultiplicationExample;
```

### Summary of Differences

| Instruction | Type        | Operand(s)                    | Result Storage          |
|-------------|-------------|-------------------------------|-------------------------|
| `MUL`       | Unsigned    | Implicit (AL, AX, EAX) + source | DX:AX (16-bit), EDX:EAX (32-bit) |
| `IMUL`      | Signed      | Implicit (AL, AX, EAX) + source or two operands | DX:AX (16-bit), EDX:EAX (32-bit) |

### Conclusion

The `MUL` and `IMUL` instructions are essential for performing multiplication in assembly language, with `MUL` focusing on unsigned integers and `IMUL` handling signed integers. Understanding how to use these instructions, including their operand types and result storage, is crucial for effective assembly programming.
