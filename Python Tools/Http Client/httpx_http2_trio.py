import httpx
import trio

results={}

async def fetch_result(client,url,results):
    print(url)
    results[url] = await client.get(url)
	
async def main_parallel_requests():
	async with httpx.AsyncClient(http2=True) as client:
		async with trio.open_nursery() as nursey:
			for i in range(2000,2020):
				url = f"https://en.wikipedia.org/wiki/{i}"
				nursey.start_soon(fetch_result,client,url,results)
		
trio.run(main_parallel_requests)
print(results)
