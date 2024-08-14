from Crypto.Cipher import AES

# key has to be 16, 24 or 32 bytes long
key="secret-key-12345"

encrypt_AES = AES.new(key.encode("utf8"), AES.MODE_CBC, 'This is an IV-12'.encode("utf8"))

# Fill with spaces the user until 32 characters
message = "This is the secret message      ".encode("utf8")

ciphertext = encrypt_AES.encrypt(message)
print("Cipher text: " , ciphertext)

decrypt_AES = AES.new(key.encode("utf8"), AES.MODE_CBC, 'This is an IV-12'.encode("utf8"))
message_decrypted =  decrypt_AES.decrypt(ciphertext)

print("Decrypted text: ",  message_decrypted.strip().decode())
