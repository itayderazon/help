#include "Add.h"
#include <sstream>
#include <cctype>
#include <iostream>

using namespace std;

Add::Add(FileDataBase* db,DataBaseServices* dbservices) {
    this->myDataBase=db;
    this->dbservices=dbservices;
}
void Add::setUserInput(const string& input) {
    user_input = input;
}

void Add::setDataBase(IDataBase* db) {
    myDataBase = db;
}

bool Add::IsValidInput(std::vector<string> args ) {
    
    if(args.size()<2){
        return false;
    }

    // If user input contains any character other than digits,return false
    for (int i = 1; i < args.size(); i++) {
        for(char c:args[i]){
            if (!isdigit(c)) {
            return false;
        }
        }
    }

    return true;
}

// Create a new user and add them with their watch list to the database
void Add::addNewUser(int userId, const std::vector<Movie>& movies) {
    User newUser;
    newUser.setUserID(userId);
    newUser.setMovies(movies);
    myDataBase->addNewUser(newUser);
}

// Add new movies to an existing user
void Add::addNewMovies(int userId, const std::vector<Movie>& addMovies) {
    DataBaseServices dbServices(myDataBase);
    User* myUser = dbServices.getUserByID(userId); // Get user from database
    if (!myUser) {
        return; // User not found
    }

    std::vector<Movie> filteredMovies;

    for (const auto& addMovie : addMovies) { // For every movie in the add command:
        if (!dbServices.hasWatchedMovie(userId, addMovie.getMovieID())) {
            filteredMovies.push_back(addMovie);
        }
    }

    // Add the movies to the user's list
    myDataBase->addNewMovies(userId, filteredMovies);
}
void Add::printcommand(){

}
void Add::execute(std::vector<std::string> args) {
    // Input validation
    if(!IsValidInput(args)){
        return;
    }
    int movieId;
    string userIdWord=args[0];
    int userId = std::stoi(userIdWord);

    // Create movie list from input
    std::vector<Movie> movies;
    string word;

    for(int i=1;i<args.size();i++){
        movieId = std::stoi(args[i]);
        Movie movie(movieId);
        movies.push_back(movie);
    }

    

    // If the user already exists, add the movies list to their current list
    if (dbservices->getUserByID(userId)!=nullptr) {
        addNewMovies(userId, movies);
    }
    // Else, create a new user with the watch list and add to the database
    else {
        addNewUser(userId, movies);
    }
}
