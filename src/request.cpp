#include "./../include/request.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdexcept>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <unistd.h>

using namespace std;

void Request::generate_body() {
    char buff[64] = {};
    ssize_t n = read(connfd, buff, sizeof(buff));

    body.append(buff, n);
    

    if (n < 0) {
        cout << "Something went wrong while reading message from client" << endl;
    }
}

Request::Request(int conn_fd) {
    connfd = conn_fd;
    generate_body();
}

string Request::get_body() {
    return body;
}
