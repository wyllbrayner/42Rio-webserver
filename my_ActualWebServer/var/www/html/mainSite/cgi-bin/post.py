#!/usr/bin/python3

import cgi, os
import cgitb; cgitb.enable()

form = cgi.FieldStorage()
#Get filename here.
fileitem = form['file']
# Test if the file was uploaded
if fileitem.filename:
   # strip leading path from file name to avoid
   # directory traversal attacks
   open("var/www/html/mainSite/cgi-bin/tmp/" + fileitem.filename, "wb").write(fileitem.file.read())
   message = 'The file "' +fileitem.filename + '" was uploaded successfully'
 
else:
   message = 'No file was uploaded'

print(message)