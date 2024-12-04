#include <iostream>
#include <fstream>
#include "RecommendCommand.h"
#include "FileDataBase.h"
#include "ICommand.h"
#include "App.h"
#include "Add.h"

int main() {
   
    std::string databaseFile = "/app/data/database.txt";
  
    std::ifstream fileCheck(databaseFile);
    bool fileExists = fileCheck.good();


    std::ofstream file(databaseFile, 
    fileExists ? std::ios::app : std::ios::out);



    std::map<std::string, ICommand*> commands;
    FileDataBase* data=new FileDataBase();
    DataBaseServices* dbservices = new DataBaseServices(data);
    
    data->setfilepath(databaseFile);
    ICommand* recommendCommand = new RecommendCommand(data,dbservices);
    ICommand* addCommand = new Add(data,dbservices);
    commands["recommend"] = recommendCommand;
    commands["add"] = addCommand;

    App app(commands);
    app.run();
    
    delete recommendCommand;

    return 0;
}
