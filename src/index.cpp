#include <iostream>
#include <map>
#include <sstream>

#include "../include/parse_command.hpp"
using namespace std;


int main() {
    map<string, string> data; 
    bool will = true;

    while (will) {
        cout << ">>> ";
        string line;
        getline(cin, line);  
        vector<string> tokens = parse_command(line);

        if (tokens.empty()) continue;

        string command = tokens[0];

        if (command == "set") {
            if (tokens.size() < 3) {
                cout << "Usage: set <key> <value>\n";
                continue;
            }
            data[tokens[1]] = tokens[2];
            cout << "OK\n";
        }

        else if (command == "get") {
            if (tokens.size() < 2) {
                cout << "Usage: get <key>\n";
                continue;
            }
            if (data.find(tokens[1]) != data.end())
                cout << data[tokens[1]] << "\n";
            else
                cout << "Key not found\n";
        }

        else if (command == "exit") {
            will = false;
        }

        else {
            cout << "Unknown command\n";
        }
    }

    return 0;
}
