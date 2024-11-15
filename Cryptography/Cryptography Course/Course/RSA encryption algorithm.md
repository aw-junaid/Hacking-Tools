### **RSA Encryption**

The **RSA encryption** algorithm is an asymmetric cryptographic technique used to secure data and communications. It was developed by **Ron Rivest**, **Adi Shamir**, and **Leonard Adleman** in 1977. Unlike symmetric encryption, where the same key is used for both encryption and decryption, RSA uses a pair of keys: a **public key** for encryption and a **private key** for decryption. This public/private key system makes RSA suitable for secure communication and digital signatures.

#### **Steps in RSA Encryption**

1. **Key Generation**: 
   - First, two large prime numbers \( p \) and \( q \) are selected.
   - These numbers are multiplied to produce a modulus \( n \) used in both the public and private keys.
   - The totient $\( \phi(n) \)$ is calculated using the formula $\( \phi(n) = (p-1)(q-1) \)$.
   - A public exponent \( e \) is chosen such that \( e \) is coprime with $\( \phi(n) \)$ (i.e., their greatest common divisor is 1).
   - The private exponent \( d \) is computed as the modular multiplicative inverse of \( e \) modulo $\( \phi(n) \)$.

   The **public key** consists of the pair \( (n, e) \), and the **private key** consists of \( (n, d) \).

2. **Encryption**:
   - The plaintext message is represented as a number \( M \), where $\( M < n \)$.
   - To encrypt the message \( M \), it is raised to the power of \( e \) and then taken modulo \( n \):
     $\[
     C = M^e \ (\text{mod} \ n)
     \]$
   - The resulting ciphertext \( C \) is then transmitted or stored.

3. **Decryption**:
   - The recipient uses their private key \( (n, d) \) to decrypt the ciphertext.
   - The ciphertext \( C \) is raised to the power of \( d \) and then taken modulo \( n \):
     $\[
     M = C^d \ (\text{mod} \ n)
     \]$
   - The result is the original plaintext message \( M \).

### **RSA Example**

Let's walk through a simplified example to illustrate the RSA encryption process.

1. **Choose two primes**:
   - $\( p = 61 \)$
   - $\( q = 53 \)$

2. **Compute \( n \) and $\( \phi(n) \)$**:
   - $\( n = p \times q = 61 \times 53 = 3233 \)$
   - $\( \phi(n) = (p - 1)(q - 1) = 60 \times 52 = 3120 \)$

3. **Choose public exponent \( e \)**:
   - \( e = 17 \) (a common choice that is coprime with $\( \phi(n) \))$.

4. **Compute private exponent \( d \)**:
   - \( d \) is the modular inverse of $\( e \)$ modulo $\( \phi(n) \)$.
   - Using the extended Euclidean algorithm, we find $\( d = 2753 \)$.

   So, the **public key** is $\( (n, e) = (3233, 17) \)$ and the **private key** is $\( (n, d) = (3233, 2753) \)$.

#### **Encryption**

Let’s encrypt a message using the public key:

- Suppose the plaintext message $\( M = 65 \)$.

- To encrypt the message, we calculate the ciphertext \( C \) using the formula:
  $\[
  C = M^e \ (\text{mod} \ n) = 65^{17} \ (\text{mod} \ 3233) = 2790
  \]$

So, the ciphertext $\( C = 2790 \)$.

#### **Decryption**

To decrypt the ciphertext, the recipient uses their private key \( (n, d) = (3233, 2753) \):

- We decrypt by calculating:
  $\[
  M = C^d \ (\text{mod} \ n) = 2790^{2753} \ (\text{mod} \ 3233) = 65
  \]$

The decrypted message $\( M = 65 \)$, which matches the original plaintext.

### **RSA Encryption Code in Python**

Here’s a basic Python implementation of RSA encryption and decryption:

```python
# Function to compute gcd of two numbers
def gcd(a, b):
    while b:
        a, b = b, a % b
    return a

# Function to find modular inverse
def mod_inverse(a, m):
    m0, x0, x1 = m, 0, 1
    if m == 1:
        return 0
    while a > 1:
        q = a // m
        m, a = a % m, m
        x0, x1 = x1 - q * x0, x0
    if x1 < 0:
        x1 += m0
    return x1

# RSA Key Generation
def generate_keys():
    # Select two prime numbers
    p = 61
    q = 53
    n = p * q
    phi_n = (p - 1) * (q - 1)

    # Choose public exponent e
    e = 17  # Commonly used value

    # Compute private exponent d
    d = mod_inverse(e, phi_n)

    return ((n, e), (n, d))

# RSA Encryption
def encrypt(plaintext, public_key):
    n, e = public_key
    return [pow(ord(char), e, n) for char in plaintext]

# RSA Decryption
def decrypt(ciphertext, private_key):
    n, d = private_key
    return ''.join([chr(pow(char, d, n)) for char in ciphertext])

# Example usage
public_key, private_key = generate_keys()

# Encrypt a message
plaintext = "HELLO"
ciphertext = encrypt(plaintext, public_key)
print(f"Encrypted: {ciphertext}")

# Decrypt the message
decrypted_message = decrypt(ciphertext, private_key)
print(f"Decrypted: {decrypted_message}")
```

### **Explanation of the Code**

1. **Key Generation**:
   - The `generate_keys` function selects two prime numbers, calculates \( n \), \( \phi(n) \), the public exponent \( e \), and computes the private exponent \( d \).

2. **Encryption**:
   - The `encrypt` function takes a plaintext message and encrypts it using the public key. It converts each character to its corresponding ASCII value, encrypts it, and returns the ciphertext.

3. **Decryption**:
   - The `decrypt` function decrypts the ciphertext using the private key. It computes the modular exponentiation of each character in the ciphertext and converts it back to the original character.

### **RSA Security**

- **RSA’s security** relies on the fact that, while it is easy to multiply large prime numbers together, it is computationally difficult to factor the resulting number back into its prime components (i.e., factoring \( n \) back into \( p \) and \( q \)).
- **Key size** plays a crucial role in the security of RSA. The larger the modulus \( n \), the stronger the encryption.
- **Quantum computers** could potentially break RSA, as they could factor large numbers efficiently using **Shor's algorithm**. However, this is a theoretical concern, and RSA is still secure for current computational capabilities.

---

### **Applications of RSA Encryption**

1. **Digital Signatures**: RSA is commonly used in digital signature schemes, where a message is signed using the sender's private key and verified by the recipient using the sender's public key.
  
2. **Secure Communications**: RSA is often used to establish a secure channel between two parties by exchanging keys in a secure manner (e.g., in protocols like **SSL/TLS** for HTTPS).

3. **Public Key Infrastructure (PKI)**: RSA is a key component of PKI systems, where it is used to create and verify digital certificates, enabling secure identification and authentication.

---

### **Conclusion**

RSA encryption is a foundational cryptographic method that plays a critical role in securing digital communications. It uses public-key cryptography, where the public key is used for encryption and the private key is used for decryption. Despite its security, RSA is vulnerable to quantum computers, and thus, post-quantum cryptography is a topic of active research.
