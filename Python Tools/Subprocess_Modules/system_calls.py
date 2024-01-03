#!/usr/bin/python3
import os
from subprocess import call

# use the os interface to get access to system information
print("Current path",os.getcwd())
print("PATH Environment variable:",os.getenv("PATH"))

# using system method from os module
print("List files using the os module:")
os.system("ls -la")

# using call method from subprocess module
print("List files using the subprocess module:")
call(["ls", "-la"])
