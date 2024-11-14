### **Cryptography - Affine Cipher**

The **Affine Cipher** is a type of substitution cipher that applies mathematical operations to encrypt and decrypt a message. It is a combination of two mathematical functions: multiplication and addition. The Affine Cipher works by transforming each letter of the plaintext using a linear function and a key consisting of two components: a multiplier and an adder.

### **Affine Cipher Encryption**

The encryption function for the Affine Cipher is defined as:

$\[
E(x) = (ax + b) \mod 26
\]$

Where:
- **x** is the numerical equivalent of the plaintext letter (A = 0, B = 1, ..., Z = 25).
- **a** and **b** are the keys used for encryption:
  - **a** must be coprime with 26 (i.e., **gcd(a, 26) = 1**), ensuring that every letter can be encrypted.
  - **b** is a shift value (can be any integer between 0 and 25).
- The result is mapped back to a letter in the alphabet.

### **Affine Cipher Decryption**

The decryption function for the Affine Cipher is:

$\[
D(y) = a^{-1}(y - b) \mod 26
\]$

Where:
- **y** is the numerical equivalent of the ciphertext letter.
- **a^{-1}** is the modular inverse of **a** (i.e., a number such that $\(a \times a^{-1} \mod 26 = 1\))$.
- **b** is the shift value used in the encryption process.

The modular inverse of **a** can be found using the Extended Euclidean Algorithm.

### **Encryption Example**

Let's say we want to encrypt the message "HELLO" using an Affine Cipher with the following keys:
- **a = 5**
- **b = 8**

#### Step 1: Assign Numerical Values to Letters

Using the standard assignment:
- H = 7
- E = 4
- L = 11
- L = 11
- O = 14

#### Step 2: Apply the Encryption Formula

Using the formula $\( E(x) = (ax + b) \mod 26 \)$, we calculate the ciphertext for each letter:

- For **H** (7):  
  $\[ E(7) = (5 \times 7 + 8) \mod 26 = (35 + 8) \mod 26 = 43 \mod 26 = 17 \]$ → Corresponding to **R**
  
- For **E** (4):  
  $\[ E(4) = (5 \times 4 + 8) \mod 26 = (20 + 8) \mod 26 = 28 \mod 26 = 2 \]$ → Corresponding to **C**
  
- For **L** (11):  
  $\[ E(11) = (5 \times 11 + 8) \mod 26 = (55 + 8) \mod 26 = 63 \mod 26 = 11 \]$ → Corresponding to **L**
  
- For **L** (11):  
  $\[ E(11) = (5 \times 11 + 8) \mod 26 = (55 + 8) \mod 26 = 63 \mod 26 = 11 \]$ → Corresponding to **L**
  
- For **O** (14):  
  $\[ E(14) = (5 \times 14 + 8) \mod 26 = (70 + 8) \mod 26 = 78 \mod 26 = 0 \]$ → Corresponding to **A**

Thus, the **ciphertext** is "RCLLA".

### **Decryption Example**

To decrypt the ciphertext "RCLLA", we need to reverse the encryption process. We use the decryption formula:

$\[
D(y) = a^{-1}(y - b) \mod 26
\]$

#### Step 1: Find the Modular Inverse of **a**

To decrypt the message, we first need to find the modular inverse of **a = 5** mod 26. The modular inverse of **5** mod 26 is **21**, because:

$\[
5 \times 21 \mod 26 = 105 \mod 26 = 1
\]$

#### Step 2: Apply the Decryption Formula

Using the formula $\( D(y) = a^{-1}(y - b) \mod 26 \)$, we calculate the plaintext for each letter in the ciphertext "RCLLA":

- For **R** (17):  
  $\[ D(17) = 21 \times (17 - 8) \mod 26 = 21 \times 9 \mod 26 = 189 \mod 26 = 7 \]$ → Corresponding to **H**

- For **C** (2):  
  $\[ D(2) = 21 \times (2 - 8) \mod 26 = 21 \times (-6) \mod 26 = -126 \mod 26 = 4 \]$ → Corresponding to **E**

- For **L** (11):  
  $\[ D(11) = 21 \times (11 - 8) \mod 26 = 21 \times 3 \mod 26 = 63 \mod 26 = 11 \]$ → Corresponding to **L**

- For **L** (11):  
  $\[ D(11) = 21 \times (11 - 8) \mod 26 = 21 \times 3 \mod 26 = 63 \mod 26 = 11 \]$ → Corresponding to **L**

- For **A** (0):  
  $\[ D(0) = 21 \times (0 - 8) \mod 26 = 21 \times (-8) \mod 26 = -168 \mod 26 = 14 \]$ → Corresponding to **O**

Thus, the **decrypted plaintext** is "HELLO".

### **Python Code for Affine Cipher**

Here’s a Python implementation for both encryption and decryption of the Affine Cipher:

```python
# Function to find the modular inverse of a number mod 26
def mod_inverse(a, m):
    for x in range(1, m):
        if (a * x) % m == 1:
            return x
    return None  # If no modular inverse exists

# Function to encrypt a message using the Affine cipher
def affine_encrypt(plaintext, a, b):
    ciphertext = ""
    for char in plaintext:
        if char.isalpha():  # Ignore non-alphabet characters
            # Convert the character to its numeric equivalent (A=0, B=1, ..., Z=25)
            num = ord(char.upper()) - ord('A')
            # Apply the encryption formula: E(x) = (a * x + b) mod 26
            encrypted_num = (a * num + b) % 26
            ciphertext += chr(encrypted_num + ord('A'))
        else:
            ciphertext += char  # Non-alphabet characters remain unchanged
    return ciphertext

# Function to decrypt a message using the Affine cipher
def affine_decrypt(ciphertext, a, b):
    plaintext = ""
    # Find the modular inverse of a
    a_inv = mod_inverse(a, 26)
    if a_inv is None:
        return "Error: No modular inverse found for the given 'a'."
    
    for char in ciphertext:
        if char.isalpha():  # Ignore non-alphabet characters
            # Convert the character to its numeric equivalent (A=0, B=1, ..., Z=25)
            num = ord(char.upper()) - ord('A')
            # Apply the decryption formula: D(y) = a_inv * (y - b) mod 26
            decrypted_num = (a_inv * (num - b)) % 26
            plaintext += chr(decrypted_num + ord('A'))
        else:
            plaintext += char  # Non-alphabet characters remain unchanged
    return plaintext

# Sample usage
plaintext = "HELLO"
a = 5
b = 8

# Encrypt the message using the Affine cipher
ciphertext = affine_encrypt(plaintext, a, b)
print(f"Ciphertext: {ciphertext}")

# Decrypt the ciphertext using the Affine cipher
decrypted_text = affine_decrypt(ciphertext, a, b)
print(f"Decrypted Text: {decrypted_text}")
```

### **Explanation of the Code:**

1. **`mod_inverse` function**: Finds the modular inverse of `a` modulo 26. This is used during decryption.
2. **`affine_encrypt` function**: Encrypts the plaintext by applying the Affine cipher formula to each letter.
3. **`affine_decrypt` function**: Decrypts the ciphertext by applying the modular inverse of `a` and reversing the encryption formula.

### **Example Output:**

```
Ciphertext: RCLLA
Decrypted Text: HELLO
```

### **Advantages of Affine Cipher:**

1. **Simple and Efficient**: It is relatively simple to implement and requires only basic modular arithmetic.
2. **Mathematically Interesting**: It provides a good example of how mathematical operations can be used in encryption algorithms.

### **Drawbacks of Affine Cipher:**

1. **Vulnerable to Frequency Analysis**: Like other substitution ciphers, the Affine cipher is vulnerable to frequency analysis, especially when used with short messages.
2. **Limited

 Key Space**: The number of possible keys is limited by the condition that **a** must be coprime with 26, which reduces the security.

### **Conclusion**

The **Affine Cipher** is a simple substitution cipher that combines multiplication and addition to encrypt and decrypt messages. While it's useful for educational purposes, it is not secure enough for modern cryptographic needs due to its susceptibility to frequency analysis.
