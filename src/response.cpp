#include "./../include/response.h"
#include <unistd.h>
#include <iostream>
#include <cerrno>
#include <cstring>
#include <fstream>
#include<sys/socket.h>

Response::Response(int conn_fd) : confd(conn_fd) {}

// send simple string message
int Response::send(const std::string &msg) {
    int n = write(confd, msg.c_str(), msg.size());
    if (n < 0) {
        std::cerr << "Write failed: " << strerror(errno) << std::endl;
    }
    return n;
}

// send a file as a response (binary-safe)
int Response::sendFile(const std::string &path) {

    std::ifstream file(path, std::ios::in | std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << path << std::endl;
        return -1;
    }

    char buffer[4096];

    while (!file.eof()) {
        file.read(buffer, sizeof(buffer));
        std::streamsize bytesRead = file.gcount();

        if (bytesRead <= 0)
            break;

        ssize_t bytesSent = write(confd, buffer, bytesRead);
        if (bytesSent < 0) {
            std::cerr << "Write failed: " << strerror(errno) << std::endl;
            file.close();
            return -1;
        }
    }
    
    // shutdown(confd, SHUT_WR);

    file.close();
    return 0; 
}
