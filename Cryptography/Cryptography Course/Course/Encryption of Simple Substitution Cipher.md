### **Cryptography - Encryption of Simple Substitution Cipher**

The **Simple Substitution Cipher** is a type of substitution cipher where each letter of the plaintext is replaced by another letter from a predefined set, according to a key. The key is typically a scrambled version of the alphabet, where each letter of the alphabet is replaced by another letter, and the key must be kept secret to maintain the encryption's security.

### **Encryption Process for Simple Substitution Cipher**

The encryption process of the Simple Substitution Cipher can be described in the following steps:

1. **Plaintext**: This is the original message you want to encrypt.
2. **Key**: A key that represents a scrambled version of the alphabet. This key is used to map each letter of the plaintext to a different letter.
3. **Mapping**: For each letter in the plaintext, the corresponding letter from the key is used to form the ciphertext.

#### Example

Let's say we have the following **key** for the Simple Substitution Cipher, which is a scrambled version of the alphabet:

| Plaintext | A | B | C | D | E | F | G | H | I | J | K | L | M | N | O | P | Q | R | S | T | U | V | W | X | Y | Z |
|-----------|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| Ciphertext| Q | W | E | R | T | Y | U | I | O | P | A | S | D | F | G | H | J | K | L | Z | X | C | V | B | N | M |

This key means:
- A is substituted with Q
- B is substituted with W
- C is substituted with E
- and so on...

Now, let's take the plaintext message **"HELLO"** and encrypt it using the substitution cipher.

#### Step 1: Assign Numerical Values to Letters

First, we convert each letter of the plaintext **"HELLO"** to its corresponding letter in the ciphertext:

- H → I
- E → T
- L → S
- L → S
- O → L

Thus, the ciphertext for the plaintext **"HELLO"** is **"ITSSL"**.

### **Python Code for Encryption of Simple Substitution Cipher**

Here’s a Python implementation to encrypt a message using the Simple Substitution Cipher:

```python
# Function to encrypt the plaintext using a substitution cipher
def encrypt_substitution(plaintext, key):
    alphabet = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
    plaintext = plaintext.upper()  # Convert plaintext to uppercase
    ciphertext = ''
    
    for char in plaintext:
        if char in alphabet:  # Encrypt only alphabetic characters
            index = alphabet.index(char)
            ciphertext += key[index]
        else:
            ciphertext += char  # Non-alphabet characters remain unchanged
    return ciphertext

# Example of encryption
key = 'QWERTYUIOPASDFGHJKLZXCVBNM'  # The scrambled alphabet used as the key

plaintext = "HELLO"
ciphertext = encrypt_substitution(plaintext, key)
print(f"Ciphertext: {ciphertext}")
```

### **Explanation of the Code:**

1. **`encrypt_substitution` function**: This function takes the plaintext and the key as inputs. It loops through each character of the plaintext:
   - If the character is an alphabet letter, it finds the corresponding letter in the key (using the index of the letter in the alphabet).
   - If the character is not an alphabet letter, it remains unchanged (e.g., spaces or punctuation).

2. **Example usage**: In this example, the key is `"QWERTYUIOPASDFGHJKLZXCVBNM"`, and the plaintext is `"HELLO"`. The function `encrypt_substitution` is called to encrypt the message.

### **Example Output:**

```
Ciphertext: ITSSL
```

### **Encryption Explanation for the Example:**

Using the key:

| Plaintext | H | E | L | L | O |
|-----------|---|---|---|---|---|
| Ciphertext| I | T | S | S | L |

Thus, **"HELLO"** is encrypted as **"ITSSL"**.

### **Advantages of the Simple Substitution Cipher Encryption:**

1. **Simple to Implement**: The encryption algorithm is very straightforward to implement, as it is simply a replacement of each letter in the plaintext with a corresponding letter from the key.
2. **Fast**: The encryption process is quick because it only involves indexing and substitution.

### **Drawbacks of the Simple Substitution Cipher Encryption:**

1. **Vulnerable to Frequency Analysis**: The main weakness of this cipher is that it can be easily broken by frequency analysis. In natural languages, some letters (such as 'E', 'T', and 'A') appear more frequently than others. By analyzing the frequency of letters in the ciphertext and comparing them with known letter frequencies in the language, an attacker can decrypt the message.
2. **Key Security**: The security of this cipher depends entirely on the secrecy of the key. If the key is compromised, the cipher can be easily broken.

### **Conclusion**

The **Simple Substitution Cipher** is a basic encryption technique where each letter of the plaintext is replaced by another letter from a scrambled alphabet. Although it is easy to implement and understand, it is not suitable for securing sensitive information due to its vulnerability to frequency analysis. For modern cryptographic needs, more sophisticated encryption methods, such as **transposition ciphers** or **block ciphers**, are typically used.
