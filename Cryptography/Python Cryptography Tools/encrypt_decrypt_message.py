from cryptography.fernet import Fernet

key = Fernet.generate_key()
cipher_suite = Fernet(key)
print("Key "+str(cipher_suite))

message = "Secret message".encode("utf8")
cipher_text = cipher_suite.encrypt(message)
plain_text = cipher_suite.decrypt(cipher_text)

print("Cipher text: "+str(cipher_text.decode()))
print("Plain text: "+str(plain_text.decode()))
