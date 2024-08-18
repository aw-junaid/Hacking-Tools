import asyncio, asyncssh, sys

class MySSHServer(asyncssh.SSHServer):
    def connection_made(self, conn):
        print('SSH connection received from %s.' % conn.get_extra_info('peername')[0])

async def start_server():
    await asyncssh.create_server(MySSHServer, 'localhost', 22,
                                 server_host_keys=['/etc/ssh/ssh_host_ecdsa_key'])

loop = asyncio.get_event_loop()

try:
    print("Starting SSH server on localhost:22")
    loop.run_until_complete(start_server())
except (OSError, asyncssh.Error) as exc:
    sys.exit('Error starting server: ' + str(exc))

loop.run_forever()
