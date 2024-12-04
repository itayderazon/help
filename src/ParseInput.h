#pragma once

#include <string>
#include <vector>
#include <utility> 
#include <sstream>

class ParseInput {
public:
    
    std::pair<std::string, std::vector<std::string>> run(const std::string& inputLine);
};
