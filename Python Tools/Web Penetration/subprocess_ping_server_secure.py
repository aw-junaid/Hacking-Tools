import subprocess


#Rather than passing a string to subprocess, our function passes a list of strings. 
#The ping program gets each argument separately, 
#so the shell does not process other commands that are provided by the user after the ping #command terminates.

def ping_secure(myserver):
    command_arguments = ['ping','-c','1', myserver]
    return subprocess.Popen(command_arguments, shell=False)

print(ping_secure('8.8.8.8'))
