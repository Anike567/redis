#include "./../include/request.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdexcept>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <unistd.h>

using namespace std;

void Request::generateBody() {
    char buff[64] = {};
    ssize_t n ;
    while((n = read(connfd, buff, sizeof(buff))) > 0){
        cout<<n<<endl;
        if (n < 0) {
            cout << "Something went wrong while reading message from client" << endl;
            break;
        }
        // for(char ch : buff){
        //     cout<<ch;
        // }
        body.append(buff, n);
    }
}

void Request::generateHeader(){
    
}

Request::Request(int conn_fd) {
    connfd = conn_fd;
    generateBody();
}

string Request::get_body() {
    return body;
}

string Request::getHeader(string name){
    if(header.find(name) != header.end()){
        return header[name];
    }

    return "";
}
