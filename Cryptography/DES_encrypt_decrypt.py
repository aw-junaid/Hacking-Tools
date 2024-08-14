from Crypto.Cipher import DES

# Fill with spaces the user until 8 characters
user =  "user    ".encode("utf8")
message = "message ".encode("utf8")

key='mycipher'

# we create the cipher with DES
cipher = DES.new(key.encode("utf8"),DES.MODE_ECB)

# encrypt username and message
cipher_user = cipher.encrypt(user)
cipher_message = cipher.encrypt(message)

print("Cipher User: " + str(cipher_user))
print("Cipher message: " + str(cipher_message))

# We simulate the server where the messages arrive encrypted
cipher = DES.new(key.encode("utf8"),DES.MODE_ECB)
decipher_user = cipher.decrypt(cipher_user)
decipher_message = cipher.decrypt(cipher_message)

print("Decipher user: " + str(decipher_user.decode()))
print("Decipher Message: " + str(decipher_message.decode()))
