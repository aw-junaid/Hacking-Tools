from torpy.http.requests import TorRequests

with TorRequests() as tor_requests:
    print("building circuit...")
    with tor_requests.get_session() as session:
        print(session.get("http://httpbin.org/ip").json())
    print("renewing circuit...")
    with tor_requests.get_session() as session:
        print(session.get("http://httpbin.org/ip").json())
    
    response = session.get('http://3g2upl4pq6kufc4m.onion')
    for key,value in response.headers.items():
        print(key,value)
