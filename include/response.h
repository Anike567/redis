#ifndef RESPONSE_H
#define RESPONSE_H

#include <string>
#include <unistd.h> 

class Response {
private:
    int confd;

public:
    Response(int conn_fd);          // constructor
    int send(const std::string &msg); // send method
};

#endif /
