The **binary**, **decimal**, and **hexadecimal** number systems are ways of representing numerical values using different bases. Let's explore each system and how to convert between them.

### 1. **Binary Number System**
- **Base**: 2
- **Digits used**: 0, 1
- **Explanation**: Each binary digit (bit) represents a power of 2, starting from the rightmost digit.

**Example**:  
`1011₂` (binary) = \( 1 \times 2^3 + 0 \times 2^2 + 1 \times 2^1 + 1 \times 2^0 \)  
= \( 1 \times 8 + 0 \times 4 + 1 \times 2 + 1 \times 1 \)  
= 11 (decimal)

### 2. **Decimal Number System**
- **Base**: 10
- **Digits used**: 0-9
- **Explanation**: The decimal system is the most commonly used system in everyday life. Each digit represents a power of 10.

**Example**:  
`345₁₀` (decimal) = \( 3 \times 10^2 + 4 \times 10^1 + 5 \times 10^0 \)  
= \( 3 \times 100 + 4 \times 10 + 5 \times 1 \)  
= 345 (decimal)

### 3. **Hexadecimal Number System**
- **Base**: 16
- **Digits used**: 0-9 and A-F (where A = 10, B = 11, C = 12, D = 13, E = 14, F = 15)
- **Explanation**: The hexadecimal system is commonly used in computing because it's easy to represent large binary numbers in a compact form.

**Example**:  
`2A₁₆` (hexadecimal) = \( 2 \times 16^1 + A \times 16^0 \)  
= \( 2 \times 16 + 10 \times 1 \)  
= 32 + 10 = 42 (decimal)

---

### **Conversions Between Binary, Decimal, and Hexadecimal**

#### **1. Binary to Decimal**
To convert a binary number to decimal:
1. Start from the rightmost bit.
2. Multiply each bit by 2 raised to the power of its position.
3. Sum the results.

**Example**:  
Convert `10110₂` to decimal:  
\( 1 \times 2^4 + 0 \times 2^3 + 1 \times 2^2 + 1 \times 2^1 + 0 \times 2^0 \)  
= \( 1 \times 16 + 0 \times 8 + 1 \times 4 + 1 \times 2 + 0 \times 1 \)  
= 16 + 0 + 4 + 2 + 0 = **22 (decimal)**

#### **2. Decimal to Binary**
To convert a decimal number to binary:
1. Divide the decimal number by 2.
2. Write down the remainder (0 or 1).
3. Divide the quotient by 2 and repeat until the quotient is 0.
4. The binary number is the sequence of remainders read from bottom to top.

**Example**:  
Convert `19₁₀` to binary:  
19 ÷ 2 = 9 remainder 1  
9 ÷ 2 = 4 remainder 1  
4 ÷ 2 = 2 remainder 0  
2 ÷ 2 = 1 remainder 0  
1 ÷ 2 = 0 remainder 1  

Thus, `19₁₀` = **10011₂** (binary)

#### **3. Binary to Hexadecimal**
To convert binary to hexadecimal:
1. Group the binary number into groups of 4 bits (starting from the right). Add leading zeros if necessary.
2. Convert each group of 4 bits into its hexadecimal equivalent.

**Example**:  
Convert `10111001₂` to hexadecimal:  
`1011 1001` (grouped in 4 bits)  
`1011₂ = B₁₆` and `1001₂ = 9₁₆`  
Thus, `10111001₂` = **B9₁₆** (hexadecimal)

#### **4. Hexadecimal to Binary**
To convert hexadecimal to binary:
1. Convert each hexadecimal digit to its 4-bit binary equivalent.

**Example**:  
Convert `A3₁₆` to binary:  
`A₁₆ = 1010₂` and `3₁₆ = 0011₂`  
Thus, `A3₁₆` = **10100011₂** (binary)

#### **5. Decimal to Hexadecimal**
To convert a decimal number to hexadecimal:
1. Divide the decimal number by 16.
2. Write down the remainder (0-9 or A-F).
3. Divide the quotient by 16 and repeat until the quotient is 0.
4. The hexadecimal number is the sequence of remainders read from bottom to top.

**Example**:  
Convert `45₁₀` to hexadecimal:  
45 ÷ 16 = 2 remainder 13 (D in hexadecimal)  
2 ÷ 16 = 0 remainder 2  

Thus, `45₁₀` = **2D₁₆** (hexadecimal)

#### **6. Hexadecimal to Decimal**
To convert hexadecimal to decimal:
1. Multiply each digit of the hexadecimal number by 16 raised to the power of its position.
2. Sum the results.

**Example**:  
Convert `1F₁₆` to decimal:  
\( 1 \times 16^1 + F \times 16^0 \)  
= \( 1 \times 16 + 15 \times 1 \)  
= 16 + 15 = **31₁₀** (decimal)

---

### **Summary of Common Conversions**:

| **From / To**      | **Binary**                    | **Decimal**                   | **Hexadecimal**               |
|--------------------|-------------------------------|-------------------------------|-------------------------------|
| **Binary**         | -                             | Multiply by powers of 2        | Group into 4 bits              |
| **Decimal**        | Divide by 2                   | -                             | Divide by 16                   |
| **Hexadecimal**    | Convert to 4-bit groups       | Multiply by powers of 16       | -                             |

These conversion techniques allow easy switching between number systems, which is crucial in fields like computer science, digital electronics, and networking.
