#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <netinet/in.h>
#include <unistd.h>
#include <stdexcept>
#include <cerrno>
#include <cstring>

using namespace std;

static void die(const char *msg) {
    int err = errno;
    fprintf(stderr, "[%d] %s\n", err, msg);
    abort();
}

int main(void) {
   
   
    // Create socket
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("socket");
        return 1;
    }

    // Setup address
    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(4000);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // 127.0.0.1

    // Connect to server
    int rv = connect(fd, (struct sockaddr *)&addr, sizeof(addr));
    if (rv < 0) {
        die("connect");
    }

    // Send message
    const string header = "GET/http1.1\r\n\r\n ";
    const string body = "{command:get, key:name}";
    ssize_t sent;
    
    sent = write(fd, header.c_str(), header.size());

    if(sent < 0 || sent < header.size()){
        perror("write");
    }
    sent = write(fd, body.c_str(), body.size());
    shutdown(fd, SHUT_WR);

    // Read response
    char buff[64] = {};
    ssize_t n;
    string str;
    while ((n = read(fd, buff, sizeof(buff))) > 0) {
        str.append(buff, n);
    }

    cout << "Server says: " << str << endl;
    close(fd);

    return 0;
}
