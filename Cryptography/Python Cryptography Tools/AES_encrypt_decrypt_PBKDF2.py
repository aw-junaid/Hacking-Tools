from Crypto.Cipher import AES
from Crypto.Protocol.KDF import PBKDF2
from Crypto import Random

# key has to be 16, 24 or 32 bytes long
key="secret-key-12345"

iterations = 10000
key_size = 16
salt = Random.new().read(key_size)
iv = Random.new().read(AES.block_size)
derived_key = PBKDF2(key, salt, key_size, iterations)

encrypt_AES = AES.new(derived_key, AES.MODE_CBC, iv)

# Fill with spaces the user until 32 characters
message = "This is the secret message      ".encode("utf8")

ciphertext = encrypt_AES.encrypt(message)
print("Cipher text: " , ciphertext)

decrypt_AES = AES.new(derived_key, AES.MODE_CBC, iv)
message_decrypted =  decrypt_AES.decrypt(ciphertext)

print("Decrypted text: ",  message_decrypted.strip().decode())

