#ifndef RESPONSE_H
#define RESPONSE_H

#include <string>
#include <unistd.h>
#include<map>

class Response {
private:
    int confd;
    // std::map<string, string> header;

public:
    Response(int conn_fd);

    int send(const std::string &msg);
    int sendFile(const std::string &path); 
};

#endif
