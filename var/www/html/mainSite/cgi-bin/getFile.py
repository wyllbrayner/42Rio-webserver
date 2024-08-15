#!/usr/bin/env python3

# Import modules for CGI handling
import sys, os, cgi

form = cgi.FieldStorage()

filename = form.getvalue('filename')

fail_message = b"<html>"
fail_message += b"<p>No file was uploaded</p>"
fail_message += b"</html>"

if (filename == None):
	sys.stdout.buffer.write(b"400 Bad Request\r\n")
	sys.stdout.buffer.write(fail_message)
	sys.exit()

if os.path.exists("var/www/html/mainSite/cgi-bin/tmp/" + filename):
	file_content = open("var/www/html/mainSite/cgi-bin/tmp/" + filename, "rb").read()
	sys.stdout.buffer.write(file_content)
else:
	sys.stdout.buffer.write(b"400 Bad Request\r\n")
	sys.stdout.buffer.write(fail_message)