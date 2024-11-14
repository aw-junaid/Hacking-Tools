### **Cryptography - AES Structure**

The **Advanced Encryption Standard (AES)** structure is based on a symmetric block cipher that transforms a fixed-length input (plaintext) into ciphertext through a series of carefully designed transformations. AES operates on a 128-bit block size and offers three possible key lengths: 128, 192, and 256 bits. The key length determines the number of encryption rounds:

- **AES-128:** 10 rounds
- **AES-192:** 12 rounds
- **AES-256:** 14 rounds

AES encryption and decryption involve multiple transformations, which are designed to introduce confusion and diffusion, properties essential for secure encryption.

---

### **AES Structure Overview**

1. **Data Representation (State):**
   - AES operates on a 4x4 matrix of bytes, known as the **state**. Each element in this matrix represents a byte of data, allowing AES to process data in blocks of 128 bits (16 bytes).
   - During encryption, the state undergoes a series of transformations involving substitution, shifting, mixing, and key addition.

2. **Initial Round:** The plaintext is first combined with the encryption key using a process called **AddRoundKey**. This operation is performed before any other transformations, creating an initial level of security.

3. **Rounds:** The main structure of AES consists of a series of rounds that vary in number based on the key size. Each round applies four transformations to the state:
   - **SubBytes**
   - **ShiftRows**
   - **MixColumns** (skipped in the final round)
   - **AddRoundKey**

4. **Final Round:** In the final round, the **MixColumns** step is omitted, leaving only the **SubBytes**, **ShiftRows**, and **AddRoundKey** transformations. This maintains the balance of security and efficiency while finalizing the encryption.

---

### **AES Transformations**

Each round of AES (except for the final round) involves the following transformations:

1. **SubBytes (Byte Substitution):**
   - Each byte in the state is replaced with a corresponding byte from a fixed 16x16 **S-box (substitution box)**, which introduces **non-linearity** into the encryption. 
   - The S-box is designed to thwart cryptographic attacks by providing resistance to linear and differential cryptanalysis.

2. **ShiftRows:**
   - Each row of the state matrix is **circularly shifted** to the left by a different offset. Specifically:
     - The first row remains unchanged.
     - The second row is shifted by one byte.
     - The third row is shifted by two bytes.
     - The fourth row is shifted by three bytes.
   - This transformation disperses byte values across columns, increasing diffusion by making each byte in one row dependent on bytes in neighboring columns.

3. **MixColumns:**
   - Each column in the state matrix undergoes a transformation involving multiplication by a fixed polynomial in the **Galois Field (GF)**, creating inter-byte dependencies. This operation is a linear transformation that combines bytes within each column to further diffuse the input data.
   - MixColumns contributes to diffusion, ensuring that changes in one part of the plaintext affect multiple ciphertext bytes.

4. **AddRoundKey:**
   - Each byte in the state matrix is combined with a byte from the **round key** using the XOR operation.
   - The round key is derived from the original encryption key through a key expansion process, which generates a unique key for each round.
   - This step integrates key-dependent security into each round, ensuring that the encryption relies directly on the key material.

---

### **AES Key Schedule (Key Expansion)**

The AES key schedule is the process that generates a unique **round key** for each encryption round based on the initial secret key. This key expansion process involves:

- **Key Rotation:** Shifting the bytes in a word (a 4-byte block of the key) to the left, creating a rotated version.
- **Substitution (SubWord):** Substituting each byte in the word using the S-box to increase non-linearity.
- **Round Constant Addition (Rcon):** XORing a constant value with the first byte of the word for each round.
  
This process results in a set of round keys that ensures each encryption round has a distinct, unpredictable key, strengthening the encryption.

---

### **AES Decryption Structure**

AES decryption reverses the encryption process using **inverse transformations** applied in reverse order. The four main transformations for decryption are:

1. **InvShiftRows:** Each row in the state matrix is shifted to the right by a specific offset.
   
2. **InvSubBytes:** Each byte in the state is replaced by its corresponding byte from an inverse S-box.

3. **InvMixColumns:** Each column is transformed by multiplying it with an inverse polynomial in the Galois Field, reversing the diffusion introduced during encryption.

4. **AddRoundKey:** Just like in encryption, each byte in the state is XORed with the round key. This step remains unchanged in both encryption and decryption.

The decryption process uses the same round keys as encryption but applies them in reverse order.

---

### **Advantages of AES Structure**

- **Efficient Design:** AES’s structure allows for highly efficient encryption and decryption processes, both in software and hardware.
- **Security:** The combination of substitution, shifting, and mixing transformations ensures high levels of **confusion** and **diffusion**, making AES resistant to most forms of cryptanalysis.
- **Flexibility:** AES can be used with different key sizes, allowing for increased security when needed without significantly affecting performance.

---

### **Conclusion**

The AES structure, with its well-designed transformations and key schedule, is highly secure, resistant to most known attacks, and efficient enough for widespread use in modern applications. AES’s robustness and versatility make it one of the most trusted encryption standards for protecting sensitive data across industries and applications.
