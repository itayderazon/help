
#pragma once

#include <string>
#include <iostream>  
#include "IInput.h"

class Input : public IInput {
public:
    std::string getInput() override;
};

