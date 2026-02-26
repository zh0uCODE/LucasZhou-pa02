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
/*
3a: The worst case time complexity of the code for part 2a would be O(m(log k + log n)). In part 2a, there are m prefixes, and only the movies that start with a certain prefix are pushed 
    into a priority queue of ratings for that prefix. There are k movies that satisfy this condition. Also, the lower_bound binary search algorithm takes O(log n) time for each prefix for a total of
    m prefixes. In part 2b, there is one best movie for each prefix for a total of m prefixes, and each pop operation from the queue takes O(1) time. The overall time for part 2b would be O(m). Adding
    parts 2a and 2b together, the overall worst case time complexity is O(m(log k + log n + 1)).
    
    Running Times: 
    input_20_random.csv: 10ms 
    input_100_random.csv: 10ms
    input_1000_random.csv: 20ms
    input_76920_random.csv: 100ms
*/

/*
3b: The worst case space complexity for 2a would be O(k) because there are at most k movies that begin with each prefix that are pushed into the priority queue of ratings based on one prefix. 
    For part 2b, every prefix has one best movie for a total of m prefixes, so the worst case space complexity for 2b is O(m). For the overall program, the worst case space complexity would
    then be O(m + k).
*/

/*
3c: Initially, I ran a full search of all movies to find those with a certain prefix. It works, but runs too slow for Gradescope to pass all tests. I then used binary search, which made my code
    run faster. Using binary search for each movie takes O(log n) time compared to a full search of n movies, which would take O(n) time. Space complexity remains the same because you are still searching 
    for k movies that begin with a certain prefix. This makes the priority queue take up the same amount of memory, which is just k movies. So, making the program have a lower time complexity 
    did not change its space complexity.   
*/





bool parseLine(string &line, string &movieName, double &movieRating) {
    int commaIndex = line.find_last_of(",");
    movieName = line.substr(0, commaIndex);
    movieRating = stod(line.substr(commaIndex+1));
    if (movieName[0] == '\"') {
        movieName = movieName.substr(1, movieName.length() - 2);
    }
    return true;
}
