import secrets
import string

def generateSecureURL():

    src = string.ascii_letters + string.digits + string.punctuation
    password = secrets.choice(string.ascii_lowercase)
    password += secrets.choice(string.ascii_uppercase)
    password += secrets.choice(string.digits)
    password += secrets.choice(string.punctuation)
    
    for i in range (16):
        password += secrets.choice(src)

    print ("Strong password:", password)

    secureURL = "https://www.domain.com/auth/reset="
    secureURL += secrets.token_urlsafe(16)

    print("Token secure URL:", secureURL)

if __name__ == "__main__":
    generateSecureURL() 
