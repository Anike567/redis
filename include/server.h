#ifndef EXPRESS_H
#define EXPRESS_H

#include <string>
#include <functional>
#include <netinet/in.h>
#include "./request.h"
#include "./response.h"

using Handler = function<void(Request &req, Response &res)>;


class Express {
private:
    int fd;
    int domain = AF_INET;
    int type = SOCK_STREAM;
    int protocol = 0;
    int conn_fd;
    map<string, Handler> getRoutes;
    struct sockaddr_in addr;

    int create_server();

public:
    Express();
    int get_fd();
    void listen(int port);
    void close_server();
    void setConnFd(int conn_fd);
    void get(const string path, Handler fn);
    Handler findRoute(const string method, const string path);
    
    ~Express();
};

#endif
