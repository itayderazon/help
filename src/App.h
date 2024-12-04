#pragma once

#include <map>
#include <string>
#include <vector>
#include "ICommand.h"
#include "IInput.h"
#include "Input.h"
#include <sstream>
#include "ParseInput.h"

class App {
private:
    std::map<std::string, ICommand*> commands;
    IInput* input=new Input();
    ParseInput* parseinput=new ParseInput();

public:
    App(std::map<std::string, ICommand*> commands);

    void run();
};

