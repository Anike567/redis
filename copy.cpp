#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <fcntl.h>   
#include<sys/stat.h>

using namespace std;

int main(void) {
    string from, to;
    cout << "Enter file name and destination directory:\n";
    string input;
    getline(cin, input);

    istringstream ss(input);
    ss >> from >> to;

    int from_fd = open(from.c_str(), O_RDONLY);
    if (from_fd < 0) {
        perror("Open source failed");
        return 1;
    }
    struct stat st;

    if(stat(from.c_str(), &st) == 0){
        cout << "File size" << st.st_size << "bytes" <<endl;
    
    }
    else{
        perror("Something went wrong");
    }

    string destPath = to + "/song.mp3";
    int to_fd = open(destPath.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (to_fd < 0) {
        perror("Open destination failed");
        close(from_fd);
        return 1;
    }

    char buff[4096];
    ssize_t n;
    while ((n = read(from_fd, buff, sizeof(buff))) > 0) {
        ssize_t written = write(to_fd, buff, n);
        if (written < 0) {
            perror("Write failed");
            close(from_fd);
            close(to_fd);
            return 1;
        }
    }

    if (n < 0) {
        perror("Read failed");
    }

    close(from_fd);
    close(to_fd);

    cout << "File copied successfully!\n";
    return 0;
}