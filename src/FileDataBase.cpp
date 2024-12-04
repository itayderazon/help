
#include "FileDataBase.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

//reads from file and updates local data base
void FileDataBase::UpdateLocalDataBase()
{
    //trying to open file
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file");
    }

    // clean local data base
    Users.clear();
    
    std::string line;
    //read each line(different user) in file and add to local data base
    while (std::getline(file, line)) {
        //get id of user
        std::istringstream iss(line);
        int userID;
        iss >> userID;

        //build user
        User user;
        user.setUserID(userID);

        //get movies of user
        std::vector<Movie> userMovies;
        int movieID;
        while (iss >> movieID) {
            Movie movie;
            movie.setMovieID(movieID);
            userMovies.push_back(movie);
        }
        //add movies to user
        user.setMovies(userMovies);
        // add user to local data base
        Users.push_back(user);
    }
}

// return users
std::vector<User> &FileDataBase::getUsers()
{
    return Users;
}

// given userid and movie vector, this function adds the new movies to userid in database 
void FileDataBase::addNewMovies(int UserID, std::vector<Movie>& movies) {
    // trying to open file
    std::fstream file(filePath, std::ios::in | std::ios::out | std::ios::app);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file: " + filePath);
    }

    // Check if user already exists in the file
    std::string line;
    std::vector<std::string> fileLines;
    bool userFound = false;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        int fileUserID;
        iss >> fileUserID;

        if (fileUserID == UserID) {
            // User found; add new movies to the existing line
            std::ostringstream oss;
            oss << line;
            //getting user's movies
            for (const auto& movie : movies) {
                oss << " " << movie.getMovieID();
            }

            fileLines.push_back(oss.str());
            userFound = true;
        } else {
            fileLines.push_back(line);
        }
    }

    // If the user wasn't found, return error
    if (!userFound) {
        throw std::runtime_error("User: " + std::to_string(UserID) + " doesn't exist");    
    }

    // Rewrite the entire file
    file.close();
    std::ofstream outFile(filePath, std::ios::trunc);
    if (!outFile.is_open()) {
        throw std::runtime_error("Unable to open file for writing: " + filePath);
    }

    for (const auto& line : fileLines) {
        outFile << line << "\n";
    }
    outFile.close();
    // update local data base
    UpdateLocalDataBase();
}

// adds new user to data base
void FileDataBase::addNewUser(User &NewUser) {
    UpdateLocalDataBase();
    // Check if the user already exists in the local database
    for (const auto& userEntry : Users) {
        if (userEntry.getUserID() == NewUser.getUserID()) {
            throw std::runtime_error("User with ID " + std::to_string(NewUser.getUserID()) + " already exists.");
        }
    }

    // Add the user to the file
    std::ofstream file(filePath, std::ios::app); // Open file in append mode
    if (file.is_open()) {
        file << NewUser.getUserID();
        //adding movies of user
        for (const auto& movie : NewUser.getMovies()) {
            file << " " << movie.getMovieID();
        }
        file << "\n";
        file.close();

    } else {
        throw std::runtime_error("Could not open file to add new user.");
    }
    //updating local database
    UpdateLocalDataBase();
}

void FileDataBase::setfilepath(std::string const filePath)
{
    this->filePath = filePath;
}




