import subprocess
import sys
command_ping = '/bin/ping'
ping_parameter ='-c 1'
domain = "www.awjunaid.com"
p = subprocess.Popen([command_ping,ping_parameter,domain], shell=False, stderr=subprocess.PIPE)
out = p.stderr.read(1)
sys.stdout.write(str(out.decode('utf-8')))
sys.stdout.flush()
