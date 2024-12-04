#ifndef RECOMMENDCOMMAND_H
#define RECOMMENDCOMMAND_H

#include <vector>
#include <string>
#include "FileDataBase.h"
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include "ICommand.h"
#include <sstream>
#include "Output.h"
#include "Input.h"
#include "IInput.h"
#include "DataBaseServices.h"



class RecommendCommand : public ICommand  {
private:
    FileDataBase* db;

    DataBaseServices* dbservices;

    IOutput* output=new Output();

    IInput* input=new Input();

    std::vector<std::pair<int, int>> calculateSimilarity(int userId);

    std::vector<std::pair<int, int>> calculateRelevance(const std::vector<std::pair<int, int>>& similarity, int targetMovie, int userId);

    

public:
    //class constractor
    explicit RecommendCommand(FileDataBase* data,DataBaseServices* dbservices);
    std::vector<int> Recommend(int userId, int targetMovie);
    bool checkInput(const std::string& userIdStr, const std::string& movieIdStr);
    

    


    virtual void execute(std::vector<std::string>);
    void printcommand();
};

#endif // RECOMMENDCOMMAND_H
