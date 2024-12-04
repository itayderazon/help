
#include "Input.h"

    
std::string Input::getInput(){
        std::string input;
        std::getline(std::cin, input);
        return input;
    };