#ifndef MOVIE_H // Include guard to prevent multiple inclusions
#define MOVIE_H

class Movie {
private:
    int movieid; // Movie ID

public:
    Movie(int id); // Default constructor
    Movie() = default;

    // Setter for movieid
    void setMovieID(int id);

    // Getter for movieid
    int getMovieID() const;
};

#endif // MOVIE_H
