#ifndef REQUEST_H
#define REQUEST_H

#include <iostream>
#include <string>
#include <unistd.h> 
#include <map>

using namespace std;

class Request {
private:
    int connfd;
    string request;
    map<string, string> header;
    map<string, string> body;
    string method;
    string path;

    void generateRequestString();
    void parseRequest();
    void parseHeaders(const string& headerPart);
    void parseBody(const string& bodyPart);
    string trim(const string& s);

public:
    Request(int conn_fd);

    map<string, string>& getBody();
    string getHeader(string name);
    string getMethod();
    string getPath();
};

#endif
