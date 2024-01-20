import subprocess

#the problem with this method is that the shell can 
#process other commands that are provided by the user after the ping command terminates.
def ping_insecure(myserver):
    return subprocess.Popen('ping -c 1 %s' % myserver, shell=True)

print(ping_insecure('8.8.8.8 & touch file'))
