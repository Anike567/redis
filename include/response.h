#ifndef RESPONSE_H
#define RESPONSE_H

#include <string>
#include <unistd.h>

class Response {
private:
    int confd;
    map<string, string> header;

public:
    Response(int conn_fd);

    int send(const std::string &msg);
    int sendFile(const std::string &path); 
};

#endif
