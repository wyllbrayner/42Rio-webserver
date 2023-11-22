#!/usr/bin/env python3
import cgi
import cgitb

cgitb.enable()  # for troubleshooting

print("Content-type: text/html\n")
print("<html><head>")
print("<title>CGI script output</title>")
print("</head><body>")

form = cgi.FieldStorage()
if "fname" not in form or "lname" not in form:
    print("<h1>Error</h1>")
    print("Please fill in the name and addr fields.")
else:
    print("<p>Eu", form["fname"].value + " " + form["lname"].value + " sou amigo(a) do parnaldo, o melhor cadete da 42.</p>")

print("</body></html>")