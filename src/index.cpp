#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <netinet/in.h>
#include <unistd.h>
#include <stdexcept>
#include <cerrno>
#include <cstring>
#include "./../include/request.h"
#include "./../include/response.h"
#include "./../include/server.h"

using namespace std;

int main() {
    map<string, string> mp;
    try {
        Express app;
        app.listen(4000); 

        while (true) {
            struct sockaddr_in client_address = {};
            socklen_t address_len = sizeof(client_address);

            int connfd = accept(app.get_fd(), (struct sockaddr*)&client_address, &address_len);
            
            if (connfd < 0) {
                cerr << "Accept failed: " << strerror(errno) << endl;
                continue;
            }

            app.setConnFd(connfd);
            Request req(connfd);
            Response res(connfd);
            string method = req.getMethod();
            string path = req.getPath();

            cout<<method << "  "<<path<<endl;

            map<string, string> body = req.getBody();
            if (auto it = body.find("command"); it != body.end())
                cout << it->first << " " << it->second << endl;

            if (auto it = body.find("key"); it != body.end())
                cout << it->first << " " << it->second << endl;

            string msg = "Message from redis server";

            res.send(msg);

            
        }

    } catch (const exception &e) {
        cerr << "Fatal error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
