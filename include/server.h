#ifndef EXPRESS_H
#define EXPRESS_H

#include <string>
#include <functional>
#include <netinet/in.h>
#include "./request.h"
#include "./response.h"

class Express {
private:
    int fd;
    int domain = AF_INET;
    int type = SOCK_STREAM;
    int protocol = 0;
    int conn_fd;
    struct sockaddr_in addr;

    int create_server();

public:
    Express();
    int get_fd();
    void listen(int port);
    void close_server();
    void setConnFd(int conn_fd);

    ~Express();
};

#endif
