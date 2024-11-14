### **Cryptography - Decryption of Simple Substitution Cipher**

Decryption of the **Simple Substitution Cipher** involves reversing the encryption process. Since the substitution cipher is based on a one-to-one mapping between the plaintext and ciphertext letters, decryption simply requires mapping the ciphertext back to the original plaintext using the inverse of the key used for encryption.

### **Decryption Process for Simple Substitution Cipher**

To decrypt a message encrypted with a simple substitution cipher, the following steps are followed:

1. **Ciphertext**: This is the encrypted message you want to decrypt.
2. **Key**: A scrambled version of the alphabet (used for encryption).
3. **Inverse Mapping**: To decrypt, we reverse the substitution process by mapping each letter from the ciphertext back to the corresponding letter from the plaintext alphabet.

#### Example

Let's say the key used for encryption is:

| Plaintext | A | B | C | D | E | F | G | H | I | J | K | L | M | N | O | P | Q | R | S | T | U | V | W | X | Y | Z |
|-----------|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| Ciphertext| Q | W | E | R | T | Y | U | I | O | P | A | S | D | F | G | H | J | K | L | Z | X | C | V | B | N | M |

If the ciphertext is **"ITSSL"**, the decryption process would involve reversing the mapping:

- I → H
- T → E
- S → L
- S → L
- L → O

Thus, the plaintext after decryption is **"HELLO"**.

### **Python Code for Decryption of Simple Substitution Cipher**

Here is a Python implementation to decrypt a message using the **Simple Substitution Cipher**:

```python
# Function to decrypt the ciphertext using a substitution cipher
def decrypt_substitution(ciphertext, key):
    alphabet = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
    ciphertext = ciphertext.upper()  # Convert ciphertext to uppercase
    plaintext = ''
    
    # Create a reverse key for decryption
    reverse_key = {key[i]: alphabet[i] for i in range(len(alphabet))}
    
    for char in ciphertext:
        if char in key:  # Decrypt only alphabetic characters
            plaintext += reverse_key[char]  # Use reverse key to find the plaintext
        else:
            plaintext += char  # Non-alphabet characters remain unchanged
    return plaintext

# Example of decryption
key = 'QWERTYUIOPASDFGHJKLZXCVBNM'  # The scrambled alphabet used as the key

ciphertext = "ITSSL"
plaintext = decrypt_substitution(ciphertext, key)
print(f"Plaintext: {plaintext}")
```

### **Explanation of the Code:**

1. **`decrypt_substitution` function**: This function takes the ciphertext and the key as inputs. It creates a reverse mapping of the key (from ciphertext to plaintext) using a dictionary. Then, for each letter in the ciphertext:
   - It replaces the ciphertext letter with the corresponding letter from the plaintext alphabet using the reverse key.
   - Non-alphabet characters (like spaces or punctuation) are kept unchanged.

2. **Example usage**: The key is `"QWERTYUIOPASDFGHJKLZXCVBNM"`, and the ciphertext is `"ITSSL"`. The function `decrypt_substitution` is called to decrypt the message.

### **Example Output:**

```
Plaintext: HELLO
```

### **Decryption Explanation for the Example:**

Using the reverse key:

| Ciphertext | I | T | S | S | L |
|------------|---|---|---|---|---|
| Plaintext  | H | E | L | L | O |

Thus, the ciphertext **"ITSSL"** is decrypted to **"HELLO"**.

### **Advantages of Decryption of Simple Substitution Cipher:**

1. **Simple and Fast**: The decryption process is as simple and fast as encryption. It just requires reversing the substitution using the key.
2. **Easy to Implement**: Since it's just a simple letter-to-letter substitution, the decryption algorithm is very easy to implement.

### **Drawbacks of Decryption of Simple Substitution Cipher:**

1. **Key Dependency**: Decryption is only possible if the receiver knows the key (the scrambled alphabet). If the key is compromised, the encryption can easily be broken.
2. **Vulnerable to Frequency Analysis**: The ciphertext still maintains the same structure as the plaintext. For example, if a certain letter (such as 'E' in English) appears often in the plaintext, it will appear frequently in the ciphertext as well. This makes the cipher vulnerable to frequency analysis attacks, where an attacker can use the frequency of letters to guess the key.

### **Conclusion**

Decryption of the **Simple Substitution Cipher** involves reversing the substitution process by using the inverse of the key used during encryption. While the decryption process is straightforward and quick, the cipher is insecure by modern standards due to its vulnerability to frequency analysis. More advanced encryption techniques are generally used for securing sensitive information.
