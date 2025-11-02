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

int main() {
    try {
        Express app;
        app.listen(4000); // start server on port 8080

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
            const string method = req.getMethod();
            const string path = req.getPath();

            cout<<method << "  "<<path<<endl;

            map<string, string> body = req.getBody();

            for(auto itr : body){
                cout<<itr.first << "  "<< itr.second<<endl;
            }
            if(path == "/" && method == "GET"){
                res.sendFile("./../test/index.html");
            }
            else if(path == "/style.css" && method == "GET"){
                res.sendFile("./../test/style.css");
            }
            else{
                res.send("Not Found", "404");
            }
        }

    } catch (const exception &e) {
        cerr << "Fatal error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
