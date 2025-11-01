#include "./../include/request.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdexcept>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sstream>
#include <algorithm>

using namespace std;

Request::Request(int conn_fd) {
    connfd = conn_fd;
    generateRequestString();
    parseRequest();
}

// return the method is the request is get or post



// Read raw HTTP request from socket
void Request::generateRequestString() {
    char buff[1024];
    ssize_t n;

    while ((n = read(connfd, buff, sizeof(buff))) > 0) {
        request.append(buff, n);
        if (n < sizeof(buff)) break;
    }

    if (n < 0) {
        cerr << "Error reading from socket: " << strerror(errno) << endl;
    }
}

// Trim helper
string Request::trim(const string &s) {
    size_t start = s.find_first_not_of(" \r\n\t");
    size_t end = s.find_last_not_of(" \r\n\t");
    return (start == string::npos) ? "" : s.substr(start, end - start + 1);
}

// Parse full HTTP request
void Request::parseRequest() {
    size_t pos = request.find("\r\n\r\n");
    if (pos == string::npos) {
        cerr << "Invalid HTTP request format (no header-body separator)" << endl;
        return;
    }

    string headerPart = request.substr(0, pos);
    string bodyPart = request.substr(pos + 4); // skip "\r\n\r\n"

    parseHeaders(headerPart);
    parseBody(bodyPart);
}

// Parse request line + headers
void Request::parseHeaders(const string& headerPart) {
    istringstream stream(headerPart);
    string line;

    // First line: "POST / HTTP/1.1"
    if (getline(stream, line)) {
        istringstream first(line);
        first >> method >> path;
    }

    // Other lines: "Header-Name: value"
    while (getline(stream, line)) {
        if (line.empty() || line == "\r") continue;

        size_t sep = line.find(":");
        if (sep == string::npos) continue;

        string key = trim(line.substr(0, sep));
        string value = trim(line.substr(sep + 1));

        header[key] = value;
    }
}

// Parse JSON-like body into key-value pairs
void Request::parseBody(const string& bodyPartRaw) {
    string bodyPart = trim(bodyPartRaw);

    if (bodyPart.empty()) return;

    // Remove braces if JSON-style
    if (bodyPart.front() == '{') bodyPart.erase(0, 1);
    if (bodyPart.back() == '}') bodyPart.pop_back();

    // Replace commas with newlines for easy splitting
    for (char &c : bodyPart) {
        if (c == ',') c = '\n';
    }

    istringstream iss(bodyPart);
    string line;

    while (getline(iss, line)) {
        line = trim(line);
        if (line.empty()) continue;

        size_t sep = line.find(":");
        if (sep == string::npos) continue;

        string key = trim(line.substr(0, sep));
        string value = trim(line.substr(sep + 1));

        // Remove quotes if present
        if (!key.empty() && key.front() == '"') key.erase(0, 1);
        if (!key.empty() && key.back() == '"') key.pop_back();
        if (!value.empty() && value.front() == '"') value.erase(0, 1);
        if (!value.empty() && value.back() == '"') value.pop_back();

        body[key] = value;
    }
}

string Request::getBody(string key) {
    if (body.find(key) != body.end()) {
        return body[key];
    }
    return "undefined";
}

string Request::getHeader(string name) {
    if (header.find(name) != header.end()) {
        return header[name];
    }
    return "";
}

string Request::getMethod() {
    return method;
}

string Request::getPath() {
    return path;
}
