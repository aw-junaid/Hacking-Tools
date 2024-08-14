import os
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.backends import default_backend

backend = default_backend()
key = os.urandom(32)
iv = os.urandom(16)
cipher = Cipher(algorithms.AES(key), modes.CBC(iv), backend=backend)

encryptor = cipher.encryptor()
print(encryptor)

message_encrypted = encryptor.update("a secret message".encode("utf8"))

print("Cipher text: "+str(message_encrypted))
cipher_text =  message_encrypted + encryptor.finalize()

decryptor = cipher.decryptor()

print("Plain text: "+str(decryptor.update(cipher_text).decode()))
