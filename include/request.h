#ifndef REQUEST_H
#define REQUEST_H

#include <iostream>
#include <string>
#include <unistd.h> 

using namespace std;

class Request {
private:
    int connfd;
    string body;

    void generate_body();

public:
    Request(int conn_fd);
    string get_body();
};

#endif
