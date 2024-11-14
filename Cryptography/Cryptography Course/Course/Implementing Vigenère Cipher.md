### **Cryptography - Implementing Vigenère Cipher**

The **Vigenère Cipher** is one of the most widely known **polyalphabetic ciphers**. It involves shifting each letter of the plaintext by an amount corresponding to the letter in the keyword. The Vigenère Cipher is significantly stronger than simpler ciphers like the Caesar Cipher because it uses multiple shifts depending on the keyword.

### **Steps to Implement Vigenère Cipher**

1. **Prepare the plaintext and keyword**:
   - The keyword is repeated to match the length of the plaintext.
   
2. **Encryption Process**:
   - Each letter of the plaintext is shifted based on the corresponding letter of the keyword. The shift is determined by the position of the letter in the alphabet (e.g., A = 0, B = 1, ..., Z = 25).
   
3. **Decryption Process**:
   - To decrypt, the reverse of the encryption process is applied by shifting each letter of the ciphertext back by the corresponding keyword letter.

### **Encryption Process for Vigenère Cipher**
For a plaintext letter, find the position of that letter and the corresponding letter in the keyword. Add their respective positions in the alphabet, modulo 26. This gives the encrypted letter.

### **Decryption Process for Vigenère Cipher**
For a ciphertext letter, subtract the position of the corresponding letter in the keyword from the ciphertext letter, modulo 26, to get the plaintext letter.

### **Vigenère Cipher Implementation in Python**

Let's write the Python code to implement both encryption and decryption of the Vigenère cipher.

#### 1. **Vigenère Cipher Encryption**

```python
def encrypt_vigenere(plaintext, key):
    alphabet = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
    plaintext = plaintext.upper().replace(" ", "")  # Convert to uppercase and remove spaces
    key = key.upper().replace(" ", "")  # Convert to uppercase and remove spaces
    ciphertext = ''
    key_index = 0  # Initialize the key index

    for char in plaintext:
        if char in alphabet:  # Encrypt only alphabetic characters
            # Calculate the position in the alphabet for both plaintext letter and key letter
            plaintext_index = alphabet.index(char)
            key_index_value = alphabet.index(key[key_index % len(key)])
            # Shift the plaintext letter based on the key letter
            new_char = alphabet[(plaintext_index + key_index_value) % 26]
            ciphertext += new_char
            key_index += 1  # Move to the next letter of the key
        else:
            ciphertext += char  # Non-alphabet characters remain unchanged

    return ciphertext
```

#### 2. **Vigenère Cipher Decryption**

```python
def decrypt_vigenere(ciphertext, key):
    alphabet = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
    ciphertext = ciphertext.upper().replace(" ", "")  # Convert to uppercase and remove spaces
    key = key.upper().replace(" ", "")  # Convert to uppercase and remove spaces
    plaintext = ''
    key_index = 0  # Initialize the key index

    for char in ciphertext:
        if char in alphabet:  # Decrypt only alphabetic characters
            # Calculate the position in the alphabet for both ciphertext letter and key letter
            ciphertext_index = alphabet.index(char)
            key_index_value = alphabet.index(key[key_index % len(key)])
            # Reverse the shift by the key letter to get the plaintext letter
            new_char = alphabet[(ciphertext_index - key_index_value) % 26]
            plaintext += new_char
            key_index += 1  # Move to the next letter of the key
        else:
            plaintext += char  # Non-alphabet characters remain unchanged

    return plaintext
```

### **Example of Using the Vigenère Cipher**

Let's take an example where we will:
1. Encrypt a plaintext using the keyword.
2. Decrypt the ciphertext back to the original plaintext.

#### Example:

- **Plaintext**: "HELLO WORLD"
- **Keyword**: "KEY"

```python
# Example usage
plaintext = "HELLO WORLD"
keyword = "KEY"

# Encrypt the plaintext
ciphertext = encrypt_vigenere(plaintext, keyword)
print(f"Ciphertext: {ciphertext}")

# Decrypt the ciphertext
decrypted_text = decrypt_vigenere(ciphertext, keyword)
print(f"Decrypted Text: {decrypted_text}")
```

### **Expected Output:**

```
Ciphertext: RIJVS UYVJN
Decrypted Text: HELLO WORLD
```

### **How the Code Works:**

1. **Encryption (`encrypt_vigenere`)**:
   - The `plaintext` and `key` are converted to uppercase and spaces are removed for consistency.
   - Each letter in the plaintext is encrypted by shifting it according to the corresponding letter in the key.
   - The key is repeated if necessary to match the length of the plaintext.
   - The ciphertext is returned after all the shifts are applied.

2. **Decryption (`decrypt_vigenere`)**:
   - The `ciphertext` and `key` are also converted to uppercase.
   - Each letter in the ciphertext is decrypted by reversing the shift using the corresponding letter of the key.
   - The plaintext is returned after all the shifts are reversed.

### **Explanation of the Example:**

- **Plaintext**: "HELLO WORLD"
- **Keyword**: "KEY"
- The keyword is repeated to match the length of the plaintext, becoming "KEYKEY KEYKE".
  
Each letter of the plaintext is shifted by the corresponding letter in the keyword:
- **H** → **R** (shifted by K)
- **E** → **I** (shifted by E)
- **L** → **J** (shifted by Y)
- **L** → **V** (shifted by K)
- **O** → **S** (shifted by E)
- And so on...

This results in the ciphertext **"RIJVS UYVJN"**.

For decryption, the process is reversed by shifting each letter of the ciphertext back according to the corresponding letter in the keyword. The result is the original **"HELLO WORLD"**.

### **Advantages of Vigenère Cipher:**

1. **More Secure Than Caesar Cipher**: The Vigenère cipher is significantly more secure than the Caesar cipher because it uses multiple shifts depending on the keyword, which makes frequency analysis more difficult.
2. **Flexible Key Length**: The length of the key can vary, providing more options for securing messages.

### **Drawbacks of Vigenère Cipher:**

1. **Key Reuse Weakness**: If the keyword is short or repeated frequently, the cipher can still be vulnerable to attacks like **Kasiski Examination**.
2. **Keyword Management**: The security of the Vigenère cipher depends on the secrecy of the key. If the key is intercepted or guessed, the cipher can be easily broken.

### **Conclusion**

The **Vigenère Cipher** provides better security compared to simpler ciphers like the Caesar Cipher. By using a keyword to apply multiple shifts to the plaintext, it becomes much harder to break using frequency analysis. However, it is not immune to more advanced cryptographic analysis, and in modern cryptography, more advanced algorithms like AES (Advanced Encryption Standard) are used for secure communication.
