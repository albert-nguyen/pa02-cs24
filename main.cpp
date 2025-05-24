#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include "movies.h"
#include <chrono>

using namespace std;

bool parseLine(string& line, string& movieName, double& movieRating);

int main(int argc, char** argv) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " moviesFilename [prefixFilename]" << endl;
        return 1;
    }

    ifstream movieFile(argv[1]);
    if (!movieFile) {
        cerr << "Could not open file " << argv[1] << endl;
        return 1;
    }

    map<string, Movie> movieMap;
    string line, movieName;
    double movieRating;

    while (getline(movieFile, line) && parseLine(line, movieName, movieRating)) {
        movieMap[movieName] = Movie(movieName, movieRating);
    }
    movieFile.close();

    if (argc == 2) {
        printSortedMovies(movieMap);
        return 0;
    }

    ifstream prefixFile(argv[2]);
    if (!prefixFile) {
        cerr << "Could not open file " << argv[2] << endl;
        return 1;
    }

    vector<string> prefixes;
    while (getline(prefixFile, line)) {
        if (!line.empty()) {
            prefixes.push_back(line);
        }
    }
    prefixFile.close();

    //auto start = std::chrono::high_resolution_clock::now();

    processPrefixes(movieMap, prefixes);

    //auto end = std::chrono::high_resolution_clock::now();
    //std::chrono::duration<double, std::milli> duration = end - start;

    //std::cout << "Time taken: " << duration.count() << " ms" << std::endl;

    return 0;
}

/*

Time Complexity Analysis PART 3A

O(n) for all movies. std::map so search and insertion is O(logn). While loop goes over k max entries that fit prefix so O(k). However relies on l length of prefix for find() so o(kl).
Push_back is O(1). All prefixes is O(m). Sort is O(klogk). So Worst time complexity is O(n + m(logn + k * l + klogk)). 

input_20_random.csv using prefix_large.txt:

Time taken: 1) 330.2 ms 2) 204.6 ms 3) 215.6 ms 4) 207.5 ms 5) 223.1 ms

input_100_random.csv using prefix_large.txt:

Time taken: 1) 220.7 ms 2) 207.8 ms 3) 279.5 ms 4) 293.3 ms 5) 232.8 ms

input_1000_random.csv using prefix_large.txt:

Time taken: 1) 322.7 ms 2) 209.9 ms 3) 315.6 ms 4) 348.6 ms 5) 330.9 ms

input_76920_random.csv using prefix_large.txt:

Time taken: 1) 1051.2 ms 2) 1176.1 ms 3) 1023.1 ms 4) 962.1 ms 5) 1054.7 ms

Yes my data matches my initial guess in time complexity.

Space Complexity Analysis PART 3B

movieMap map<string, Movie> holds all movies n with key string l characters so O(n * l). 
Prefixes vector<string> holds m prefixes up to l characters so O(m * l). 
matched vector<Movie> temporarily holds movies matching prefixes so O(n * l). 
bestMovies and foundPrefixes hold up to m entries in each up to l characters so O(m * l).
So worst space complexity is O(n * l + m * l).

Tradeoffs Between Time/Space Complexity Part 3C

Algorithm Design Summary:

I designed my algorithm primarily for low time complexity to ensure efficient search and output for 
large datasets and many prefixes. The main operations use a sorted map<string, Movie> 
to allow fast prefix-based lookups with lower_bound, which gives O(log n) access 
per prefix. I also sorted matched movies to get the highest-rated one, 
achieving O(k log k) for sorting at most k matches per prefix.

Target Complexities:
- Time Complexity: O(n + m(logn + k * l + klogk)) — fast prefix searching and best-movie extraction
- Space Complexity: O(n * l + m * l) — reasonably compact storage of movie titles and prefix results

Were both goals achieved?

Yes, I was able to achieve both low time and low space complexity:
- The movie map only stores one copy of each movie name and rating — no unnecessary duplication.
- The matched movies are stored only temporarily during iteration and re-used for each prefix.
- Using `map` ensures logarithmic-time lookup while maintaining space proportional to the number of elements.

Which was harder to achieve?

Low time complexity was harder to achieve, especially while maintaining correctness in prefix 
matching and ensuring the output format was correct. Optimizing both prefix lookup and sorted output 
without increasing memory overhead required careful design decisions. For instance utilizing map instead of vectors.

Overall, the algorithm balances efficiency and clarity while scaling to large inputs.
*/


bool parseLine(string& line, string& movieName, double& movieRating) {
    int commaIndex = line.find_last_of(",");
    movieName = line.substr(0, commaIndex);
    movieRating = stod(line.substr(commaIndex + 1));

    if (!movieName.empty() && movieName[0] == '"') {
        movieName = movieName.substr(1, movieName.length() - 2);
    }

    return true;
}
