from secrets import choice
from string import ascii_letters, ascii_uppercase, digits

characters = ascii_letters + ascii_uppercase + digits
length = 16
random_password= ''.join(choice(characters) for character in range(length))
print("The password generated is:", random_password)
