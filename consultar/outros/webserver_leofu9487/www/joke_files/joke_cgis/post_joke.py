import urllib.parse, os
string = urllib.parse.unquote_plus(input()[8:])
for i in range(0, 100):
	if os.path.isfile('www/joke_files/jokes/joke_' + str(i)) == False:
		f = open('www/joke_files/jokes/joke_' + str(i), 'x')
		f.write(string)
		f.close()
		print('Status: 201\r')
		print('Content-Type: text/html\r\n')
		print('The joke is saved as joke_' + str(i) + '\r')
		print('<li><a href="/test_joke.html">back to joke page</a></li>')
		break
	elif i == 99:
		print('Status: 422\r')
		print('Content-Type: text/html\r\n')
		print('Status : 422 Unprocessable Entity.\r')
		print('There are already too many jokes!\r')
		print('<li><a href="/test_joke.html">back to joke page</a></li>')
		break
