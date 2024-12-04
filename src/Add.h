#ifndef ADD_H
#define ADD_H

#include <vector>
#include <string>
#include "ICommand.h"
#include "IDataBase.h"
#include "User.h"
#include "Movie.h"
#include "DataBaseServices.h"
#include "FileDataBase.h"

using std::string;

class Add : public ICommand {
private:
    string user_input;
    IDataBase* myDataBase; // Allows access to database methods
    DataBaseServices* dbservices;

public:
    Add() = default;
    Add(const string& input, IDataBase* db) : user_input(input), myDataBase(db) {}

    void setUserInput(const string& input);
    void setDataBase(IDataBase* db);
    bool IsValidInput(std::vector<string> args );
    void addNewUser(int userId, const std::vector<Movie>& movies);
    void addNewMovies(int userId, const std::vector<Movie>& movies);
    void execute(std::vector<std::string>) override;
    void printcommand();
    explicit Add(FileDataBase* db,DataBaseServices* dbservices);
};

#endif
