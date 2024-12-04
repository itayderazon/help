#ifndef FILEDATABASE_H
#define FILEDATABASE_H

#include <map>
#include <vector>
#include <string>
#include "IDataBase.h"
#include "User.h"       // Include the User class definition
#include "Movie.h"      // Include the Movie class definition

class FileDataBase : public IDataBase {
private:
     // local database - vector of all users
     std::vector<User> Users;
     //filepath to database txt file
     std::string filePath;
     
public:
    // Constructor (if needed)
    FileDataBase() = default;

    // Override methods from IDataBase
    void UpdateLocalDataBase();

    std::vector<User>& getUsers();

    // Additional methods
    void addNewMovies(int UserID, std::vector<Movie>& movies);

    void addNewUser(User& NewUser);
    
    void setfilepath(std::string const filePath);

  };

#endif // FILEDATABASE_H