#include "App.h"
#include <iostream>

using namespace std;
App::App(std::map<std::string, ICommand*> commands)
    : commands(commands) {}




void App::run() {
    while (true) {
        string inputLine = input->getInput(); // Fetch input from IInput
        auto [command, args] = parseinput->run(inputLine);
        
        if (commands[command] != nullptr) {
            commands[command]->execute(args);
        }
       
    }
}
