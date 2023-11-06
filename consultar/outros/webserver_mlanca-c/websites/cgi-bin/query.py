#!/usr/bin/env python3

import os
import urllib.parse

print("HTTP/1.1 200 OK\r")
print("Content-Type: text/html\r")

# Get the query string from the environment
query_string = os.environ.get("QUERY_STRING", "")

# Parse the query string into a dictionary
params = urllib.parse.parse_qs(query_string)

# Print the parameters as HTML
body : str = "<html><body>"
body += "<h1>Query Parameters:</h1>"
body += "<ul>"
for name, values in params.items():
    for value in values:
        body += "<li>{0}={1}</li>".format(name, value)
body += "</ul>"
body += "</body></html>"
print(f"Content-Length: {len(body)}\r")
print("\r")
print(body, end="")