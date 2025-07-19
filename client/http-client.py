"""
HTTP Client using existing Python libraries.
Used to test the HTTP server project implementation in C.
"""

import httpx 

"""
GET / HTTP/1.1
Host: localhost:9999
Accept: */*
Accept-Encoding: gzip, deflate, br
Connection: keep-alive
User-Agent: python-httpx/0.27.0
"""
port = 9999
request = httpx.get(f"http://localhost:{port}")



print(request.status_code)