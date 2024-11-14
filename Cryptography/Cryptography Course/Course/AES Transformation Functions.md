### **Cryptography - AES Transformation Functions**

The **Advanced Encryption Standard (AES)** employs a series of transformation functions designed to provide both **confusion** and **diffusion**, essential properties that make the encryption secure. These transformations include **SubBytes**, **ShiftRows**, **MixColumns**, and **AddRoundKey**. Each of these steps introduces specific properties to the encryption process that help prevent cryptographic attacks. Here’s a breakdown of each transformation function:

---

### **1. SubBytes (Byte Substitution)**

The **SubBytes** transformation is a **non-linear substitution** step applied to each byte of the AES state matrix independently.

- **Purpose:** Introduces non-linearity and confusion, making the relationship between the plaintext and ciphertext complex and resistant to linear attacks.
- **Implementation:** Each byte is replaced by another byte according to a pre-defined substitution table known as the **S-box (substitution box)**. The S-box is carefully designed to be resistant to cryptanalysis, ensuring high security.
- **S-box Design:** The AES S-box is created using an **inversion in a finite field (Galois Field GF(2^8))**, followed by an affine transformation. This structure introduces resistance to both linear and differential cryptanalysis.

The result is that each byte in the state matrix is replaced with a byte from the S-box, enhancing the cipher's overall complexity.

---

### **2. ShiftRows (Row Shifting)**

The **ShiftRows** transformation is a **permutation** step that shifts the bytes in each row of the AES state matrix to the left by a specific number of positions.

- **Purpose:** Increases diffusion by rearranging bytes across different columns in the matrix. This means that a change in one byte will affect multiple columns after subsequent transformations.
- **Implementation:**
  - **First row:** No shift (remains as is).
  - **Second row:** Shift left by 1 byte.
  - **Third row:** Shift left by 2 bytes.
  - **Fourth row:** Shift left by 3 bytes.

This transformation ensures that data dependencies extend across multiple columns, making it harder for attackers to analyze relationships between plaintext and ciphertext.

---

### **3. MixColumns (Column Mixing)**

The **MixColumns** transformation is a **linear transformation** applied to each column of the state matrix. It mixes the data within each column by applying a polynomial function in the **Galois Field (GF(2^8))**.

- **Purpose:** Provides diffusion by mixing the bytes within each column so that any change to a single byte in the input affects all bytes in that column.
- **Implementation:** Each column in the state matrix is multiplied by a fixed polynomial, represented as a matrix multiplication:

  $\[
  \begin{bmatrix}
  2 & 3 & 1 & 1 \\
  1 & 2 & 3 & 1 \\
  1 & 1 & 2 & 3 \\
  3 & 1 & 1 & 2 \\
  \end{bmatrix}
  \times
  \begin{bmatrix}
  a_0 \\
  a_1 \\
  a_2 \\
  a_3 \\
  \end{bmatrix}
  \]$
  
  where $\( a_0, a_1, a_2, a_3 \)$ represent the bytes in a single column.

- **Effect:** The multiplication within the Galois Field ensures that each byte in a column is influenced by other bytes in that column, dispersing information across the matrix and increasing resistance to attacks.

---

### **4. AddRoundKey (Round Key Addition)**

The **AddRoundKey** transformation is an XOR operation between the current state matrix and a round key derived from the original AES key.

- **Purpose:** Adds key-dependency to each round of AES, ensuring that the encryption process is unique to the specific key in use.
- **Implementation:** Each byte in the state matrix is XORed with the corresponding byte in the round key. This step is straightforward but highly effective, as the encryption depends directly on the key, preventing attackers from predicting the transformations without the correct key.
- **Key Schedule:** The AES key schedule generates a different round key for each round using transformations and rotations. This ensures that each round key differs significantly from the others, increasing the difficulty of key recovery.

---

### **Overall Round Structure in AES**

In each round of AES, these transformation functions are applied sequentially to the AES state, except in the final round, where **MixColumns** is omitted to balance the encryption process.

- **Initial Round:** AddRoundKey
- **Main Rounds (for AES-128, 9 rounds):**
  - SubBytes → ShiftRows → MixColumns → AddRoundKey
- **Final Round:** SubBytes → ShiftRows → AddRoundKey (MixColumns omitted)

The combination of these transformations creates a complex and secure encryption process, achieving both **confusion** (by using substitution and key addition) and **diffusion** (by spreading data dependencies across the state matrix). These properties make AES highly resistant to known cryptographic attacks, such as linear and differential cryptanalysis.

---

### **AES Decryption: Inverse Transformations**

For decryption, AES uses inverse transformations applied in reverse order:

- **InvShiftRows:** Shifts rows to the right.
- **InvSubBytes:** Uses an inverse S-box to reverse byte substitution.
- **InvMixColumns:** Uses an inverse polynomial to reverse column mixing.
- **AddRoundKey:** Remains the same as in encryption.

These inverse transformations are applied sequentially in reverse order to obtain the original plaintext.

---

### **Conclusion**

The transformation functions in AES are carefully designed to balance security and efficiency, creating a highly secure encryption method. By combining substitution, shifting, mixing, and key addition, AES provides robust protection against modern cryptographic attacks and is widely used for secure data transmission and storage.
