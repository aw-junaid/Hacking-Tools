import os
import yaml
user_input = input()
with open(user_input) as secure_file:
    contents = yaml.safe_load(secure_file) # secure
    print(contents)

