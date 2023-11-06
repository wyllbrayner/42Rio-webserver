#!/usr/bin/env python3

print("HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: 148\r\n"
        "\r\n"
        "<!DOCTYPE html> <html> <head>   <title>Hello "
        "Page</title> </head> <body>   <h1>Hello, World!</h1> "
        "  <p>Welcome to my hello page.</p> </body> </html>", end="")