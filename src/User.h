#ifndef USER_H
#define USER_H

#include <vector>
#include "Movie.h" // Include the header file for the Movie class

class User {
private:
    int UserID;
    std::vector<Movie> movies;

public:
    // Constructor
    User() = default;

    // Getter and Setter for UserID
    void setUserID(int id);
    int getUserID() const;

    // Getter and Setter for movies
    void setMovies(const std::vector<Movie>& movieList);
    const std::vector<Movie>& getMovies() const;
    bool operator<(const User& other) const;

};

#endif // USER_H
