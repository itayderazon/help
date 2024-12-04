#pragma once

#include <string>
#include <iostream>  
#include "IOutput.h"

class Output : public IOutput {
public:
    void getOutput(std::string msg);
};

