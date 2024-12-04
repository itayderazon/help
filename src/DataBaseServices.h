#ifndef DATABASESERVICES_H // Include guard to prevent multiple inclusions
#define DATABASESERVICES_H
#include "IDataBase.h"
#include "User.h"
#include "Movie.h"

class DataBaseServices {
    private:
    
    IDataBase* db;
    
    public:

    DataBaseServices(IDataBase* database);

    User* getUserByID(int id);

    std::vector<User>& getUsers();

    bool movieExists(int movieID);

    bool hasWatchedMovie(int userID, int movieID);
};

#endif