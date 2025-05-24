#include "movies.h"
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <map>

using namespace std;

Movie::Movie() : name(""), rating(0.0) {}

Movie::Movie(const string& name, double rating)
    : name(name), rating(rating) {}

bool Movie::operator<(const Movie& other) const {
    if (rating != other.rating)
        return rating > other.rating;
    return name < other.name;
}

void printSortedMovies(const map<string, Movie>& movieMap) {
    for (const auto& [name, movie] : movieMap) {
        cout << movie.name << ", " << fixed << setprecision(1) << movie.rating << endl;
    }
}

void processPrefixes(const map<string, Movie>& movieMap, const vector<string>& prefixes) {
    map<string, Movie> bestMovies;
    vector<string> foundPrefixes;  

    for (size_t i = 0; i < prefixes.size(); ++i) {
        const string& prefix = prefixes[i];
        vector<Movie> matched;

        auto it = movieMap.lower_bound(prefix);
        while (it != movieMap.end() && it->first.find(prefix) == 0) {
            matched.push_back(it->second);
            ++it;
        }

        if (matched.empty()) {
            cout << "No movies found with prefix " << prefix << endl;
        } else {
            sort(matched.begin(), matched.end());
            for (const Movie& m : matched) {
                cout << m.name << ", " << fixed << setprecision(1) << m.rating << endl;
            }
            cout << endl;
            bestMovies[prefix] = matched[0];
            foundPrefixes.push_back(prefix);
        }

        if (!matched.empty() && i < prefixes.size() - 1) {
            auto nextPrefix = prefixes[i + 1];
            auto nextIt = movieMap.lower_bound(nextPrefix);
            if (nextIt != movieMap.end() && nextIt->first.find(nextPrefix) == 0) {
                cout << '\n';
            }
        }
    }

    for (const string& prefix : foundPrefixes) {
        const Movie& best = bestMovies[prefix];
        cout << "Best movie with prefix " << prefix << " is: "
             << best.name << " with rating " << fixed << setprecision(1) << best.rating << endl;
    }
}




