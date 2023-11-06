#!/usr/bin/env python3

# Import modules for CGI handling
import cgi, cgitb

# Create instance of FieldStorage
form = cgi.FieldStorage()

# Get data from fields
name = form.getvalue('name')

body = f'<html> <head> <title>Hello - Second CGI Program</title> </head> <body> <h2>Hello {name}</h2> </body> </html>'
print('HTTP/1.1 200 OK\r')
print('Content-Type: text/html\r')
print(f'Content-Length: {len(body)}\r')
print('\r')
print(body, end='')