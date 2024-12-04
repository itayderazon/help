#include "RecommendCommand.h"
#include <stdexcept>
#include <sstream>
#include <iostream>

using namespace std;

RecommendCommand::RecommendCommand(FileDataBase* db,DataBaseServices* dbservices) {
    this->db=db;
    this->dbservices=dbservices;
}

vector<pair<int, int>> RecommendCommand::calculateSimilarity(int userId) {
    vector<pair<int, int>> similarity;

    // Get the target user and their movies
    User* targetUser = dbservices->getUserByID(userId);
    const vector<Movie>& targetMovies = targetUser->getMovies();

    // Calculate similarity
    for (const User& user : db->getUsers()) {
        if (user.getUserID() == userId) continue;

        int commonCount = 0;
        for (const auto& movie : user.getMovies()) {
            for (const auto& targetMovie : targetMovies) {
                if (movie.getMovieID() == targetMovie.getMovieID()) {
                    ++commonCount;
                    break;
                }
            }
        }
        similarity.push_back(make_pair(user.getUserID(), commonCount));
    }

    return similarity;
}

vector<pair<int, int>> RecommendCommand::calculateRelevance(const vector<pair<int, int>>& similarity, int targetMovie, int userId) {
    vector<pair<int, int>> movieWeights;

    // Get the target user and their movies
    User* targetUser = dbservices->getUserByID(userId);
    const vector<Movie>& targetMovies = targetUser->getMovies();

    // Iterate through users and calculate relevance
    for (const auto& simPair : similarity) {
        User* user = dbservices->getUserByID(simPair.first);
        bool watchedTargetMovie = false;

        // Check if the user has watched the target movie
        for (const auto& movie : user->getMovies()) {
            if (movie.getMovieID() == targetMovie) {
                watchedTargetMovie = true;
                break;
            }
        }

        if (!watchedTargetMovie) continue;

        for (const auto& movie : user->getMovies()) {
            int movieID = movie.getMovieID();
            bool alreadyWatched = false;

            for (const auto& targetMovie : targetMovies) {
                if (movieID == targetMovie.getMovieID()) {
                    alreadyWatched = true;
                    break;
                }
            }

            if (movieID != targetMovie && !alreadyWatched) {
                bool found = false;

                for (auto& weight : movieWeights) {
                    if (weight.first == movieID) {
                        weight.second += simPair.second;
                        found = true;
                        break;
                    }
                }

                if (!found) {
                    movieWeights.push_back(make_pair(movieID, simPair.second));
                }
            }
        }
    }

    // Sort movies by weight and then by ID in descending order
    for (size_t i = 0; i < movieWeights.size(); ++i) {
        for (size_t j = i + 1; j < movieWeights.size(); ++j) {
            if (movieWeights[i].second < movieWeights[j].second ||
                (movieWeights[i].second == movieWeights[j].second && movieWeights[i].first > movieWeights[j].first)) {
                pair<int, int> temp = movieWeights[i];
                movieWeights[i] = movieWeights[j];
                movieWeights[j] = temp;
            }
        }
    }

    return movieWeights;
}

vector<int> RecommendCommand::Recommend(int userId, int targetMovie) {
    db->UpdateLocalDataBase();

    // Generate similarity and relevance
    vector<pair<int, int>> similarity = calculateSimilarity(userId);
    vector<pair<int, int>> relevance = calculateRelevance(similarity, targetMovie, userId);

    // Extract movie IDs from relevance
    vector<int> recommendedMovies;
    
    int limit = min(static_cast<int>(relevance.size()), 10);
    for (int i = 0; i < limit; ++i) {
        recommendedMovies.push_back(relevance[i].first);
    }

    return recommendedMovies;
}

void RecommendCommand::printcommand() {
    output->getOutput("recommend[userid][movieid]");
    delete output;
}


bool RecommendCommand::checkInput(const string& userIdStr, const string& movieIdStr) {
   int userId,movieId ;
    
    try{
        db->UpdateLocalDataBase();
        if ( userIdStr.empty() || movieIdStr.empty()) {
            return false;
        }

    // Check if userIdStr and movieIdStr are integers
        for (char c : userIdStr) {
            if (!isdigit(c)) {
                return false;
            }
    }
        for (char c : movieIdStr) {
            if (!isdigit(c)) {
                return false;
            }
    }

        // Convert strings to integers
        userId = stoi(userIdStr);
        movieId = stoi(movieIdStr);

    // Validate user ID and movie ID directly
        if (dbservices->getUserByID(userId) == nullptr) {
            return false;
         }

        if (!dbservices->movieExists(movieId)) {
            return false;
        }
    }
    catch(...){
        return false;
    }

    return true;
}




void RecommendCommand::execute(vector<string> args) {
    int movieId,userId;
    if(args.size()!=2){
        return;
    }
    if (!checkInput(args[0], args[1])) {
        return;
    }
    userId = stoi(args[0]);
    movieId = stoi(args[1]);
    // Generate recommendations based on the userId and movieId
    vector<int> recommendedMovies = Recommend(userId, movieId);
    for (int movieID : recommendedMovies) {
        output->getOutput(to_string(movieID));
        output->getOutput(" ");
    }
    output->getOutput("\n");
}
