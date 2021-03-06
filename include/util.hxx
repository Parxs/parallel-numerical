#ifndef UTIL_HXX
#define UTIL_HXX

#include <cstdlib>
#include <limits>
#include <random>
#include <cmath>
#include <iostream>

/*====================================================================*/
// Implementations
/*====================================================================*/

/**
 * @brief Calculates the average of a given array.
 *
 * Because it is possible to use this function with a type that has an
 * impercise division every gotten value is casted to a type which 
 * allows more precise divisons.
 * This is still not perfect as e.g. the lowest digits may be lost when
 * the numbers are rather big but as there is no need for the resulting
 * average to be that precise it should be no problem at all.
 **/
template <typename T>
T get_Average(T *list, long sample_size){
  // double is used for higher precision when dividing
  double avg_time = 0;
  
  for(long i=0; i<sample_size; i++){
    double tmp = (double) list[i];
    
    avg_time += tmp/sample_size;
  }
  
  // add results together to get average
  avg_time = std::round(avg_time);
  return (T) avg_time;
}


/**
 * @brief Log the difference between two values into the given stream.
 **/
template <typename T>
void log_Difference(std::ostream& os, T expected, T gotten){
  using namespace std;
  os << scientific << showpos;
  os << "Expected:\t" << expected << "\n";
  os << "Got:     \t" << gotten << "\n";
  os << "Diff:    \t" << expected-gotten << "\n\n";
}


#endif




