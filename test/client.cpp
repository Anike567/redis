#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <netinet/in.h>
#include <unistd.h>
#include <stdexcept>
#include <cerrno>
#include <cstring>

using namespace std;

static void die(const char *msg) {
    int err = errno;
    fprintf(stderr, "[%d] %s\n", err, msg);
    abort();
}

int main(void){

    int fd = socket(AF_INET, SOCK_STREAM, 0);

    if(fd < 0){
        cout<< "error occured while creating socket"<<endl;
    }

    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(4000);

    addr.sin_addr.s_addr = ntohl(INADDR_LOOPBACK);

    int rv = connect(fd, (struct sockaddr *) &addr, sizeof(addr));

    if(rv){
        die("connect");
    }

    char buff[64] = {};
    string msg = "Hello from cliend";

    write(fd, msg.c_str(), msg.size());

    ssize_t n = read(fd, buff, sizeof(buff));

    printf("server says: %s\n", buff);
    close(fd);


    return 0;
}