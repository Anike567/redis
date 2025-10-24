#include "./../include/response.h"
#include <unistd.h>
#include <iostream>
#include <cerrno>
#include <cstring>

Response::Response(int conn_fd) : confd(conn_fd) { }

int Response::send(const std::string &msg) {
    int n = write(confd, msg.c_str(), msg.size());
    if (n < 0) {
        std::cerr << "Write failed: " << strerror(errno) << std::endl;
    }
    return n;
}
