#include "Movie.h"
Movie::Movie(int id){
    this->movieid = id;
}

// Setter for movieid
void Movie::setMovieID(int id) {
    movieid = id;
}

// Getter for movieid
int Movie::getMovieID() const {
    return movieid;
}
