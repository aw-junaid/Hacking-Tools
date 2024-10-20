import socketio
import aiohttp
import asyncio

# Create a new Socket.IO server instance with asyncio support
sio = socketio.AsyncServer(async_mode='aiohttp')

# Create an aiohttp web application
app = aiohttp.web.Application()
sio.attach(app)

# Define an event handler for the 'connect' event
@sio.event
async def connect(sid, environ):
    print(f"Client connected: {sid}")

# Define an event handler for the 'message' event
@sio.event
async def message(sid, data):
    print(f"Message from {sid}: {data}")
    # Broadcast the received message to all connected clients
    await sio.emit('response', {'data': f"Server received: {data}"}, room=sid)

# Define an event handler for the 'disconnect' event
@sio.event
async def disconnect(sid):
    print(f"Client disconnected: {sid}")

if __name__ == '__main__':
    # Start the server on port 5000
    web_runner = aiohttp.web.AppRunner(app)
    loop = asyncio.get_event_loop()
    
    try:
        loop.run_until_complete(web_runner.setup())
        loop.run_until_complete(
            aiohttp.web.TCPSite(
                web_runner, 'localhost', 5000
            ).start()
        )
        print("Server running on http://localhost:5000")
        loop.run_forever()
    except KeyboardInterrupt:
        pass
    finally:
        loop.run_until_complete(web_runner.cleanup())
