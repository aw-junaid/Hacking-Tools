Converting a decimal number to an arbitrary base in assembly language involves repeatedly dividing the decimal number by the base and recording the remainders. These remainders represent the digits of the number in the target base, starting from the least significant digit (LSD) to the most significant digit (MSD).

### Steps to Convert Decimal to Arbitrary Base

1. **Initialize Registers**: Store the decimal number and the base.
2. **Divide and Capture Remainders**: Repeatedly divide the decimal number by the base, capturing the remainder each time. The remainder represents a digit in the new base.
3. **Store Digits in Reverse Order**: Since the remainder gives the LSD first, store the digits in reverse order.
4. **Handle Output**: Convert the numeric digits to their respective ASCII characters (for display purposes, if needed).

### Example: Converting Decimal to Base-7

Let's convert the decimal number `162` to base-7. We would divide the decimal number by 7 until the quotient is 0, recording the remainders:

$\[
162 \div 7 = 23 \, \text{remainder} \, 1
\]$

$\[
23 \div 7 = 3 \, \text{remainder} \, 2
\]$

$\[
3 \div 7 = 0 \, \text{remainder} \, 3
\]$

Thus, `162` in decimal is `321` in base-7.

### Assembly Code Example (x86)

Here’s how you can implement this process in assembly language:

```assembly
section .data
    num     dd  162           ; decimal number to convert
    base    dd  7             ; target base (in this case, base-7)
    digits  db  10 dup(0)      ; buffer to store the digits (10 bytes should be sufficient)
    digit_count dd 0           ; to keep track of how many digits we process

section .text
    global _start

_start:
    mov     eax, [num]         ; load the decimal number into eax
    mov     ebx, [base]        ; load the base into ebx
    xor     ecx, ecx           ; clear ecx, we'll use this to track index in the digits array
    xor     edx, edx           ; clear edx, as it's used by div instruction for 64-bit division

convert_loop:
    ; Perform division: eax / ebx, quotient in eax, remainder in edx
    div     ebx                ; divide eax by the base (quotient -> eax, remainder -> edx)
    add     dl, '0'            ; convert the remainder to ASCII (for display)
    mov     [digits + ecx], dl ; store the remainder in the digits array
    inc     ecx                ; increment index (ecx) for storing next digit
    
    test    eax, eax           ; check if the quotient is zero
    jnz     convert_loop       ; if not zero, continue the loop
    
    ; After the loop, ecx holds the number of digits (how many remainders we got)
    mov     [digit_count], ecx
    
    ; Now we need to reverse the order of digits for correct output
    
    ; Exit system call (we're not printing the result in this code)
    mov     eax, 60            ; exit system call number
    xor     edi, edi           ; exit code 0
    syscall

strlen:
    ; Function to calculate the length of a string
    ; eax will return length
    mov     eax, 0
    strlen_loop:
        cmp     byte [esi + eax], 0
        je      done
        inc     eax
        jmp     strlen_loop
    done:
        ret

power:
    ; Function to compute base^exponent, result in eax
    ; Takes base in eax and exponent in ecx
    mov     edx, 1
    cmp     ecx, 0
    je      done_power
    .power_loop:
        imul    edx, eax
        dec     ecx
        cmp     ecx, 0
        jne     .power_loop
    done_power:
        mov     eax, edx
        ret

    ret
```

### Key Points:
- **`div` instruction**: This performs division, placing the quotient in `eax` and the remainder in `edx`.
- **Remainder**: Each remainder represents a digit in the new base, starting from the least significant digit (LSD).
- **Digit reversal**: Since the LSD is calculated first, you may need to reverse the digits before outputting them.

### Handling Different Bases:
- For bases larger than 10 (e.g., hexadecimal, base-16), you need to handle alphabetic characters (`A-F` for 10-15). This requires an extra check to convert remainders larger than 9 into the correct ASCII characters (`'A'` to `'F'`).
