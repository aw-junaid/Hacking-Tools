from Crypto.Hash import MD5

def get_file_checksum(filename):
    hash = MD5.new()
    chunk_size = 8191
    with open(filename, 'rb') as file:
        while True:
            chunk = file.read(chunk_size)
            if len(chunk) == 0:
                break
            hash.update(chunk)
            return hash.hexdigest()
			
print('The MD5 checksum is',get_file_checksum('checksSumFile.py'))
