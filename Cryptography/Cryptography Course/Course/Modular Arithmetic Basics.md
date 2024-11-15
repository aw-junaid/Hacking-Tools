### **Euclidean Algorithm (Greatest Common Divisor Problem)**

The **Euclidean Algorithm** is a method for finding the **Greatest Common Divisor (GCD)** of two integers, \( a \) and \( b \). The GCD of two numbers is the largest positive integer that divides both \( a \) and \( b \) without leaving a remainder.

The Euclidean algorithm works by repeatedly applying the **division algorithm**, which states:
$\[
a = bq + r
\]$
where:
- \( a \) and \( b \) are the two integers,
- \( q \) is the quotient,
- \( r \) is the remainder, where $\( 0 \leq r < b \)$.

The Euclidean algorithm then recursively applies the same process to \( b \) and \( r \) until \( r = 0 \), at which point \( b \) is the GCD.

#### **Steps of the Euclidean Algorithm:**
1. Divide \( a \) by \( b \), obtaining the quotient \( q \) and remainder \( r \).
2. Replace \( a \) with \( b \), and \( b \) with \( r \).
3. Repeat the process until $\( r = 0 \)$, at which point \( b \) is the GCD of \( a \) and \( b \).

#### **Example:**

Let's find the GCD of 252 and 105 using the Euclidean Algorithm.

1. $\( 252 \div 105 = 2 \)$ (quotient) and remainder $\( r = 252 - 105 \times 2 = 42 \)$, so:
   $\[
   252 = 105 \times 2 + 42
   \]$
2. Next, apply the algorithm to \( 105 \) and \( 42 \):
   $\[
   105 \div 42 = 2 \ \text{(quotient)}, \ 105 - 42 \times 2 = 21 \ \text{(remainder)}
   \]$
   $\[
   105 = 42 \times 2 + 21
   \]$
3. Now, apply the algorithm to \( 42 \) and \( 21 \):
   $\[
   42 \div 21 = 2 \ \text{(quotient)}, \ 42 - 21 \times 2 = 0 \ \text{(remainder)}
   \]$
   $\[
   42 = 21 \times 2 + 0
   \]$

Since the remainder is now 0, the GCD is \( 21 \).

Thus, **GCD(252, 105) = 21**.

---

### **Extended Euclidean Algorithm**

The **Extended Euclidean Algorithm** is an extension of the Euclidean algorithm that, in addition to computing the GCD of two numbers, also finds the coefficients \( x \) and \( y \) such that:
$\[
ax + by = \text{GCD}(a, b)
\]$
These coefficients are important in various cryptographic applications, such as finding **modular inverses** in RSA encryption and the **Chinese Remainder Theorem**.

#### **Steps of the Extended Euclidean Algorithm:**
1. Use the Euclidean algorithm to compute the GCD of \( a \) and \( b \).
2. At each step, keep track of how the remainder \( r \) can be expressed as a linear combination of \( a \) and \( b \).
3. When the remainder becomes 0, backtrack to express the GCD as $\( ax + by \)$.

#### **Example:**

Let's use the Extended Euclidean Algorithm to find the coefficients \( x \) and \( y \) such that:
$\[
252x + 105y = \text{GCD}(252, 105)
\]$
From the previous Euclidean algorithm steps, we know that **GCD(252, 105) = 21**. Now, we need to express 21 as a linear combination of 252 and 105.

1. From the first division step:
   $\[
   252 = 105 \times 2 + 42
   \]$
   Rearranging this:
   $\[
   42 = 252 - 105 \times 2
   \]$
   
2. From the second division step:
   $\[
   105 = 42 \times 2 + 21
   \]$
   Rearranging this:
   $\[
   21 = 105 - 42 \times 2
   \]$
   
3. Now substitute the value of 42 from the first step into this equation:
   $\[
   21 = 105 - 2 \times (252 - 105 \times 2)
   \]$
   Simplifying:
   $\[
   21 = 105 - 2 \times 252 + 4 \times 105
   \]$
   $\[
   21 = 5 \times 105 - 2 \times 252
   \]$

Thus, we have:
$\[
21 = 5 \times 105 - 2 \times 252
\]$

Therefore, the coefficients are $\( x = -2 \)$ and $\( y = 5 \)$. So:
$\[
252(-2) + 105(5) = 21
\]$

This shows that the linear combination of \( 252 \) and \( 105 \) that gives the GCD is:
$\[
252(-2) + 105(5) = 21
\]$

---

### **Applications of the Extended Euclidean Algorithm:**

1. **Modular Inverse**: In cryptography, especially in algorithms like **RSA**, the Extended Euclidean Algorithm is used to compute the modular inverse. That is, given \( a \) and \( n \), it helps find \( x \) such that:
   $\[
   ax \equiv 1 \ (\text{mod} \ n)
   \]$
   The Extended Euclidean Algorithm can find such \( x \) by solving $\( ax + ny = 1 \)$ for integers \( x \) and \( y \).

2. **RSA Key Generation**: The Extended Euclidean Algorithm is used during the key generation process of RSA to compute the modular inverse of \( e \) modulo \( \varphi(n) \), where \( e \) is the public exponent and \( \varphi(n) \) is Euler’s totient function.

3. **Solving Diophantine Equations**: It can be used to solve Diophantine equations of the form $\( ax + by = d \)$, where \( d \) is the GCD of \( a \) and \( b \).

---

### **Code Implementation of Euclidean Algorithm and Extended Euclidean Algorithm (Python)**

#### **Euclidean Algorithm (GCD)**:

```python
def euclidean_algorithm(a, b):
    while b != 0:
        a, b = b, a % b
    return a

# Example usage
a = 252
b = 105
gcd = euclidean_algorithm(a, b)
print("GCD:", gcd)
```

#### **Extended Euclidean Algorithm**:

```python
def extended_euclidean_algorithm(a, b):
    if b == 0:
        return a, 1, 0  # GCD, x, y
    gcd, x1, y1 = extended_euclidean_algorithm(b, a % b)
    x = y1
    y = x1 - (a // b) * y1
    return gcd, x, y

# Example usage
a = 252
b = 105
gcd, x, y = extended_euclidean_algorithm(a, b)
print(f"GCD: {gcd}, x: {x}, y: {y}")
```

### **Conclusion:**

The **Euclidean algorithm** is a powerful and efficient method for computing the **greatest common divisor** of two numbers, while the **extended Euclidean algorithm** not only computes the GCD but also finds the coefficients that express the GCD as a linear combination of the two numbers. These algorithms are crucial for many cryptographic applications, including modular inverses, RSA encryption, and solving Diophantine equations.
