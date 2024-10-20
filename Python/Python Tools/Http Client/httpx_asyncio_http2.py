import httpx
import asyncio

async def resquest_http2():
	async with httpx.AsyncClient(http2=True) as client:
		response = await client.get("https://www.google.es")
		print(response)
		print(response.http_version)
		
asyncio.run(resquest_http2())
