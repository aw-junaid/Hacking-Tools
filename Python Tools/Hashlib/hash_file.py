import hashlib

file_name = input("Enter file name:")
 
file = open(file_name, 'r')
data = file.read().encode('utf-8')

print("-- %s --" % file_name)
print(hashlib.algorithms_available)

for algorithm in hashlib.algorithms_available:
    hash = hashlib.new(algorithm)
    hash.update(data)
    try:
        hexdigest = hash.hexdigest()
    except TypeError:
        hexdigest = hash.hexdigest(128)
    
    print("%s: %s" % (algorithm, hexdigest))
