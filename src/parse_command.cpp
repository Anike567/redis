#include "../include/parse_command.hpp"
#include <sstream>
#include <vector>
#include <string>

using namespace std;

vector<string> parse_command(const string& command) {
    stringstream ss(command);
    string word;
    vector<string> command_and_args;

    while (ss >> word) {
        command_and_args.push_back(word);
    }

    return command_and_args;
}
