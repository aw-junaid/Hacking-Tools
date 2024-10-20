import os
import pickle
import yaml
user_input = input()

with open(user_input,'rb') as file:
    contents = pickle.load(file) # insecure

with open(user_input, 'rb') as file:
    contents = yaml.load(file) # insecure
