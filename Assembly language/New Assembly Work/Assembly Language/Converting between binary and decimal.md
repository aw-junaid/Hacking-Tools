Converting between binary and decimal is essential in computer science and digital electronics. Below are methods for both conversions, along with examples.

### 1. Converting Binary to Decimal

To convert a binary number to decimal, you can use the positional value method, where each bit (binary digit) is multiplied by its corresponding power of 2.

#### Steps:
1. Write down the binary number.
2. Assign powers of 2 starting from the rightmost bit (which is $\(2^0\))$.
3. Multiply each bit by its corresponding power of 2.
4. Sum all the products to get the decimal value.

#### Example:
Convert the binary number **1101** to decimal.

1. Write down the binary number: **1101**
2. Assign powers of 2:
   - $\(1 \times 2^3\)$
   - $\(1 \times 2^2\)$
   - $\(0 \times 2^1\)$
   - $\(1 \times 2^0\)$

3. Calculate each term:
   $\[
   = (1 \times 8) + (1 \times 4) + (0 \times 2) + (1 \times 1)
   \]$
   
   $\[
   = 8 + 4 + 0 + 1 = 13
   \]$

Thus, **1101** in binary equals **13** in decimal.

### 2. Converting Decimal to Binary

To convert a decimal number to binary, you can use the division-by-2 method, where you repeatedly divide the decimal number by 2 and record the remainders.

#### Steps:
1. Divide the decimal number by 2.
2. Record the quotient and the remainder.
3. Repeat the division with the quotient until it equals 0.
4. The binary representation is the remainders read from bottom to top.

#### Example:
Convert the decimal number **13** to binary.

1. Divide **13** by **2**:
   $\[
   13 \div 2 = 6 \text{ remainder } 1
   \]$

2. Divide the quotient **6** by **2**:
   $\[
   6 \div 2 = 3 \text{ remainder } 0
   \]$

3. Divide the quotient **3** by **2**:
   $\[
   3 \div 2 = 1 \text{ remainder } 1
   \]$

4. Divide the quotient **1** by **2**:
   $\[
   1 \div 2 = 0 \text{ remainder } 1
   \]$

5. Now read the remainders from bottom to top: **1101**.

Thus, **13** in decimal equals **1101** in binary.

### Summary of Conversion Methods

- **Binary to Decimal**: Multiply each bit by $\(2\)$ raised to the power of its position and sum the results.
- **Decimal to Binary**: Divide the number by $\(2\)$, record the remainder, and continue dividing the quotient until it reaches $\(0\)$. The binary number is formed by reading the remainders in reverse order.

### Additional Examples

#### Example 1: Binary to Decimal
Convert **1010** to decimal.
$\[
1010_2 = (1 \times 2^3) + (0 \times 2^2) + (1 \times 2^1) + (0 \times 2^0)
\]$

$\[
= 8 + 0 + 2 + 0 = 10_{10}
\]$

#### Example 2: Decimal to Binary
Convert **10** to binary.
1. $\(10 \div 2 = 5\)$ remainder **0**
2. $\(5 \div 2 = 2\)$ remainder **1**
3. $\(2 \div 2 = 1\)$ remainder **0**
4. $\(1 \div 2 = 0\)$ remainder **1**

Reading from bottom to top: **1010**.

Thus, **10** in decimal equals **1010** in binary.

### Conclusion

Understanding how to convert between binary and decimal is fundamental in computer science, as binary is the native language of computers. Being proficient in these conversions helps in various areas, including programming, data representation, and digital circuit design.
