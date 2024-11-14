### **Public Key Encryption in Cryptography**

**Public Key Encryption** (also known as **Asymmetric Encryption**) uses two keys: a **public key** for encryption and a **private key** for decryption. The public key is shared openly, while the private key remains secret. This type of encryption is widely used in modern cryptography to secure data, establish secure communication channels, and verify digital signatures.

### **How Public Key Encryption Works**

1. **Encryption**:
   - The sender encrypts the message using the recipient’s public key.
2. **Decryption**:
   - The recipient decrypts the message using their private key.

Because only the recipient knows their private key, only they can decrypt the message, ensuring confidentiality.

### **Public Key Encryption Example: RSA (Rivest-Shamir-Adleman)**

**RSA** is one of the most widely used public key encryption systems. In RSA:
- A pair of keys (public and private) are generated.
- The public key is used for encryption, and the private key is used for decryption.

### **Python Code Example: RSA Public Key Encryption**

To demonstrate public key encryption in Python, we will use the **`cryptography`** library, which provides high-level cryptographic recipes and primitives. Below is an example of how to generate keys and encrypt/decrypt messages using RSA.

#### 1. **Install the `cryptography` library**

First, install the cryptography library (if you don’t have it installed):

```bash
pip install cryptography
```

#### 2. **Code for RSA Public Key Encryption and Decryption**

```python
from cryptography.hazmat.primitives.asymmetric import rsa
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives.asymmetric import padding
from cryptography.hazmat.primitives import serialization

# Step 1: Generate RSA Public and Private Keys
def generate_rsa_keys():
    private_key = rsa.generate_private_key(
        public_exponent=65537,
        key_size=2048,
    )
    public_key = private_key.public_key()

    # Serialize keys for storage or sharing
    private_pem = private_key.private_bytes(
        encoding=serialization.Encoding.PEM,
        format=serialization.PrivateFormat.TraditionalOpenSSL,
        encryption_algorithm=serialization.NoEncryption()
    )
    
    public_pem = public_key.public_bytes(
        encoding=serialization.Encoding.PEM,
        format=serialization.PublicFormat.SubjectPublicKeyInfo
    )

    return private_pem, public_pem, private_key, public_key

# Step 2: Encrypt a Message with Public Key
def encrypt_message(public_key, message):
    encrypted = public_key.encrypt(
        message.encode(),  # Convert the message to bytes
        padding.OAEP(
            mgf=padding.MGF1(algorithm=hashes.SHA256()),
            algorithm=hashes.SHA256(),
            label=None
        )
    )
    return encrypted

# Step 3: Decrypt the Message with Private Key
def decrypt_message(private_key, encrypted_message):
    decrypted = private_key.decrypt(
        encrypted_message,
        padding.OAEP(
            mgf=padding.MGF1(algorithm=hashes.SHA256()),
            algorithm=hashes.SHA256(),
            label=None
        )
    )
    return decrypted.decode()  # Convert bytes back to string

# Generate RSA Keys
private_pem, public_pem, private_key, public_key = generate_rsa_keys()

# Print Public and Private Keys (just for demonstration)
print("Public Key:\n", public_pem.decode())
print("Private Key:\n", private_pem.decode())

# Step 4: Encrypt and Decrypt a Message
message = "This is a secret message."
print("\nOriginal Message:", message)

# Encrypt the message using the public key
encrypted_message = encrypt_message(public_key, message)
print("\nEncrypted Message:", encrypted_message)

# Decrypt the message using the private key
decrypted_message = decrypt_message(private_key, encrypted_message)
print("\nDecrypted Message:", decrypted_message)
```

### **Explanation of the Code**:

1. **Key Generation**:
   - The `rsa.generate_private_key()` method generates a private key.
   - The `public_key()` method extracts the corresponding public key from the private key.
   
2. **Encryption**:
   - The `encrypt_message()` function takes the public key and a plaintext message, encrypts it using **OAEP padding** and **SHA-256** for hashing. OAEP is a secure padding scheme used in RSA encryption.
   
3. **Decryption**:
   - The `decrypt_message()` function decrypts the message using the private key and the same padding scheme.

4. **Serialization**:
   - The keys are serialized to PEM format, which is a widely used format for encoding keys, making it easy to share or store them.
   
5. **Testing**:
   - After generating the keys, we encrypt a simple message and then decrypt it to demonstrate the process.

---

### **Output Example**:

```plaintext
Public Key:
-----BEGIN PUBLIC KEY-----
MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA...
-----END PUBLIC KEY-----
Private Key:
-----BEGIN PRIVATE KEY-----
MIIEowIBAAKCAQEA...
-----END PRIVATE KEY-----

Original Message: This is a secret message.

Encrypted Message: b'\x9c\x17\xfa\xf9\x9c....'

Decrypted Message: This is a secret message.
```

### **Security Considerations**:

1. **Key Size**: In the example, we used a 2048-bit RSA key size, which is considered secure. However, for highly sensitive data, larger key sizes may be necessary (e.g., 4096-bit keys).
   
2. **Padding**: The **OAEP (Optimal Asymmetric Encryption Padding)** scheme used here ensures that RSA encryption is secure and mitigates certain types of attacks like the **chosen ciphertext attack**.

3. **Performance**: RSA encryption is computationally expensive, which is why it's often combined with symmetric key encryption (as in **hybrid cryptosystems** like TLS/SSL).

### **Conclusion**:

Public key encryption, such as RSA, provides secure communication over insecure channels by using a pair of keys: a public key for encryption and a private key for decryption. The example above demonstrates how to generate RSA keys, encrypt data with the public key, and decrypt data with the private key using Python.
