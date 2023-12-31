/*
# include <fcntl.h>
bool isNonBlocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        // Error handling: fcntl failed
        std::cerr << "Error getting file descriptor flags\n";
        return false; // Assuming non-blocking on error is false
    }

    return (flags & O_NONBLOCK) != 0;
    // if (isNonBlocking(this->conn.getFd(i).fd))
    //     std::cout << "The socket " << i << " is in non-blocking mode.\n";
    // else
    //     std::cout << "The socket " << i << " is in blocking mode.\n";

}
*/
