#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <netinet/in.h>
#include <unistd.h>
#include <stdexcept>
#include <cerrno>
#include <cstring>
#include<fstream>
#include<signal.h>
#include "./../include/request.h"
#include "./../include/response.h"
#include "./../include/server.h"

int main() {
    signal(SIGPIPE, SIG_IGN); 
    try {
        Express app;
        app.listen(4000); 

        

        app.get("/", [](Request &req, Response &res){
            res.sendFile("./../test/index.html");
        });


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
            
            cout<<method<<" "<<path<<endl;
            auto handler = app.findRoute(method, path);

            if(handler){
                handler(req, res);
            }
            else {
                const string location = "./../test" + path;
                ifstream file(location);

                if(file.good()){
                    res.sendFile(location);
                }
                else{
                    res.send("Not Found", "404");
                }
            }
            close(connfd);
        }

    } catch (const exception &e) {
        cerr << "Fatal error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
