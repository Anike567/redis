#include "./../include/server.h"
#include <iostream>
#include <stdexcept>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <functional>
#include <string>
#include<fstream>

using namespace std;

int Express::create_server() {
    fd = socket(domain, type, protocol);
    if (fd == -1) {
        throw runtime_error(string("Socket creation failed: ") + strerror(errno));
    }

    int opt = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); 

    cout << "Socket created. FD = " << fd << endl;
    return fd;
}

Express::Express() {
    try {
        create_server();
    } catch (const exception& e) {
        cerr << e.what() << endl;
    }
}

int Express::get_fd() {
    return fd;
}

void Express::setConnFd(int conn_fd){
    this->conn_fd = conn_fd;
}


void Express::listen(int port) {
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (::bind(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        throw runtime_error(string("Bind failed: ") + strerror(errno));
    }

    if (::listen(fd, SOMAXCONN) == -1) {
        throw runtime_error(string("Listen failed: ") + strerror(errno));
    }

    cout << "Server running on port " << port << endl;
}

void Express::get(const string path, Handler fn){
    getRoutes[path] = fn;
}

Handler Express::findRoute(const string method, const string path){
    if(method == "GET" && getRoutes.count(path)){
        return getRoutes[path];
    }

    return nullptr;
}

void Express::close_server() {
    close(fd);
    cout << "Server closed." << endl;
}


Express::~Express() {
    close_server();
}
