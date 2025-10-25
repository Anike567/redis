#include <iostream>
#include <unistd.h>     
#include <sys/wait.h> 

using namespace std;

int main(void) {
    while (true) {
        string msg;
        cout << "Enter a message (or 'exit' to quit): ";
        getline(cin, msg);

        if (msg == "exit") break;

        pid_t pid = fork();  // create a child process

        if (pid < 0) {
            cerr << "Fork failed!" << endl;
            return 1;
        }

        if (pid == 0) {
            // Child process
            char* args[] = {(char*)"./client", (char*)msg.c_str(), nullptr};
            execvp(args[0], args);

            // If execvp returns, it means it failed
            cerr << "Exec failed!" << endl;
            return 1;
        } else {
            // Parent process waits for child to finish
            int status;
            waitpid(pid, &status, 0);
            cout << "Child process finished with status " << status << endl;
        }
    }

    return 0;
}
