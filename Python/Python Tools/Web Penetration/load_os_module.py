import os

try:
    eval("__import__('os').system('clear')", {})
    print("Module OS loaded by eval")
except Exception as exception:
    print(repr(exception))
