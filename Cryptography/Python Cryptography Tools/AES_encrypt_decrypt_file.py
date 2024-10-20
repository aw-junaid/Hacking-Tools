from Crypto.Cipher import AES
from Crypto.Hash import SHA256
import os, random, struct
from Crypto import Random

def encrypt_file(key, filename):
    chunk_size = 64*1024

    output_filename = filename + '.encrypted'

    # Random Initialization vector
    iv = Random.new().read(AES.block_size)

    #create the encryption cipher
    encryptor = AES.new(key, AES.MODE_CBC, iv)

    #Determine the size of the file
    filesize = os.path.getsize(filename)
	
	#Open the output file and write the size of the file. 
	#We use the struct package for the purpose.
    with open(filename, 'rb') as inputfile:
        with open(output_filename, 'wb') as outputfile:
            outputfile.write(struct.pack('<Q', filesize))
            outputfile.write(iv)
            
            while True:
                chunk = inputfile.read(chunk_size)
                if len(chunk) == 0:
                    break
                elif len(chunk) % 16 != 0:
                    chunk += bytes(' ','utf-8') * (16 - len(chunk) % 16)

                outputfile.write(encryptor.encrypt(chunk))


def decrypt_file(key, filename):
    chunk_size = 64*1024

    output_filename = os.path.splitext(filename)[0]
	
	#open the encrypted file and read the file size and the initialization vector. 
	#The IV is required for creating the cipher.
    with open(filename, 'rb') as infile:
        origsize = struct.unpack('<Q', infile.read(struct.calcsize('Q')))[0]
        iv = infile.read(16)
		
		#create the cipher using the key and the IV.
        decryptor = AES.new(key, AES.MODE_CBC, iv)
		
		#We also write the decrypted data to a verification file, 
		#so we can check the results of the encryption 
		#and decryption by comparing with the original file.
        with open(output_filename, 'wb') as outfile:
            while True:
                chunk = infile.read(chunk_size)
                if len(chunk) == 0:
                    break
                outfile.write(decryptor.decrypt(chunk))

            outfile.truncate(origsize)


def getKey(password):
    hasher = SHA256.new(password)
    return hasher.digest()


def main():
    choice = input("do you want to (E)ncrypt or (D)ecrypt?: ")
    
    if choice == 'E':
        filename = input('file to encrypt: ')
        password = input('password: ')
        encrypt_file(getKey(password.encode("utf8")), filename)
        print('done.')

    elif choice == 'D':
        filename = input('file to decrypt: ')
        password = input('password: ')
        decrypt_file(getKey(password.encode("utf8")), filename)
        print('done.')

    else:
        print('no option selected.')

if __name__ == "__main__":
    main()
    
