#ifndef IDATABASE_H
#define IDATABASE_H

#include <vector>
#include <string>
#include "User.h"  
#include "Movie.h"

class IDataBase {
public:

    // Pure virtual functions
    virtual void UpdateLocalDataBase() = 0;
    virtual void addNewUser(User& NewUser) = 0;
    virtual void addNewMovies(int UserID, std::vector<Movie>& movies) = 0;
    virtual std::vector<User>& getUsers() = 0;
};

#endif // IDATABASE_H