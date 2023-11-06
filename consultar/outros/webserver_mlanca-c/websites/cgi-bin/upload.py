#!/usr/bin/env python3
import cgi
import os
import sys

def generate404():
    body = "<html><head><title>404 Not Found</title></head>"
    body += "<body><h1>404 Not Found</h1></body></html>"
    print("HTTP/1.1 404 Not Found\r")
    print("Content-type: text/html\r")
    print(f"Content-Length: {len(body)}\r")
    print("\r")
    print(body, end='')
    sys.exit(0)

def save_file(upload_file, body):

    # print('upload file: ', upload_file)
    # print('upload dir: ', upload_dir)
    # Save the file to the specified directory
    sys.stderr.write(upload_file)
    absolute_path = os.path.expanduser(upload_file)
    open(absolute_path, 'wb').write(bytes(body, 'UTF-8'))

    return file



file = sys.stdin.read()
# print(file)
# Get the file from the form data
form = cgi.FieldStorage()
upload_file = form.getvalue('file', None)
upload_dir = '/tmp'
if (upload_file is None or upload_dir is None):
    generate404()


os.makedirs(upload_dir, exist_ok=True)
upload_file = os.path.join(upload_dir, upload_file)

# Save the file to the specified directory
save_file(upload_file, file)

# Print the path to the saved file as the response
print('HTTP/1.1 201 Created\r')
print('Content-type: text/plain\r')
print('\r')