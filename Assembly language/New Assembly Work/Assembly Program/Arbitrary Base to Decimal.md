Converting a number from an arbitrary base (e.g., binary, hexadecimal, or base-7) to decimal in assembly language involves iterating through each digit of the number, multiplying it by the base raised to the appropriate power, and summing the results.

Here’s how you can approach the conversion process:

### Formula for Arbitrary Base to Decimal
The formula to convert a number from an arbitrary base $\( B \)$ to decimal is:

$\[
\text{Decimal Value} = d_n \times B^n + d_{n-1} \times B^{n-1} + \cdots + d_1 \times B^1 + d_0 \times B^0
\]$

Where:
- $\( d_n, d_{n-1}, \dots, d_0 \)$ are the digits of the number in the arbitrary base.
- $\( B \)$ is the base of the number (e.g., 2 for binary, 16 for hexadecimal).
- $\( n \)$ is the position of the digit (starting from the right, where the least significant digit is at position 0).

### Steps to Implement in Assembly

1. **Initialize Registers**: Store the base of the number and initialize variables for powers of the base and the final result.
2. **Iterate through Each Digit**: Start with the least significant digit, multiply it by the base raised to the power of its position, and accumulate the result.
3. **Handle Digit-by-Digit Multiplication**: Each digit is multiplied by the appropriate power of the base, and the result is added to the running sum.

### Example: Converting Base-7 to Decimal (x86 Assembly)

Let’s say you want to convert the number `321` in base-7 to its decimal equivalent.

$\[
321_{\text{base-7}} = 3 \times 7^2 + 2 \times 7^1 + 1 \times 7^0
\]$

This would give the decimal result of:

$\[
3 \times 49 + 2 \times 7 + 1 = 147 + 14 + 1 = 162
\]$

### Assembly Code Example

```assembly
section .data
    num     db  '321', 0     ; base-7 number as string (digits are ASCII characters)
    base    dd  7            ; base (7 in this case)

section .text
    global _start

_start:
    ; Initialize registers
    xor     ebx, ebx          ; result (total sum), set to 0
    xor     ecx, ecx          ; loop counter for digit position
    mov     edi, 0            ; power of base, initialized to 0
    
    ; Start at the most significant digit (in our case, '3' from "321")
    mov     esi, num
    call    strlen            ; get the length of the number string
    dec     eax               ; adjust for zero-based index
    
convert_loop:
    ; Get the current digit (starting with the least significant)
    mov     al, [esi + eax]   ; load the current ASCII digit
    sub     al, '0'           ; convert ASCII to integer
    mov     edx, eax          ; save the digit in edx
    
    ; Multiply the digit by base^position
    mov     eax, [base]       ; load base value (7)
    mov     ecx, edi          ; use ecx to track power of base
    call    power             ; eax = base^position
    imul    edx, eax          ; multiply digit by base^position
    
    ; Add the result to the total sum (stored in ebx)
    add     ebx, edx
    
    ; Update for next iteration
    dec     eax               ; go to the next digit (moving left)
    inc     edi               ; increment power of base
    test    eax, eax          ; check if we've processed all digits
    jge     convert_loop      ; repeat if there are digits left
    
    ; ebx now contains the base-100

    ; program call sys_exit
    mov     eax  
