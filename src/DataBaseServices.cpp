#include "DataBaseServices.h"

DataBaseServices::DataBaseServices(IDataBase *database) : db(database) {}

User* DataBaseServices::getUserByID(int id)
{
    std::vector<User>& Users = getUsers(); // Reference to the vector of users
    for (size_t i = 0; i < Users.size(); ++i) { // Loop through the vector
        if (Users[i].getUserID() == id) { // Check if the user ID matches
            return &Users[i]; // Return a pointer to the matched user
        }
    }
    return nullptr; // Return nullptr if no match is found
}

std::vector<User>& DataBaseServices::getUsers()
{
    return db->getUsers(); // Return the vector of users from the database
}
bool DataBaseServices::hasWatchedMovie(int userID, int movieID) {
    User* user = getUserByID(userID);
    
    if (user == nullptr) {
        return false;
    }

    const std::vector<Movie>& userMovies = user->getMovies();

    // Manual loop to find the movie
    for (size_t i = 0; i < userMovies.size(); ++i) {
        if (userMovies[i].getMovieID() == movieID) {
            return true; // Movie found
        }
    }

    return false; // Movie not found
}

bool DataBaseServices::movieExists(int movieID) {
    const std::vector<User>& users = db->getUsers();

    for (size_t i = 0; i < users.size(); ++i) {
        const std::vector<Movie>& userMovies = users[i].getMovies();

        // Manual loop to find the movie
        for (size_t j = 0; j < userMovies.size(); ++j) {
            if (userMovies[j].getMovieID() == movieID) {
                return true; // Movie found
            }
        }
    }

    return false; // Movie not found
}


