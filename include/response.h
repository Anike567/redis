#ifndef RESPONSE_H
#define RESPONSE_H

#include <string>
#include <unistd.h>
#include <map>

using namespace std;

class Response {
private:
    int connfd;
    map<string, string> headers;

public:
    Response(int conn_fd);

    void setHeader(const string &key, const string &value);
    int send(const string &body, const string &status = "200 OK", const string &contentType = "text/plain");
    int sendFile(const string &path);
};

#endif
