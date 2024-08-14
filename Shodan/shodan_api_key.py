import os

# Set the Shodan API key as an environment variable
os.environ['SHODAN_API_KEY'] = 'SET_YOUR_OWN_API_KEY'

# Verify that the environment variable is set correctly
if 'SHODAN_API_KEY' not in os.environ:
    print("Error: SHODAN_API_KEY environment variable not set.")
    exit(1)

print("SHODAN_API_KEY environment variable set successfully.")
