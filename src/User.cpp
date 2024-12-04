#include "User.h"

// Getter and Setter for UserID
void User::setUserID(int id)
{
    this->UserID = id;
}

int User::getUserID() const
{
    return UserID;
}

// Getter and Setter for movies
void User::setMovies(const std::vector<Movie>& movieList) {
    this->movies = movieList;
}

const std::vector<Movie>&User::getMovies() const {
    return movies;
}

//operator < for User
bool User::operator<(const User& other) const {
        return UserID < other.getUserID();
}