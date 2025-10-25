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

int main(int argc, char *argv[]) {
    // Build message from command line args
    string msg = "";
    for (int i = 1; i < argc; i++) {
        msg += argv[i];
        if (i < argc - 1) msg += " ";
    }

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
    ssize_t sent = write(fd, msg.c_str(), msg.size());
    if (sent < 0) {
        perror("write");
    }
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
