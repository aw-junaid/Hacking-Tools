from subprocess import Popen, PIPE

process = Popen(['nmap','-O','127.0.0.1'], stdout=PIPE, stderr=PIPE)
stdout, stderr = process.communicate()
print(stdout.decode())
