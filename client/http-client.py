"""
HTTP Client using existing Python libraries.
Used to test the HTTP server project implementation in C.
"""

import httpx 

"""
CLI
"""
user_yes_or_no = input("Are you sure the server has started? [Y/N]")
if user_yes_or_no.lower() != "y":
    print("Exiting...")
    exit(0)

port = int(input("Enter the port number of the server: "))

if not port:
    print("No port provided, using default port 8080.")
    port = "8080"

if port < 1024:
    print("Port number should be above 1024 for non-root users. Using port 8080 instead.")
    port = "8080"

"""
This is what we send.
GET / HTTP/1.1
Host: localhost:9999
Accept: */*
Accept-Encoding: gzip, deflate, br
Connection: keep-alive
User-Agent: python-httpx/0.27.0

What the server sees:
python3 -m http.server -b 127.0.0.42 8080

Serving HTTP on 127.0.0.1 port 8080 (http://127.0.0.1:8080/) ...
127.0.0.1 - - [20/Jul/2025 00:49:04] "GET / HTTP/1.1" 200 -

"""
request = httpx.get(f"http://localhost:{port}")

print("Status code: ", request.status_code)
print("Headers: ", request.headers)
print("Text: ", request.text)
print("Content: ", request.content)

"""
200
Headers({'server': 'SimpleHTTP/0.6 Python/3.10.14', 'date': 'Sat, 19 Jul 2025 16:49:35 GMT', 'content-type': 'text/html; charset=utf-8', 'content-length': '448'})
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<title>Directory listing for /</title>
</head>
<body>
<h1>Directory listing for /</h1>
<hr>
    <ul>
        <li><a href="http-server">http-server</a></li>
        <li><a href="http-server.c">http-server.c</a></li>
        <li><a href="http-server.py">http-server.py</a></li>
    </ul>
<hr>
</body>
</html>

"""