#!/usr/bin/python3

import cgi, os
import cgitb; cgitb.enable()

form = cgi.FieldStorage()

fileitem = form['file']

if fileitem.filename:
   if not os.path.exists("var/www/html/mainSite/cgi-bin/tmp/"):
      os.makedirs("var/www/html/mainSite/cgi-bin/tmp/")
   
   open("var/www/html/mainSite/cgi-bin/tmp/" + fileitem.filename, "wb").write(fileitem.file.read())
   message = 'The file "' + fileitem.filename + '" was uploaded successfully'
 
else:
   message = 'No file was uploaded'

print(message)