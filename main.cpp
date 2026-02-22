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
    vector<Movie> movies_alpha; //initalized pq, store by rating
    string line, movieName;
    double movieRating;
    // Read each file and store the name and rating
    while (getline (movieFile, line) && parseLine(line, movieName, movieRating)){
        Movie m{movieName, movieRating}; //create object
	movies_alpha.push_back(m); //push       
            // Use std::string movieName and double movieRating
            // to construct your Movie objects
            // cout << movieName << " has rating " << movieRating << endl;
            // insert elements into your data structure
    }

    movieFile.close();
    sort(movies_alpha.begin(), movies_alpha.end(), CompareMoviesAlpha()); //sort
    
    priority_queue<Movie, vector<Movie>, CompareMoviesAlphaPQ> temp; //copy to store movies alpha
    if (argc == 2){
            //print all the movies in ascending alphabetical order of movie names
            for (auto m : movies_alpha) {
	        cout << m.title << ", " << m.rating << endl; //print line by line!
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
    queue<pair<Movie, string>> best_movies; //queue for storing highest rated prefix
    for (auto s : prefixes) { //range for loop
      bool foundMatch = false; //didn't find a match yet
      priority_queue<Movie, vector<Movie>, CompareMovieRatingPQ> movies_rating_prefix; //make a sub queue of rating for one prefix
      /*
      for (auto m : movies_alpha) {
        if (m.title.starts_with(s)) { //title begins with prefix?
          //cout << m.title << ", " << m.rating << endl; //print
          movies_rating_prefix.push(m); //insert movies based on prefix
          foundMatch = true; //flag true 
        } 
      }
      */
      auto it = lower_bound(
        movies_alpha.begin(), movies_alpha.end(), s,
        [] (const Movie& m, const string& prefix) { //pass parameters for lower bound?
          return m.title < prefix; //alphabetical
        }
      );
      for (auto jt = it; jt != movies_alpha.end(); ++jt) { //iterator
        if (jt->title.starts_with(s)) { //starts with s
          movies_rating_prefix.push(*jt); //push
          foundMatch = true; //true match
        } else { //no?
          break;
        }
      }
      //2b prep
      if (foundMatch == false) { //still no after searching for prefix
        cout << "No movies found with prefix " << s << endl; //print this
      } else {
	best_movies.push({movies_rating_prefix.top(), s}); //push the beginning of every subset (same prefix, begin is the highest)
        while (!movies_rating_prefix.empty()) { //iterate through each subset
          cout << movies_rating_prefix.top().title << ", " << movies_rating_prefix.top().rating << endl; //print contents
          movies_rating_prefix.pop(); //pop!
        }
        cout << endl; // extra space between found movies only
      }
    } 
    
    //2b
    while(!best_movies.empty()) { //best movies not empty
      string title = best_movies.front().first.title; //this is movie name in the queue
      double rating =  best_movies.front().first.rating; //rating in the queue
      string prefix = best_movies.front().second; //string in the queue (prefix)
      cout << "Best movie with prefix " << prefix << " is: " << title << " with rating " << fixed << setprecision(1) << rating << endl; //print
      best_movies.pop(); //prepare for next
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
