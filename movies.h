#ifndef MOVIE_H
#define MOVIE_H
#include <iostream>
#include <string>
using namespace std;
struct Movie { //movie struct
  string title; //string title
  double rating; //rating
};

struct CompareMoviesAlpha { //functor to compare movies in alphabetical order
  bool operator()(const Movie& a, const Movie& b) const { //overloaded function op.
    return a.title < b.title; //will be true if the two titles are in alphabetical order "ie: a comes before b"
  }
};
//works!
struct CompareMoviesAlphaPQ { //priority queue version
  bool operator()(const Movie& a, const Movie& b) const {
    return a.title > b.title; //opposite of regular
  }
};
struct CompareMovieRating { //compare rating
  bool operator()(const Movie& a, const Movie& b) const { //overload function op.
    return a.rating > b.rating; //every a is bigger than b
  }
};
//set logic; same 
 
struct CompareMovieRatingPQ { //for the priority queue
  bool operator()(const Movie& a, const Movie& b) const { //overload ()
    if (a.rating < b.rating) { //a lower priority
      return true; //true
    } else if (a.rating > b.rating) { //b lower priority
      return false; //false
    } else { //same rating
      return a.title > b.title; //following priority queue logic. compare alphabetical 
    }
  }  
};
//pq logic:
//if b.rating > a.rating == true (a is less than b), a has lower priority, smaller movies move to the bottom, max heap (opposite of set)

  



#endif

 
