from Crypto.PublicKey import RSA 
from Crypto.Cipher import PKCS1_OAEP
from Crypto.Hash import SHA256
from Crypto.Signature import PKCS1_v1_5

def generate(bit_size):
    keys = RSA.generate(bit_size)
    return keys

def encrypt(pub_key, data):
    cipher = PKCS1_OAEP.new(pub_key)
    return cipher.encrypt(data)

def decrypt(priv_key, data):
    cipher = PKCS1_OAEP.new(priv_key)
    return cipher.decrypt(data)

keys = generate(2048)

print("Public key:")
print(keys.publickey().export_key('PEM').decode(), end='\n\n')
with open("public.key",'wb') as file:
    file.write(keys.publickey().export_key())

print("Private Key:")
print(keys.export_key('PEM').decode())
with open("private.key",'wb') as file:
    file.write(keys.export_key('PEM'))

text2cipher = "text2cipher".encode("utf8")

hasher = SHA256.new(text2cipher)
signer = PKCS1_v1_5.new(keys)
signature = signer.sign(hasher)

verifier = PKCS1_v1_5.new(keys)
if verifier.verify(hasher, signature):
    print('The signature is valid!')
else:
    print('The message was signed with the wrong private key or modified')
 
encrypted_data = encrypt(keys.publickey(),text2cipher)
print("Text encrypted:",encrypted_data)

decrypted_data = decrypt(keys,encrypted_data)
print("Text Decrypted:",decrypted_data.decode())


