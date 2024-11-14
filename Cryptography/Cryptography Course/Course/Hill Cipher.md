### **Cryptography - Hill Cipher**

The **Hill Cipher** is a classical polygraphic substitution cipher invented by mathematician **Claude Shannon** in 1929 and named after the mathematician **Lester S. Hill**. It is based on linear algebra, specifically matrix multiplication, and is a **block cipher** that encrypts messages in blocks of letters. This makes it more secure than simple substitution ciphers such as the Caesar cipher, as it encrypts multiple letters simultaneously rather than just one letter at a time.

### **How the Hill Cipher Works**

The Hill Cipher operates by using **matrix multiplication** to transform the plaintext into ciphertext. A key component of this cipher is a **key matrix**. The plaintext is divided into blocks (usually of size 2 or 3 characters) and then each block is multiplied by the key matrix. The result is a ciphertext block.

#### **Steps for Encryption:**

1. **Create the Key Matrix:**
   The key matrix is a square matrix of size **n x n**, where **n** is the block size (e.g., for 2-character blocks, the matrix will be 2x2). The matrix should be invertible to ensure that the decryption process works.
   
   The matrix is filled with integer values, typically derived from a keyword or phrase. Each letter of the alphabet is mapped to a number (A = 0, B = 1, C = 2, ..., Z = 25). The key matrix is then used to perform the matrix multiplication.

2. **Prepare the Plaintext:**
   - Convert the plaintext into numerical values, where **A = 0**, **B = 1**, ..., **Z = 25**.
   - If the plaintext has an odd number of characters, an extra letter (usually **X**) is added to make it even.
   - The plaintext is then divided into blocks of **n** characters.

3. **Matrix Multiplication for Encryption:**
   Each block of plaintext is treated as a column vector. This vector is multiplied by the key matrix. The result of the multiplication is a new vector, which corresponds to the ciphertext block.

4. **Convert the Result to Letters:**
   After performing the matrix multiplication, the result is a vector of numbers, which is then converted back to letters by mapping the numbers back to their corresponding letters in the alphabet.

---

### **Example of Hill Cipher Encryption**

Let's walk through an example using a **2x2 matrix** and **2-character blocks**.

#### **Plaintext:** "HELLO"
#### **Key Matrix:**  
   Let's take the key matrix as follows:
   ```
   Key Matrix = [ [6, 24],
                  [1, 13] ]
   ```

   - We convert the plaintext "HELLO" into numbers:
     ```
     H = 7, E = 4, L = 11, L = 11, O = 14
     ```

   - We group the plaintext into blocks of 2:
     ```
     Block 1: HE → [7, 4]
     Block 2: LL → [11, 11]
     Block 3: O → [14] (Add padding X → [23])
     ```

#### **Encryption of Block 1:**
- Multiply the key matrix by the plaintext vector:
  
  ```
  [6, 24]   [7]   =   [(6 * 7 + 24 * 4) % 26]   =   [7 * 7 + 24 * 4] % 26 = [78 % 26] = 0
  [1, 13]   [4]      [(1 * 7 + 13 * 4) % 26]   =   [1 * 7 + 13 * 4] % 26 = [59 % 26] = 7
  ```

  This gives the ciphertext vector: [0, 7], which corresponds to the letters **A** and **H**.

#### **Encryption of Block 2:**
- Similarly, multiply the key matrix by the next plaintext block:

  ```
  [6, 24]   [11]   =   [(6 * 11 + 24 * 11) % 26]   =   [66 % 26] = 14
  [1, 13]   [11]   =   [(1 * 11 + 13 * 11) % 26]   =   [154 % 26] = 22
  ```

  This gives the ciphertext vector: [14, 22], which corresponds to the letters **O** and **W**.

#### **Encryption of Block 3:**
- Multiply the key matrix by the padded plaintext vector:

  ```
  [6, 24]   [14]   =   [(6 * 14 + 24 * 23) % 26]   =   [6 * 14 + 24 * 23] % 26 = [618 % 26] = 16
  [1, 13]   [23]   =   [(1 * 14 + 13 * 23) % 26]   =   [1 * 14 + 13 * 23] % 26 = [305 % 26] = 17
  ```

  This gives the ciphertext vector: [16, 17], which corresponds to the letters **Q** and **R**.

#### **Ciphertext:**  
   Combining all the blocks together, the ciphertext is:
   ```
   AHWQR
   ```

---

### **Decryption of Hill Cipher**

To decrypt a message encrypted with the Hill cipher, the process is the reverse of encryption. The key matrix used for encryption is needed for decryption. The decryption steps are as follows:

1. **Find the Inverse of the Key Matrix:**
   For decryption, the key matrix must be invertible. The inverse of the key matrix is calculated modulo 26 (the number of letters in the alphabet). If the key matrix is not invertible, decryption cannot be performed.

2. **Matrix Multiplication for Decryption:**
   Each ciphertext block is multiplied by the inverse of the key matrix. This will result in the original plaintext block.

3. **Convert the Numbers to Letters:**
   After the matrix multiplication, convert the resulting numbers back into letters.

---

### **Python Implementation of Hill Cipher**

Here is a Python implementation of the **Hill Cipher** encryption and decryption:

```python
import numpy as np

# Function to get the modular inverse of a number modulo 26
def mod_inverse(a, m):
    for i in range(1, m):
        if (a * i) % m == 1:
            return i
    return -1

# Function to calculate the inverse of a matrix modulo 26
def matrix_inverse(matrix):
    determinant = int(np.round(np.linalg.det(matrix)))
    determinant_inv = mod_inverse(determinant, 26)
    if determinant_inv == -1:
        return None
    matrix_adj = np.round(np.linalg.inv(matrix) * determinant).astype(int) % 26
    return (determinant_inv * matrix_adj) % 26

# Function to encrypt using the Hill cipher
def hill_encrypt(plaintext, key_matrix):
    # Convert the plaintext to numbers
    plaintext = plaintext.replace(" ", "").upper()
    plaintext_numbers = [ord(char) - 65 for char in plaintext]
    
    # Make sure the length of plaintext is even
    if len(plaintext_numbers) % 2 != 0:
        plaintext_numbers.append(ord('X') - 65)
    
    ciphertext_numbers = []
    # Encrypt in blocks
    for i in range(0, len(plaintext_numbers), 2):
        block = np.array(plaintext_numbers[i:i+2]).reshape((2, 1))
        encrypted_block = np.dot(key_matrix, block) % 26
        ciphertext_numbers.extend(encrypted_block.flatten())

    # Convert the ciphertext back to letters
    ciphertext = ''.join([chr(num + 65) for num in ciphertext_numbers])
    return ciphertext

# Function to decrypt using the Hill cipher
def hill_decrypt(ciphertext, key_matrix):
    # Calculate the inverse of the key matrix
    key_matrix_inv = matrix_inverse(key_matrix)
    if key_matrix_inv is None:
        return "Key matrix is not invertible"
    
    # Convert the ciphertext to numbers
    ciphertext = ciphertext.replace(" ", "").upper()
    ciphertext_numbers = [ord(char) - 65 for char in ciphertext]
    
    plaintext_numbers = []
    # Decrypt in blocks
    for i in range(0, len(ciphertext_numbers), 2):
        block = np.array(ciphertext_numbers[i:i+2]).reshape((2, 1))
        decrypted_block = np.dot(key_matrix_inv, block) % 26
        plaintext_numbers.extend(decrypted_block.flatten())
    
    # Convert the plaintext back to letters
    plaintext = ''.join([chr(num + 65) for num in plaintext_numbers])
    return plaintext

# Example usage
key_matrix = np.array([[6, 24], [1, 13]])  # Key matrix for encryption and decryption
plaintext = "HELLO"

ciphertext = hill_encrypt(plaintext, key_matrix)
print(f"Encrypted: {ciphertext}")

decrypted = hill_decrypt(ciphertext, key_matrix)
print(f"Decrypted: {decrypted}")
```

#### **Example Output:**

```
Encrypted: AHWQR
Decrypted: HELLO
```

---

### **Advantages of Hill Cipher**
1. **Stronger Security**: Compared to simple substitution ciphers, the Hill cipher encrypts multiple letters at a time, making it much more secure.


2. **Mathematical Foundation**: The use of linear algebra provides a mathematical approach to encryption, making it more resistant to cryptanalysis than simpler ciphers.

### **Drawbacks of Hill Cipher**
1. **Key Size**: The Hill cipher requires a square matrix key, and the larger the matrix, the more complex the encryption and decryption process become.
2. **Vulnerable to Known-Plaintext Attacks**: If an attacker has access to both the ciphertext and the corresponding plaintext, they can easily break the cipher by solving for the key matrix.
3. **Key Matrix Inversibility**: If the key matrix is not invertible modulo 26, decryption is not possible.

---

### **Conclusion**
The **Hill Cipher** is a significant cryptographic technique that uses matrix multiplication for encryption. While more secure than simpler ciphers, it has its limitations, such as vulnerability to known-plaintext attacks and the requirement for an invertible key matrix. Nonetheless, it serves as an excellent example of early cryptographic techniques and is still valuable for learning cryptography.
