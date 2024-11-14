### **Cryptography - Vigenère Cipher**

The **Vigenère Cipher** is a more advanced form of the **Caesar Cipher** and is classified as a **polyalphabetic cipher**. Unlike a Caesar cipher, which shifts each letter of the plaintext by the same fixed number, the Vigenère cipher uses a keyword (repeated as necessary) to shift each letter of the plaintext by different values, making it more secure than simple substitution ciphers.

The **Vigenère Cipher** was invented by the French diplomat **Blaise de Vigenère** in the 16th century and is known as one of the most secure ciphers of its time. It became widely used in military communication due to its strength over simple substitution methods.

### **How Vigenère Cipher Works**

1. **Key**: The Vigenère cipher requires a keyword, which is repeated over the plaintext to create a series of shifts. Each letter of the plaintext is shifted by the corresponding letter in the repeated keyword.
2. **Shifting Mechanism**: The shift is determined by the position of the letter in the alphabet. For example, if the keyword letter is **A**, there is no shift, while if it's **B**, the shift is by 1, if it's **C**, the shift is by 2, and so on.

#### Example of Vigenère Cipher Encryption:

Let's consider the following example:

- **Plaintext**: "HELLO"
- **Keyword**: "KEY"

To encrypt, we repeat the keyword to match the length of the plaintext:

- **Repeated Keyword**: "KEYKE"

Now, shift each letter of the plaintext according to the corresponding letter in the keyword:

- **H** (shifted by K): K is the 11th letter in the alphabet, so H is shifted by 10 positions (H + 10 → R).
- **E** (shifted by E): E is the 4th letter in the alphabet, so E is shifted by 4 positions (E + 4 → I).
- **L** (shifted by Y): Y is the 25th letter in the alphabet, so L is shifted by 24 positions (L + 24 → J).
- **L** (shifted by K): K is the 11th letter in the alphabet, so L is shifted by 10 positions (L + 10 → V).
- **O** (shifted by E): E is the 4th letter in the alphabet, so O is shifted by 4 positions (O + 4 → S).

The **ciphertext** is **"RIJVS"**.

### **Decryption Process of Vigenère Cipher**

To decrypt the ciphertext, the receiver must know the keyword. The decryption process involves reversing the encryption by shifting each letter of the ciphertext backwards by the corresponding value in the keyword.

#### Decryption Example:

Let's decrypt the ciphertext **"RIJVS"** using the keyword **"KEY"**:

- **R** (shifted by K): K is the 11th letter, so we shift R back by 10 positions (R - 10 → H).
- **I** (shifted by E): E is the 4th letter, so we shift I back by 4 positions (I - 4 → E).
- **J** (shifted by Y): Y is the 25th letter, so we shift J back by 24 positions (J - 24 → L).
- **V** (shifted by K): K is the 11th letter, so we shift V back by 10 positions (V - 10 → L).
- **S** (shifted by E): E is the 4th letter, so we shift S back by 4 positions (S - 4 → O).

Thus, the decrypted **plaintext** is **"HELLO"**.

### **Python Code for Vigenère Cipher (Encryption and Decryption)**

Here's a Python implementation of both the **Vigenère Cipher** encryption and decryption:

```python
# Function to encrypt the plaintext using Vigenère cipher
def encrypt_vigenere(plaintext, key):
    alphabet = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
    plaintext = plaintext.upper()  # Convert plaintext to uppercase
    key = key.upper()  # Convert key to uppercase
    ciphertext = ''
    key_index = 0  # Initialize the key index
    
    for char in plaintext:
        if char in alphabet:  # Encrypt only alphabetic characters
            plaintext_index = alphabet.index(char)
            key_index_value = alphabet.index(key[key_index % len(key)])
            ciphertext += alphabet[(plaintext_index + key_index_value) % 26]
            key_index += 1
        else:
            ciphertext += char  # Non-alphabet characters remain unchanged
    
    return ciphertext

# Function to decrypt the ciphertext using Vigenère cipher
def decrypt_vigenere(ciphertext, key):
    alphabet = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
    ciphertext = ciphertext.upper()  # Convert ciphertext to uppercase
    key = key.upper()  # Convert key to uppercase
    plaintext = ''
    key_index = 0  # Initialize the key index
    
    for char in ciphertext:
        if char in alphabet:  # Decrypt only alphabetic characters
            ciphertext_index = alphabet.index(char)
            key_index_value = alphabet.index(key[key_index % len(key)])
            plaintext += alphabet[(ciphertext_index - key_index_value) % 26]
            key_index += 1
        else:
            plaintext += char  # Non-alphabet characters remain unchanged
    
    return plaintext

# Example usage of encryption and decryption
key = 'KEY'
plaintext = 'HELLO'
ciphertext = encrypt_vigenere(plaintext, key)
print(f"Ciphertext: {ciphertext}")

decrypted_text = decrypt_vigenere(ciphertext, key)
print(f"Decrypted Text: {decrypted_text}")
```

### **Explanation of the Code:**

1. **`encrypt_vigenere` function**: This function takes the plaintext and the key as inputs. It loops over each character of the plaintext:
   - For alphabetic characters, it finds their positions in the alphabet and shifts them by the corresponding letter in the key.
   - The key is repeated if necessary to match the length of the plaintext.
   - Non-alphabet characters (such as spaces or punctuation) are left unchanged.
   
2. **`decrypt_vigenere` function**: This function takes the ciphertext and the key as inputs and reverses the process. It shifts the ciphertext letters backwards using the key's corresponding letter.

3. **Example**: In this example, the key is `"KEY"`, and the plaintext is `"HELLO"`. The function `encrypt_vigenere` encrypts the message, and `decrypt_vigenere` decrypts the ciphertext back to the original plaintext.

### **Example Output:**

```
Ciphertext: RIJVS
Decrypted Text: HELLO
```

### **Advantages of Vigenère Cipher:**

1. **Stronger than Caesar Cipher**: The Vigenère cipher is much stronger than the Caesar cipher because it uses multiple shifts based on a keyword.
2. **Polyalphabetic**: It eliminates the weaknesses of the Caesar cipher by using a different shift for each letter, making frequency analysis more difficult.
3. **Flexible Key Length**: The length of the key can be varied, allowing for a larger key space and higher security.

### **Drawbacks of Vigenère Cipher:**

1. **Key Length Limitation**: If the keyword is short, it may repeat often, which can make the cipher vulnerable to attacks like the **Kasiski examination** or **frequency analysis** if the keyword length is discovered.
2. **Keyword Security**: The security of the Vigenère cipher depends on the secrecy of the key. If the key is compromised, the cipher can be easily broken.
3. **Manual Key Management**: In practice, managing and securely sharing long, random keys can be challenging.

### **Conclusion**

The **Vigenère Cipher** is a more secure encryption technique compared to the Caesar Cipher, as it uses multiple shifts based on a keyword, making it resistant to simple frequency analysis. While it provides better security for the time it was used, it still has weaknesses, especially when the key is short or reused. Modern cryptographic systems have moved beyond the Vigenère cipher, but it remains an important historical example of a polyalphabetic cipher.
