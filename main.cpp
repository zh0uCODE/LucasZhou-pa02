// Winter'26
// Instructor: Diba Mirza
// Student name: Lucas Zhou
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>
#include <cstring>
#include <algorithm>
#include <limits.h>
#include <iomanip>
#include <set>
#include <queue>
#include <sstream>
using namespace std;

#include "utilities.h"
#include "movies.h"

bool parseLine(string &line, string &movieName, double &movieRating);

int main(int argc, char** argv){
    if (argc < 2){
        cerr << "Not enough arguments provided (need at least 1 argument)." << endl;
        cerr << "Usage: " << argv[ 0 ] << " moviesFilename prefixFilename " << endl;
        exit(1);
    }

    ifstream movieFile (argv[1]);
 
    if (movieFile.fail()){
        cerr << "Could not open file " << argv[1];
        exit(1);
    }
  
    // Create an object of a STL data-structure to store all the movies
    set<Movie, CompareMoviesAlpha> movies_alpha; //initalized set, store by rating
    string line, movieName;
    double movieRating;
    // Read each file and store the name and rating
    while (getline (movieFile, line) && parseLine(line, movieName, movieRating)){
        Movie m{movieName, movieRating}; //create object
	movies_alpha.insert(m); //insert       
            // Use std::string movieName and double movieRating
            // to construct your Movie objects
            // cout << movieName << " has rating " << movieRating << endl;
            // insert elements into your data structure
    }

    movieFile.close();

    if (argc == 2){
            //print all the movies in ascending alphabetical order of movie names
            for (const auto& movie : movies_alpha) { //iterate
	        cout << movie.title << ", " << movie.rating << endl; //print line by line!
	    } 
            return 0;
    }

    ifstream prefixFile (argv[2]);

    if (prefixFile.fail()) {
        cerr << "Could not open file " << argv[2];
        exit(1);
    }
    
    //part 2 code:
    vector<string> prefixes;
    while (getline (prefixFile, line)) {
      
        if (!line.empty()) {
            prefixes.push_back(line);
        }
    }
    //2a
    for (auto s : prefixes) { //range for loop
      bool foundMatch = false; //didn't find a match yet
      set<Movie, CompareMovieRating> movies_rating_prefix; //make a subset of rating for one prefix
      for (auto m : movies_alpha) { //ready to check
        if (m.title.starts_with(s)) { //title begins with prefix?
          //cout << m.title << ", " << m.rating << endl; //print
          movies_rating_prefix.insert(m); //insert movies based on prefix
          foundMatch = true; //flag true 
        } 
      }
      if (foundMatch == false) { //still no after searching for prefix
        cout << "No movies found with prefix " << s << endl; //print this
      } else {
        for (auto x : movies_rating_prefix) { //iterate through each subset
          cout << x.title << ", " << x.rating << endl; //print contents
        }
      }
      cout << endl; //extra space
    } 
    //2b
    for (auto s : prefixes) {
      bool foundMatch = false; //didn't find match
      priority_queue<Movie, vector<Movie>, CompareMovieRatingPQ> movies_rating_prefix; //subset, max heap in movies vector
      for (auto m : movies_alpha) { //ready to check
        if (m.title.starts_with(s)) { //title begins with prefix?
          //cout << m.title << ", " << m.rating << endl; //print
          movies_rating_prefix.push(m); //push movies based on prefix
          foundMatch = true; //flag true
        }
      }
      if (foundMatch == true) { //still no after searching for prefix
        cout << "Best movie with prefix " << s << " is: " << movies_rating_prefix.top().title << " with rating " << fixed << setprecision(1) << movies_rating_prefix.top().rating << endl;
      }
    }
    

    //  For each prefix,
    //  Find all movies that have that prefix and store them in an appropriate data structure
    //  If no movie with that prefix exists print the following message
    //cout << "No movies found with prefix "<<"<replace with prefix>" << endl;

    //  For each prefix,
    //  Print the highest rated movie with that prefix if it exists.

    return 0;
}

/* Add your run time analysis for part 3 of the assignment here as commented block*/

bool parseLine(string &line, string &movieName, double &movieRating) {
    int commaIndex = line.find_last_of(",");
    movieName = line.substr(0, commaIndex);
    movieRating = stod(line.substr(commaIndex+1));
    if (movieName[0] == '\"') {
        movieName = movieName.substr(1, movieName.length() - 2);
    }
    return true;
}
