#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <netinet/in.h>
#include <unistd.h>
#include <stdexcept>
#include <cerrno>
#include <cstring>

using namespace std;

class Request{

};

class Response{

};

class Express {
private :
    int fd;
    int domain = AF_INET;      // IPv4
    int type = SOCK_STREAM;    // TCP
    int protocol = 0;          // Default (TCP)
    struct sockaddr_in addr = {};

    int create_server() {
        fd = socket(domain, type, protocol);
        if (fd == -1) {
            throw runtime_error(string("Socket creation failed: ") + strerror(errno));
        }
        cout << "Socket created successfully. FD = " << fd << endl;
        return fd;
    }


    //constructor
public:
    Express() {  
        try {
            fd = create_server();
        } catch (const exception &e) {
            cerr << "Error: " << e.what() << endl;
        }
    }
    
    int get_fd(){
        return fd;
    }
    void listen(int port) {
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = INADDR_ANY;

        int rv = ::bind(fd, (struct sockaddr*)&addr, sizeof(addr));
        if (rv == -1) {
            throw runtime_error(string("bind() failed: ") + strerror(errno));
        }

        rv = ::listen(fd, SOMAXCONN);
        if (rv == -1) {
            throw runtime_error(string("listen() failed: ") + strerror(errno));
        }

        cout << "Server listening on port " << port << "..." << endl;
    }

    void close_server() {
        if (close(fd) == -1) {
            throw runtime_error(string("Failed to close socket: ") + strerror(errno));
        }
        cout << "Socket closed successfully." << endl;
    }



    // destructor for automatic cleanup

    ~Express() {  
        try {
            close_server();
        } catch (...) {
            // Ignore errors during cleanup
        }
    }
};

int main() {
    try {
        Express server;
        server.listen(3000); // start server on port 8080

        while (true) {
            struct sockaddr_in client_address = {};
            socklen_t address_len = sizeof(client_address);

            int connfd = accept(server.get_fd(), (struct sockaddr*)&client_address, &address_len);

            if (connfd < 0) {
                cerr << "Accept failed: " << strerror(errno) << endl;
                continue;
            }

            cout<<connfd<<endl;
            // Simple message to the client
            string msg =
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/plain\r\n"
                "Connection: close\r\n"
                "\r\n"
                "Hello from C++ Express server!";

            write(connfd, msg.c_str(), msg.size());
            close(connfd); // close connection after responding
        }

    } catch (const exception &e) {
        cerr << "Fatal error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
