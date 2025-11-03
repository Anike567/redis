#include "./../include/response.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cerrno>
#include <cstring>
#include <sys/socket.h>
#include<sys/stat.h>
#include <fcntl.h>  

Response::Response(int conn_fd) : connfd(conn_fd) {
    headers["Server"] = "CppServer/1.0";
    headers["Connection"] = "close";
}

void Response::setHeader(const string &key, const string &value) {
    headers[key] = value;
}

int Response::send(const string &body, const string &status, const string &contentType) {
    // Compute Content-Length
    headers["Content-Type"] = contentType;
    headers["Content-Length"] = to_string(body.size());

    // Start response
    string response = "HTTP/1.1 " + status + "\r\n";

    // Append headers
    for (const auto &[key, value] : headers) {
        response += key + ": " + value + "\r\n";
    }

    // End of headers
    response += "\r\n";

    // Append body
    response += body;

    // Send all
    ssize_t n = write(connfd, response.c_str(), response.size());
    if (n < 0) {
        cerr << "Write failed: " << strerror(errno) << endl;
        return -1;
    }

    return n;
}

int Response::sendFile(const string &path) {
    // Determine content type
    string contentType = "application/octet-stream";
    if (path.find(".html") != string::npos)
        contentType = "text/html";
    else if(path.find(".css") != string::npos){
        contentType = "text/css";
    }
    else if(path.find(".js") != string::npos){
        contentType = "application/javascript";
    }
    else if (path.find(".json") != string::npos)
        contentType = "application/json";
    else if (path.find(".png") != string::npos)
        contentType = "image/png";
    else if (path.find(".jpg") != string::npos || path.find(".jpeg") != string::npos)
        contentType = "image/jpeg";
    else if (path.find(".mp3") != string::npos)
        contentType = "audio/mpeg";
    else if (path.find(".mp4") != string::npos)
        contentType = "video/mp4";
    else if (path.find(".pdf") != string::npos)
        contentType = "application/pdf";

    struct stat st;
    if (stat(path.c_str(), &st) < 0) {
        perror("stat failed");
        return -1;
    }

    // Build header
    stringstream headerStream;
    headerStream << "HTTP/1.1 200 OK\r\n";
    headerStream << "Content-Type: " << contentType << "\r\n";
    headerStream << "Content-Length: " << st.st_size << "\r\n";
    headerStream << "Connection: close\r\n";
    headerStream << "\r\n";

    string headerStr = headerStream.str();

    if (write(connfd, headerStr.c_str(), headerStr.size()) < 0) {
        perror("header write failed");
        return -1;
    }

    int file_fd = open(path.c_str(), O_RDONLY);
    if (file_fd < 0) {
        perror("open file failed");
        return -1;
    }

    char buffer[4096];
    ssize_t n;
    while ((n = read(file_fd, buffer, sizeof(buffer))) > 0) {
        ssize_t total_written = 0;
        while (total_written < n) {
            ssize_t written_bytes = write(connfd, buffer + total_written, n - total_written);

              if (written_bytes < 0) {
                if (errno == EPIPE) {
                    cerr << "Client closed connection early\n";
                    close(file_fd);
                    return -1;
                }
                perror("write failed");
                close(file_fd);
                return -1;
            }
            total_written += written_bytes;
        }
    }

  


    close(file_fd);
    
    shutdown(connfd, SHUT_WR);
    return 0;
}
