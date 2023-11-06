# webserv

coding style :

class : CapitalForTheFirstLetterOfEveryWord

function : replace_space_with_underscore

todo :
1. default conf if none are provided ?
2. remove -D TEST flag
3. remember to close all the fds (sockets, epoll ...) 
4. is F_GETFL forbidden ? (function make_socket_nonblock)
5. Should I make all the sockets non-block ?
6. Should I erase old request every time I get a new one?
7. Is it necessary to set client to write in epoll?  If we write until buffer is full, write will write EAGAIN in errno, at this moment EPOLLOUT will be triggered in epoll. ref : https://www.gushiciku.cn/pl/g2xg/zh-tw
8. two the same uri for different locations in the same server
9. need to test find_file in HTTPRequest.cpp
10. Default index page for server
11. Check Error 414 ?
12. upload a big file can't finish reading at once
13. CGI TLE
14. Do I need to delete_client_from_epoll after dealing with event ?

## reference

socket programming :

https://www.geeksforgeeks.org/socket-programming-cc/

https://github.com/Jibus22/webserv/wiki/Socket

https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa

epoll :

https://swingseagull.github.io/2016/11/08/epoll-sample/
https://www.cnblogs.com/fnlingnzb-learner/p/5835573.html
https://www.jianshu.com/p/ee381d365a29

cgi :

https://tigercosmos.xyz/post/2020/01/web/cgi/

cookie :

https://www.freecodecamp.org/news/everything-you-need-to-know-about-cookies-for-web-development/

https://www.valentinog.com/blog/cookies/

https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers/Cookie

http status code :

https://en.wikipedia.org/wiki/List_of_HTTP_status_codes

delete method :

https://reqbin.com/Article/HttpDelete

unread :

https://www.notion.so/etelcode/Documentation-Webserv-320727979ffd4176a7dd5ba41aaadf46 - french
