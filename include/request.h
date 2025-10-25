#ifndef REQUEST_H
#define REQUEST_H

#include <iostream>
#include <string>
#include <unistd.h> 
#include<map>

using namespace std;

class Request {
private:
    int connfd;
    string body;
    map<string, string> header;

    void generateBody();
    void generateHeader();

public:
    Request(int conn_fd);
    string get_body();
    string getHeader(string name);
};

#endif
