#ifndef MOVIES_H
#define MOVIES_H

#include <string>
#include <vector>
#include <map>

struct Movie {
    std::string name;
    double rating;

    Movie();
    Movie(const std::string& name, double rating);
    bool operator<(const Movie& other) const;
};

void printSortedMovies(const std::map<std::string, Movie>& movieMap);
void processPrefixes(const std::map<std::string, Movie>& movieMap, const std::vector<std::string>& prefixes);

#endif
