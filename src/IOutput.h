#pragma once

#include <string>


class IOutput {
public:
    virtual void getOutput(const std::string output)  = 0; 
};


