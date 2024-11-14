### **Cryptography - Multiplicative Cipher**

The **Multiplicative Cipher** is a type of substitution cipher where each letter in the plaintext is replaced by another letter based on a multiplication rule. It is a simple form of encryption that involves multiplication in modular arithmetic. The cipher works by multiplying the numerical value of each letter by a constant number (the key), and the result is reduced modulo the size of the alphabet. This process transforms the plaintext into ciphertext.

### **How Multiplicative Cipher Works**

The basic idea behind the multiplicative cipher is:

1. **Assign Numbers to Letters**: First, assign each letter of the alphabet a numerical value. Typically, this is done using **A = 0, B = 1, C = 2, ..., Z = 25**.
   
2. **Choose a Key**: The key is a number `k`, and it must be coprime with 26 (the number of letters in the alphabet). This ensures that every letter can be encrypted and decrypted correctly.

3. **Encryption**: For each letter in the plaintext, replace it with the letter corresponding to the product of the key and the letter's numerical value, modulo 26. Mathematically, this can be represented as:
   
   $\[
   C = (P \times k) \mod 26
   \]$
   Where:
   - `C` is the ciphertext letter
   - `P` is the plaintext letter (numerical value)
   - `k` is the key
   - `mod 26` ensures the result is within the range of 0 to 25 (the alphabet).

4. **Decryption**: To decrypt the message, you need to find the modular inverse of the key. The modular inverse of `k` is the number `k'` such that:
   
   $\[
   (k \times k') \mod 26 = 1
   \]$
   Once you have the modular inverse `k'`, the plaintext `P` can be recovered by using the formula:
   
   $\[
   P = (C \times k') \mod 26
   \]$

### **Example of Multiplicative Cipher**

#### **Encryption Example**

Let's say we want to encrypt the message "HELLO" using the multiplicative cipher with a key `k = 7`.

1. **Assign Numbers to Letters**:
   - H = 7
   - E = 4
   - L = 11
   - L = 11
   - O = 14

2. **Apply the Multiplicative Encryption Formula**:
   Using the key `k = 7`, we apply the formula $\( C = (P \times k) \mod 26 \)$ to each letter.

   - For 'H' (7): $\( C = (7 \times 7) \mod 26 = 49 \mod 26 = 23 \)$ → Corresponding to 'X'
   - For 'E' (4): $\( C = (4 \times 7) \mod 26 = 28 \mod 26 = 2 \)$ → Corresponding to 'C'
   - For 'L' (11): $\( C = (11 \times 7) \mod 26 = 77 \mod 26 = 25 \)$ → Corresponding to 'Z'
   - For 'L' (11): $\( C = (11 \times 7) \mod 26 = 77 \mod 26 = 25 \)$ → Corresponding to 'Z'
   - For 'O' (14): $\( C = (14 \times 7) \mod 26 = 98 \mod 26 = 18 \)$ → Corresponding to 'S'

Thus, the encrypted message is:
```
Ciphertext: XCZZS
```

#### **Decryption Example**

To decrypt the ciphertext "XCZZS", we need to find the modular inverse of the key `k = 7` mod 26.

1. **Find the Modular Inverse**:
   We need to find a value `k'` such that:
   $\[
   (7 \times k') \mod 26 = 1
   \]$
   By testing values, we find that the modular inverse of 7 modulo 26 is 15 $(since \( (7 \times 15) \mod 26 = 105 \mod 26 = 1 \))$.

2. **Apply the Decryption Formula**:
   Now, we decrypt each letter of the ciphertext using the formula:
   $\[
   P = (C \times k') \mod 26
   \]$
   - For 'X' (23): $\( P = (23 \times 15) \mod 26 = 345 \mod 26 = 7 \)$ → Corresponding to 'H'
   - For 'C' (2): $\( P = (2 \times 15) \mod 26 = 30 \mod 26 = 4 \)$ → Corresponding to 'E'
   - For 'Z' (25): $\( P = (25 \times 15) \mod 26 = 375 \mod 26 = 11 \)$ → Corresponding to 'L'
   - For 'Z' (25): $\( P = (25 \times 15) \mod 26 = 375 \mod 26 = 11 \)$ → Corresponding to 'L'
   - For 'S' (18): $\( P = (18 \times 15) \mod 26 = 270 \mod 26 = 14 \)$ → Corresponding to 'O'

Thus, the decrypted message is:
```
Plaintext: HELLO
```

### **Python Code for Multiplicative Cipher**

Here’s a Python implementation of the Multiplicative Cipher for encryption and decryption:

```python
# Function to find the modular inverse of a number modulo 26
def mod_inverse(a, m):
    for x in range(1, m):
        if (a * x) % m == 1:
            return x
    return None  # If no modular inverse exists

# Function to encrypt a message using the multiplicative cipher
def multiplicative_encrypt(plaintext, key):
    ciphertext = ""
    for char in plaintext:
        if char.isalpha():  # Ignore non-alphabet characters
            # Convert the character to its numeric equivalent (A=0, B=1, ..., Z=25)
            num = ord(char.upper()) - ord('A')
            # Apply the encryption formula: C = (P * k) mod 26
            encrypted_num = (num * key) % 26
            ciphertext += chr(encrypted_num + ord('A'))
        else:
            ciphertext += char  # Non-alphabet characters remain unchanged
    return ciphertext

# Function to decrypt a message using the multiplicative cipher
def multiplicative_decrypt(ciphertext, key):
    plaintext = ""
    # Find the modular inverse of the key
    key_inverse = mod_inverse(key, 26)
    if key_inverse is None:
        return "Error: No modular inverse found for the given key."
    
    for char in ciphertext:
        if char.isalpha():  # Ignore non-alphabet characters
            # Convert the character to its numeric equivalent (A=0, B=1, ..., Z=25)
            num = ord(char.upper()) - ord('A')
            # Apply the decryption formula: P = (C * k') mod 26
            decrypted_num = (num * key_inverse) % 26
            plaintext += chr(decrypted_num + ord('A'))
        else:
            plaintext += char  # Non-alphabet characters remain unchanged
    return plaintext

# Sample usage
plaintext = "HELLO"
key = 7

# Encrypt the message using the multiplicative cipher
ciphertext = multiplicative_encrypt(plaintext, key)
print(f"Ciphertext: {ciphertext}")

# Decrypt the ciphertext using the multiplicative cipher
decrypted_text = multiplicative_decrypt(ciphertext, key)
print(f"Decrypted Text: {decrypted_text}")
```

### **Explanation of the Code:**

1. **`mod_inverse` function**: This function finds the modular inverse of the key modulo 26. It iterates over all possible values and checks if the product of the key and the current number modulo 26 equals 1.

2. **`multiplicative_encrypt` function**: This function takes the plaintext and key, converts each letter to a numeric value, applies the encryption formula, and then converts the result back into letters to form the ciphertext.

3. **`multiplicative_decrypt` function**: This function decrypts the ciphertext by applying the modular inverse of the key and recovering the original plaintext.

### **Example Output:**

```
Ciphertext: XCZZS
Decrypted Text: HELLO
```

### **Advantages of Multiplicative Cipher:**

1. **Simple**: It is easy to implement and understand.
2. **Strong for Small Texts**: The key provides a simple form of encryption for small messages or as part of more complex ciphers.

### **Drawbacks of Multiplicative Cipher:**

1. **Weak Security**: The cipher is vulnerable to frequency analysis and is not secure for modern encryption needs.
2. **Key Limitations**: The key must be coprime with 26, which limits the number of possible keys to just those that meet this condition (i.e., keys like 1, 3, 5, 7, etc.).

### **Conclusion**

The **Multiplicative Cipher** is a simple but weak encryption technique that relies on modular multiplication. While it can provide basic security for small messages, it is not suitable for securing sensitive

 or large-scale data. It is mainly of educational interest today, to help understand the fundamentals of encryption.
