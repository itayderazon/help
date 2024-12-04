#include "ParseInput.h"
using namespace std;

pair<string, vector<string>> ParseInput::run(const string& inputLine) {
    stringstream ss(inputLine);
    string command;
    ss >> command;

    vector<string> args;
    string arg;
    while (ss >> arg) {
        args.push_back(arg);
    }

    return {command, args};
}