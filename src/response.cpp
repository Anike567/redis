#include "./../include/response.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cerrno>
#include <cstring>
#include <sys/socket.h>

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
    ifstream file(path, ios::in | ios::binary);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << path << endl;
        return -1;
    }

    // Determine content type
    string contentType = "application/octet-stream";
    if (path.find(".html") != string::npos)
        contentType = "text/html";
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

    // Get file size
    file.seekg(0, ios::end);
    size_t fileSize = file.tellg();
    file.seekg(0, ios::beg);

    // Build header
    stringstream headerStream;
    headerStream << "HTTP/1.1 200 OK\r\n";
    headerStream << "Content-Type: " << contentType << "\r\n";
    headerStream << "Content-Length: " << fileSize << "\r\n";
    headerStream << "Connection: close\r\n";
    headerStream << "\r\n";

    string headerStr = headerStream.str();

    // Send header
    if (write(connfd, headerStr.c_str(), headerStr.size()) < 0) {
        cerr << "Error writing headers: " << strerror(errno) << endl;
        file.close();
        return -1;
    }

    // Send file in chunks
    char buffer[4096];
    while (file.read(buffer, sizeof(buffer)) || file.gcount() > 0) {
        ssize_t bytesSent = write(connfd, buffer, file.gcount());
        if (bytesSent < 0) {
            cerr << "Error sending file: " << strerror(errno) << endl;
            file.close();
            return -1;
        }
    }

    file.close();
    return 0;
}
