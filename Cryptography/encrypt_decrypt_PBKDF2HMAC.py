from cryptography.fernet import Fernet
from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives.kdf.pbkdf2 import PBKDF2HMAC

import base64
import os

password = "password".encode("utf8")

salt = os.urandom(16)
pbkdf = PBKDF2HMAC(algorithm=hashes.SHA256(),length=32,salt=salt,iterations=100000,backend=default_backend())

key = pbkdf.derive(password)

pbkdf = PBKDF2HMAC(algorithm=hashes.SHA256(),length=32,salt=salt,iterations=100000,backend=default_backend())

pbkdf.verify(password, key)

key = base64.urlsafe_b64encode(key)
fernet = Fernet(key)
token = fernet.encrypt("Secret message".encode("utf8"))

print("Token: "+str(token))
print("Message: "+str(fernet.decrypt(token).decode()))
